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

        struct Observed
        {
            template<typename Sensor, typename View>
                Observed (Sensor*, Lcd&, Id<View>);

            ObservablePtr sensor;
            ObserverPtr view;
        };

        typedef std::vector<Observed> Observers;

        template<typename Sensor, typename View>
            Observed::Observed (Sensor* s, Lcd& lcd, Id<View>) :
                sensor (s),
                view (new View (lcd, *s))
            {}

        Observers createObservers (
                IPConnection* connection,
                Lcd& lcd,
                const Configuration& config,
                const UidRegistry& registry)
        {
            Observers observers;

            observers.emplace_back (
                    new sensors::Illuminance (
                        connection, registry, config.illuminanceSensitivity ()),
                    lcd,
                    Id<view::Illuminance> ());

            observers.emplace_back (
                    new sensors::Temperature (connection, registry),
                    lcd,
                    Id<view::Temperature> ());

            observers.emplace_back (
                    new sensors::Barometer (
                        connection, registry, config.barometerSensitivity ()),
                    lcd,
                    Id<view::AirPressure> ());

            observers.emplace_back (
                    new sensors::Humidity (
                        connection, registry, config.humiditySensitivity ()),
                    lcd,
                    Id<view::Humidity> ());

            return observers;
        }

        void hookObservers (Observers& observers)
        {
            for (const Observed& o : observers) {
                o.sensor->addObserver (*(o.view));
            }
        }

        void unHookObservers (Observers& observers)
        {
            for (const Observed& o : observers) {
                o.sensor->removeObserver (*(o.view));
            }
        }
    }

    Application::Application (const Configuration& config) :
        configuration (config),
        db (config.dbPath ()),
        connection (config.host (), config.port ()),
        uidRegistry (connection.get ()),
        button (connection.get (), uidRegistry, 1), /* TODO choose correct button */
        lcd (connection.get (), uidRegistry),
        dbWriter ()
    {}

    Application::~Application ()
    {}

    bool Application::run ()
    {
        bool success = true;

        Observers observers = createObservers (
                connection.get (), lcd, configuration, uidRegistry);

        hookObservers (observers);

        return success;
    }
}}

