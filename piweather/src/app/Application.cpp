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
        struct Observed
        {
            template<typename Sensor, typename Viewer>
                Observed (Sensor*, Lcd&);

            std::unique_ptr<Observable> sensor;
            std::unique_ptr<Observer> view;
        };

        template<typename Sensor, typename Viewer, typename ...Args>
        Observed::Observed (Sensor* s, Lcd& lcd) :
            sensor (s),
            view (new View (lcd, *sensor))
        {} 

        typedef std::vector<Observed> Observers;

        Observers createObservers (
                Lcd& lcd,
                const UidRegistry& registry,
                IPConnection* connection)
        {
            Observers observers {
                Observed<sensor::Illuminance, view::Illuminance> (
                        new sensor::Illuminance (conection, registry, 1), lcd)
            };
        }
    }

    Application::Application (const Configuration& config) :
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

        return success;
    }
}}

