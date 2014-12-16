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
#include    "app/SignalHandler.hpp"

#include    <device/Connection.hpp>
#include    <device/UidRegistry.hpp>

#include    <device/Button.hpp>
#include    <device/Lcd.hpp>

#include    <sensors/SensorDevice.hpp>

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

        typedef std::unique_ptr<sensors::SensorDevice> SensorPtr;
        typedef std::unique_ptr<WeatherView> ViewPtr;

        template<typename T>
            struct Id { typedef T type; };

        struct SensorView
        {
            template<typename Sensor, typename View>
                SensorView (Sensor*, Lcd&, Id<View>, view::Dimensions);

            SensorView () = default;

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
                const device::Connection& connection,
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

        void hookView (const SensorView& sensorView)
        {
            sensorView.sensor->addObserver (*(sensorView.view));
            sensorView.view->valueChanged ();
        }

        void hookViews (const SensorViews& sensorViews)
        {
            for (const SensorView& s : sensorViews) {
                hookView (s);
            }
        }

        void unHookView (const SensorView& sensorView)
        {
            sensorView.sensor->removeObserver (*(sensorView.view));
        }

        void unHookViews (const SensorViews& sensorViews)
        {
            for (const SensorView& s : sensorViews) {
                unHookView (s);
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
                Disconnected
            };

            public:
                virtual ~StateHandler () {}
                StateHandler (const Configuration&);

                int run ();

            private:
                static void wrapDisconnect (std::uint8_t, void*);

            private:
                void onDisconnect (std::uint8_t);
                void onButtonPressed ();
                void onLcdTimeout ();
                void onStoreValues ();

            private:
                void backlightOn ();

            private:
                device::Connection connection;
                device::UidRegistry uidRegistry;
                device::Button button;
                device::Lcd lcd;
                db::Database db;
                const Configuration& configuration;
                SensorViews sensorViews;
                SensorView ipView;
                Listener buttonListener;
                ViewState viewState;
                AsyncTimer dbWriter;
                AsyncTimer lcdDimmer;
        };

        StateHandler::StateHandler (const Configuration& config) :
            connection {config.host, config.port},
            uidRegistry {connection},
            button {connection, uidRegistry, config.button},
            lcd {connection, uidRegistry},
            db {config.dbPath},
            configuration (config),
            sensorViews {createSensorViews (connection, lcd, config, uidRegistry)},
            ipView {},
            buttonListener {std::bind (&StateHandler::onButtonPressed, this), nullptr},
            viewState {Normal},
            dbWriter {std::bind (&StateHandler::onStoreValues, this)},
            lcdDimmer {std::bind (&Lcd::backlightOff, &lcd)}
        {
            backlightOn ();
            hookViews (sensorViews);

            button.addObserver (buttonListener);

            ipcon_register_callback (
                    connection.get (), IPCON_CALLBACK_DISCONNECTED,
                    reinterpret_cast<void*> (&StateHandler::wrapDisconnect), this);

            dbWriter.start (config.saveInterval);
            std::unique_ptr<sensors::IpAddress> a {new sensors::IpAddress};
            ipView.view.reset (new view::IpView {lcd, *a});
            ipView.sensor.reset (a.release ());
        }

        int StateHandler::run ()
        {
            int sig = 0;

            while (sig == 0) {
                sig = SignalHandler::wait (1000);

                for (SensorView& sensorView : sensorViews) {
                    sensorView.sensor->status ();
                }
            }
            
            return sig;
        }

        void StateHandler::wrapDisconnect (std::uint8_t reason, void* s)
        {
            StateHandler* self = static_cast<StateHandler*> (s);
            try {
                self->onDisconnect (reason);
            }
            catch (const std::exception&) {}
        }

        void StateHandler::onDisconnect (std::uint8_t reason)
        {
            viewState = Disconnected;

            std::wstring error;

            switch (reason) {
                case IPCON_DISCONNECT_REASON_REQUEST:
                    error = L"Verbindung getrennt";
                    break;
                case IPCON_DISCONNECT_REASON_ERROR:
                    error = L"Keine Verbindung";
                    break;
                case IPCON_DISCONNECT_REASON_SHUTDOWN:
                    error = L"Gerät fährt herunter";
                    break;
            }

            view::ErrorView view (lcd);
            view.display (error);
        }

        void StateHandler::onButtonPressed ()
        {
            if (lcd.isBacklightOn ()) {
                lcd.clear ();
                backlightOn ();

                switch (viewState) {
                    case Normal:
                        viewState = Ip;
                        hookView (ipView);
                        unHookViews (sensorViews);
                        break;
                    case Ip:
                        viewState = Normal;
                        unHookView (ipView);
                        hookViews (sensorViews);
                        break;
                    case Disconnected:
                        viewState = Normal;
                        hookViews (sensorViews);
                        break;
                }
            }
            else {
                backlightOn ();
            }
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
        bool running = true;

        while (running) {

            try {
                StateHandler handler (config);
                handler.run ();

                running = false;
            }
            catch (const std::exception& e) {
                success = false;

                std::cerr
                    << "Error in application execution: " << e.what () << std::endl;

                if (++errorCounter > 5) {
                    running = false;
                }

                // try to reconnect
                std::this_thread::sleep_for (std::chrono::seconds {2});
            }
        }

        return success;
    }
}}

