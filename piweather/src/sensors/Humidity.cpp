/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Humidity.cpp is free software copyrighted by David Daniel.
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
#include    <sensors/Humidity.hpp>

#include    <stdexcept>

namespace piw { namespace sensors {

    namespace {
        device::DeviceRegistration deviceRegistration {HUMIDITY_DEVICE_IDENTIFIER};
    }

    /**
     * Constructs a new Humidity.
     */
    Humidity::Humidity (
            const Connection& connection,
            const device::UidRegistry& registry,
            std::uint16_t threshold) :

        ConnectedDevice (connection),
        ThresholdObservable {std::uint16_t (threshold * 10)}
    {
        humidity_create (
                &sensor,
                registry.getUid (HUMIDITY_DEVICE_IDENTIFIER).c_str (),
                connection ());

        init ();

        humidity_register_callback (
                &sensor,
                HUMIDITY_CALLBACK_HUMIDITY_REACHED,
                wrapper (), this);
    }

    Humidity::~Humidity ()
    {
        humidity_destroy (&sensor);
    }

    std::uint16_t Humidity::read ()
    {
        std::uint16_t value;

        if (humidity_get_humidity (&sensor, &value) < 0) {
            throw std::runtime_error ("Cannot get the humidity.");
        }

        return value;
    }

    void Humidity::adjust (std::uint16_t low, std::uint16_t high)
    {
        int result = humidity_set_humidity_callback_threshold (&sensor, 'o', low, high);

        if (result < 0) {
            throw std::runtime_error ("Cannot adjust the humidity threshold.");
        }
    }

    void Humidity::valueChanged (std::uint16_t current)
    {
        value (current);
        triggerAdjust ();
        notifyObservers ();
    }
}}
