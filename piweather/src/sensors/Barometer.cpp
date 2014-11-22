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

namespace piw { namespace sensors {

    /**
     * Constructs a new Barometer.
     */
    Barometer::Barometer (
            IPConnection* connection,
            const device::UidRegistry& registry,
            std::int32_t threshold) :

        ThresholdObservable {threshold},
        barometer {new ::Barometer ()}
    {
        barometer_create (
                barometer.get (),
                registry.getUid (BAROMETER_DEVICE_IDENTIFIER).c_str (),
                connection);

        init ();

        barometer_register_callback (
                barometer.get (),
                BAROMETER_CALLBACK_AIR_PRESSURE_REACHED,
                wrapper (), this);
    }

    Barometer::~Barometer ()
    { barometer_destroy (barometer.get ()); }

    std::int32_t Barometer::read ()
    {
        std::int32_t value;

        if (barometer_get_air_pressure (barometer.get (), &value) < 0) {
            throw std::runtime_error ("Cannot get the air pressure.");
        }

        return value;
    }

    void Barometer::adjust ()
    {
        std::int32_t current = value ();
        std::int32_t thresh = threshold ();

        int result = barometer_set_air_pressure_callback_threshold (
                barometer.get (), 'o', current - thresh, current + thresh);

        if (result < 0) {
            throw std::runtime_error ("Cannot adjust the threshold");
        }
    }

    void Barometer::valueChanged (std::int32_t v)
    {
        value (v);
        adjust ();
        notifyObservers ();
    }
}}

