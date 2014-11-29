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

#include    <ip_connection.h>

#include    <device/Lcd.hpp>
#include    <device/UidRegistry.hpp>

#include    <sensors/Illuminance.hpp>
#include    <sensors/Barometer.hpp>
#include    <sensors/Humidity.hpp>
#include    <sensors/Temperature.hpp>

#include    <view/Illuminance.hpp>
#include    <view/AirPressure.hpp>
#include    <view/Humidity.hpp>
#include    <view/Temperature.hpp>

#include    <vector>

namespace piw { namespace app {

    using device::Lcd;
    using device::UidRegistry;
    using device::Observable;
    using device::Observer;

    namespace {

        typedef std::unique_ptr<Observable> ObservablePtr;
        typedef std::unique_ptr<Observer> ObserverPtr;

        template<typename T>
            struct Id { typedef T type; };

        struct SensorView
        {
            template<typename Sensor, typename View>
                SensorView (Sensor*, Lcd&, Id<View>);

            ObservablePtr sensor;
            ObserverPtr view;
        };

        typedef std::vector<SensorView> SensorViews;

        template<typename Sensor, typename View>
            SensorView::SensorView (Sensor* s, Lcd& lcd, Id<View>) :
                sensor (s),
                view (new View (lcd, *s))
            {}

        struct SensorViewCollector
        {
            SensorViewCollector (SensorViews&, Lcd&);

            template<typename Sensor, typename View>
                SensorViewCollector& add (Sensor*, Id<View>);

            SensorViews& sensorViews;
            Lcd& lcd;
        };

        SensorViewCollector::SensorViewCollector (SensorViews& o, Lcd& l) :
            sensorViews (o),
            lcd (l)
        {}

        template<typename Sensor, typename View>
            SensorViewCollector& SensorViewCollector::add (Sensor* sensor, Id<View> viewId)
        {
            sensorViews.emplace_back (sensor, lcd, viewId);
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

        void hookView (SensorViews& sensorViews)
        {
            for (const SensorView& o : sensorViews) {
                o.sensor->addObserver (*(o.view));
            }
        }

        void unHookView (SensorViews& sensorViews)
        {
            for (const SensorView& o : sensorViews) {
                o.sensor->removeObserver (*(o.view));
            }
        }
    }

    Application::Application (const Configuration& config) :
        configuration (config),
        db (config.dbPath),
        connection (config.host, config.port),
        uidRegistry (connection.get ()),
        button (connection.get (), uidRegistry, config.button),
        lcd (connection.get (), uidRegistry),
        dbWriter ()
    {}

    Application::~Application ()
    {}

    bool Application::run ()
    {
        bool success = true;

        SensorViews sensorViews = createSensorViews (
                connection.get (), lcd, configuration, uidRegistry);

        hookView (sensorViews);

        return success;
    }
}}

