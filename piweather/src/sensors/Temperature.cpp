/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Temperature.cpp is free software copyrighted by David Daniel.
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
#include    "sensors/Temperature.hpp"

#include    <stdexcept>

namespace piw { namespace sensors {

    /**
     * Constructs a new Temperature.
     */
    Temperature::Temperature (
            IPConnection* connection,
            const device::UidRegistry& registry,
            std::int16_t threshold) :

        ThresholdObservable {threshold},
        barometer {new Barometer}
    {
        barometer_create (
                barometer.get (),
                registry.getUid (BAROMETER_DEVICE_IDENTIFIER).c_str (),
                connection);

        init ();

        if (barometer_set_air_pressure_callback_period (barometer.get (), 500) < 0) {
            throw std::runtime_error ("Cannot set the temperature callback period.");
        }

        barometer_register_callback (
                barometer.get (),
                BAROMETER_CALLBACK_AIR_PRESSURE,
                reinterpret_cast<void*> (&Temperature::wrap), this);
    }

    Temperature::~Temperature ()
    { barometer_destroy (barometer.get ()); }

    std::int16_t Temperature::read ()
    {
        std::int16_t temp;

        if (barometer_get_chip_temperature (barometer.get (), &temp) < 0) {
            throw std::runtime_error ("Cannot get the temperature.");
        }

        return temp;
    }

    void Temperature::check (std::int32_t)
    {
        std::int16_t thresh = threshold ();
        std::int16_t observed = value ();
        std::int16_t current = read ();

        bool trigger
            = current > observed
            ? (observed + thresh) <= current
            : (observed - thresh) >= current;

        if (trigger) {
            value (current);
            notifyObservers ();
        }
    }

    void Temperature::wrap (std::int32_t value, void* user_data)
    {
        Temperature* self = static_cast<Temperature*> (user_data);

        try {
            self->check (value);
        }
        catch (const std::exception&) { /* ignored */ }
    }
}}

