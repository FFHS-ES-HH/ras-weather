/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Barometer.cpp is free software copyrighted by David Daniel.
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
#include    <sensors/Barometer.hpp>

#include    <stdexcept>

namespace piw { namespace sensors {

    namespace {
        const device::DeviceRegistration deviceRegistration {BAROMETER_DEVICE_IDENTIFIER};
    }

    /**
     * Constructs a new Barometer.
     */
    Barometer::Barometer (
            const device::Connection& conn,
            const device::UidRegistry& registry,
            double threshold) :

        ThresholdObservable {std::int32_t (threshold * 1000.0)},
        ConnectedDevice (conn)
    {
        barometer_create (
                &barometer,
                registry.getUid (BAROMETER_DEVICE_IDENTIFIER).c_str (),
                connection ());

        init ();

        barometer_register_callback (
                &barometer,
                BAROMETER_CALLBACK_AIR_PRESSURE_REACHED,
                wrapper (), this);
    }

    Barometer::~Barometer ()
    { barometer_destroy (&barometer); }

    std::int32_t Barometer::read ()
    {
        std::int32_t value;

        if (barometer_get_air_pressure (&barometer, &value) < 0) {
            throw std::runtime_error ("Cannot get the air pressure.");
        }

        return value;
    }

    void Barometer::adjust (std::int32_t low, std::int32_t high)
    {
        int result = barometer_set_air_pressure_callback_threshold (
                &barometer, 'o', low, high);

        if (result < 0) {
            throw std::runtime_error ("Cannot adjust the air pressure threshold.");
        }
    }

    void Barometer::valueChanged (std::int32_t v)
    {
        value (v);
        triggerAdjust ();
        notifyObservers ();
    }
}}

