/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Application.cpp is free software copyrighted by David Daniel.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions.
 */
#include    "app/Application.hpp"
#include    "app/AsyncTimer.hpp"

#include    <device/Connection.hpp>
#include    <device/UidRegistry.hpp>

#include    <device/Button.hpp>
#include    <device/Lcd.hpp>
#include    <device/Observable.hpp>

#include    "view/Illuminance.hpp"
#include    "view/AirPressure.hpp"
#include    "view/Humidity.hpp"
#include    "view/Temperature.hpp"
#include    "view/IpView.hpp"
#include    "view/ErrorView.hpp"

#include    "db/Database.hpp"

#include    <vector>
#include    <thread>
#include    <iostream>

namespace piw { namespace app {

    using device::Lcd;
    using device::UidRegistry;
    using device::Observer;
    using device::Button;

    using view::WeatherView;

    namespace {

        typedef std::unique_ptr<device::Observable> SensorPtr;
        typedef std::unique_ptr<WeatherView> ViewPtr;

        template<typename T>
            struct Id { typedef T type; };

        struct SensorView
        {
            template<typename Sensor, typename View>
                SensorView (Sensor*, Lcd&, Id<View>, view::Dimensions);

            SensorPtr sensor;
            ViewPtr view;
        };

        typedef std::vector<SensorView> SensorViews;

        template<typename Sensor, typename View>
            SensorView::SensorView (
                    Sensor* s,
                    Lcd& lcd,
                    Id<View>,
                    view::Dimensions dimensions) :

                sensor (s),
                view (new View (lcd, *s, dimensions))
            {}

        struct SensorViewCollector
        {
            SensorViewCollector (SensorViews&, Lcd&);

            template<typename Sensor, typename View>
                SensorViewCollector& add (Sensor*, Id<View>);

            SensorViews& sensorViews;
            Lcd& lcd;
            size_t position;
        };

        SensorViewCollector::SensorViewCollector (SensorViews& o, Lcd& l) :
            sensorViews (o),
            lcd (l),
            position (0)
        {}

        template<typename Sensor, typename View>
            SensorViewCollector& SensorViewCollector::add (Sensor* sensor, Id<View> viewId)
        {
            sensorViews.emplace_back (
                    sensor,
                    lcd,
                    viewId,
                    view::Dimensions::byPosition (++position));

            return *this;
        }

        SensorViews createSensorViews (
                IPConnection* connection,
                Lcd& lcd,
                const Configuration& config,
                const UidRegistry& registry)
        {
            SensorViews sensorViews;
            SensorViewCollector collector (sensorViews, lcd);

            collector
                .add (
                        new sensors::Illuminance (
                            connection, registry, config.illuminanceSensitivity),
                        Id<view::Illuminance> ())

                .add (
                        new sensors::Temperature (
                            connection, registry,
                            config.temperatureSensitivity,
                            config.pollInterval),
                        Id<view::Temperature> ())

                .add (
                        new sensors::Barometer (
                            connection, registry, config.barometerSensitivity),
                        Id<view::AirPressure> ())

                .add (
                        new sensors::Humidity (
                            connection, registry, config.humiditySensitivity),
                        Id<view::Humidity> ());

            return sensorViews;
        }

        void hookView (const SensorViews& sensorViews)
        {
            for (const SensorView& s : sensorViews) {
                s.sensor->addObserver (*(s.view));
                s.view->valueChanged ();
            }
        }

        void unHookView (const SensorViews& sensorViews)
        {
            for (const SensorView& s : sensorViews) {
                s.sensor->removeObserver (*(s.view));
            }
        }

        class Listener : public device::Observer
        {
            public:
                virtual ~Listener () {}
                Listener () = default;

                template<typename V, typename E>
                    Listener (V, E);

                virtual void valueChanged ();
                virtual void onError (const std::exception&);

            private:
                std::function<void ()> valueListener;
                std::function<void (const std::exception&)> errorListener;
        };

        template<typename V, typename E>
            inline Listener::Listener (V v, E e) :
                valueListener (v),
                errorListener (e)
            {}

        void Listener::valueChanged ()
        {
            if (valueListener) {
                valueListener ();
            }
        }

        void Listener::onError (const std::exception& error)
        {
            if (errorListener) {
                errorListener (error);
            }
        }

        class StateHandler
        {
            enum ViewState
            {
                Normal,
                Ip,
                Error
            };

            public:
                virtual ~StateHandler () {}
                StateHandler (const Configuration&);

                void block ();

            private:
                static void wrapDisconnect (std::uint8_t, void*);

            private:
                void onDisconnect (std::uint8_t);
                void onButtonPressed ();
                void onLcdTimeout ();
                void onStoreValues ();

            private:
                void displayIp ();
                void backlightOn ();

            private:
                device::Connection connection;
                device::UidRegistry uidRegistry;
                device::Button button;
                device::Lcd lcd;
                db::Database db;
                const Configuration& configuration;
                SensorViews sensorViews;
                Listener buttonListener;
                ViewState viewState;
                AsyncTimer dbWriter;
                AsyncTimer lcdDimmer;
        };

        StateHandler::StateHandler (const Configuration& config) :
            connection {config.host, config.port},
            uidRegistry {connection.get ()},
            button {connection.get (), uidRegistry, config.button},
            lcd {connection.get (), uidRegistry},
            db {config.dbPath},
            configuration (config),
            sensorViews {createSensorViews (connection.get (), lcd, config, uidRegistry)},
            buttonListener {std::bind (&StateHandler::onButtonPressed, this), nullptr},
            viewState {Normal},
            dbWriter {std::bind (&StateHandler::onStoreValues, this)},
            lcdDimmer {std::bind (&Lcd::backlightOff, &lcd)}
        {
            backlightOn ();
            hookView (sensorViews);

            button.addObserver (buttonListener);

            ipcon_register_callback (
                    connection.get (), IPCON_CALLBACK_DISCONNECTED,
                    reinterpret_cast<void*> (&StateHandler::wrapDisconnect), this);

            dbWriter.start (config.saveInterval);
        }


        void StateHandler::block ()
        {
            std::mutex mutex;
            std::unique_lock<std::mutex> lock (mutex);
            std::condition_variable condition;
            condition.wait (lock);
        }

        void StateHandler::wrapDisconnect (std::uint8_t reason, void* s)
        {
            StateHandler* self = static_cast<StateHandler*> (s);
            self->onDisconnect (reason);
        }

        void StateHandler::onDisconnect (std::uint8_t reason)
        {
            std::wstring error;

            switch (reason) {
                case IPCON_DISCONNECT_REASON_REQUEST:
                    // error = L"Device disconnected";
                    break;
                case IPCON_DISCONNECT_REASON_ERROR:
                    // error = L"Device not available";
                    break;
                case IPCON_DISCONNECT_REASON_SHUTDOWN:
                    // error = L"Device shutting down";
                    break;
            }

            viewState = Error;
        }

        void StateHandler::onButtonPressed ()
        {
            backlightOn ();

            switch (viewState) {
                case Normal:
                    unHookView (sensorViews);
                    viewState = Ip;
                    break;
                case Ip:
                    hookView (sensorViews);
                    viewState = Normal;
                    break;
                case Error:
                    hookView (sensorViews);
                    viewState = Normal;
                    break;
            }
        }

        void StateHandler::displayIp ()
        {
            view::IpView ip (lcd);
            ip.display ();
        }

        void StateHandler::onStoreValues ()
        {
            try {
                db::Values values = db::Values ();
                values.created_at = std::chrono::system_clock::now ();

                for (const SensorViews::value_type& s : sensorViews) {
                    s.view->storeValue (values);
                }

                db.storeValues (values);
            }
            catch (const std::exception& /* ignored */) {}
        }

        void StateHandler::onLcdTimeout ()
        {
            try {
                lcd.backlightOff ();
            }
            catch (const std::exception& /* ignored */) {}
        }

        void StateHandler::backlightOn ()
        {
            lcd.backlightOn ();
            if (configuration.lcdTimeout > std::chrono::milliseconds::zero ()) {
                lcdDimmer.start (configuration.lcdTimeout, true);
            }
        }
    }

    bool Application::run (const Configuration& config)
    {
        bool success = true;
        size_t errorCounter = 0;

        while (true) {

            try {
                StateHandler handler (config);
                handler.block ();
            }
            catch (const std::exception& e) {
                success = false;
                std::cerr
                    << "Error in application execution: " << e.what () << std::endl;

                if (++errorCounter > 5) {
                    break;
                }

                // try to reconnect
                std::this_thread::sleep_for (
                        std::chrono::milliseconds {500});
            }
        }

        return success;
    }
}}

