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
#include    <atomic>
#include    <mutex>
#include    <condition_variable>
#include    <iostream>

#include    <signal.h>

namespace piw { namespace app {

    using device::Lcd;
    using device::UidRegistry;
    using device::Observer;
    using device::Button;

    using view::WeatherView;

    namespace {

        class SignalHandler
        {
            public:
                static int wait ();

            private:
                SignalHandler ();

                static SignalHandler& instance ();
                static void handle (int, siginfo_t*, void*);

            private:
                std::mutex mutex;
                std::condition_variable condition;
                std::atomic_int signal;
                std::atomic_bool triggered;
        };

        SignalHandler::SignalHandler () :
            mutex (),
            condition (),
            signal (0),
            triggered (false)
        {
            struct sigaction handler;
            handler.sa_sigaction = SignalHandler::handle;
            handler.sa_flags = SA_SIGINFO;
            sigemptyset (&handler.sa_mask);

            sigaction (SIGTERM, &handler, nullptr);
            sigaction (SIGINT, &handler, nullptr);
        }

        void SignalHandler::handle (int n, siginfo_t*, void*)
        {
            SignalHandler& self = instance ();

            self.signal.store (n);
            self.triggered.store (true);
            self.condition.notify_all ();
        }

        SignalHandler& SignalHandler::instance ()
        {
            static SignalHandler handler;

            return handler;
        }

        int SignalHandler::wait ()
        {
            SignalHandler& self = SignalHandler::instance ();
            bool running = true;
            int result = 0;

            while (running) {

                std::unique_lock<std::mutex> lock (self.mutex);
                self.condition.wait (lock);

                if (self.triggered.load ()) {

                    result = self.signal.load ();

                    self.triggered.store (running = false);
                }
            }

            return result;
        }

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

                int run ();

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
            uidRegistry {connection},
            button {connection, uidRegistry, config.button},
            lcd {connection, uidRegistry},
            db {config.dbPath},
            configuration (config),
            sensorViews {createSensorViews (connection, lcd, config, uidRegistry)},
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

        int StateHandler::run ()
        { return SignalHandler::wait (); }

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
            if (lcd.isBacklightOn ()) {
                lcd.clear ();
                backlightOn ();

                switch (viewState) {
                    case Normal:
                        unHookView (sensorViews);
                        displayIp ();
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
            else {
                backlightOn ();
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

