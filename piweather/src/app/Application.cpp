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

#include    <device/Connection.hpp>
#include    <device/UidRegistry.hpp>

#include    <device/Button.hpp>
#include    <device/Lcd.hpp>
#include    <device/Observable.hpp>
#include    <device/IpAddress.hpp>

#include    "view/Illuminance.hpp"
#include    "view/AirPressure.hpp"
#include    "view/Humidity.hpp"
#include    "view/Temperature.hpp"

#include    "db/Database.hpp"

#include    <vector>
#include    <thread>

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
            unsigned column = 0;
            unsigned end = Lcd::Metrics::Columns / 2;

            if (++position % 2 == 0) {
                column = Lcd::Metrics::Columns / 2;
                end = Lcd::Metrics::Columns;
            }

            unsigned line = position / 2;

            sensorViews.emplace_back (
                    sensor,
                    lcd,
                    viewId,
                    view::Dimensions (line, column, end));

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
            public:
                virtual ~StateHandler () {}
                StateHandler (const Configuration&);

            private:
                static void wrapDisconnect (std::uint8_t, void*);

            private:
                void onDisconnect (std::uint8_t);
                void onButtonPressed ();
                void onLcdTimeout ();
                void onStoreValues ();

            private:
                device::Connection connection;
                device::UidRegistry uidRegistry;
                device::Button button;
                device::Lcd lcd;
                db::Database db;
                SensorViews sensorViews;
                Listener buttonListener;
        };

        StateHandler::StateHandler (const Configuration& config) :
            connection (config.host, config.port),
            uidRegistry (connection.get ()),
            button (connection.get (), uidRegistry, config.button),
            lcd (connection.get (), uidRegistry),
            db (config.dbPath),
            sensorViews (createSensorViews (connection.get (), lcd, config, uidRegistry)),
            buttonListener (std::bind (&StateHandler::onButtonPressed, this), nullptr)
        {
            button.addObserver (buttonListener);
            hookView (sensorViews);
            ipcon_register_callback (
                    connection.get (), IPCON_CALLBACK_DISCONNECTED,
                    reinterpret_cast<void*> (&StateHandler::wrapDisconnect), this);
        }

        void StateHandler::wrapDisconnect (std::uint8_t reason, void* s)
        {
            StateHandler* self = static_cast<StateHandler*> (s);
            self->onDisconnect (reason);
        }

        void StateHandler::onDisconnect (std::uint8_t)
        {
        }

        void StateHandler::onButtonPressed ()
        {
        }
    }

    bool Application::run (const Configuration& config)
    {
        bool success = true;

        StateHandler handler (config);

        return success;
    }
}}

