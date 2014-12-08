/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Illuminance.cpp is free software copyrighted by David Daniel.
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
#include    <sensors/Illuminance.hpp>

namespace piw { namespace sensors {

    namespace {
        device::DeviceRegistration deviceRegistration {AMBIENT_LIGHT_DEVICE_IDENTIFIER};
    }

    Illuminance::Illuminance (
            IPConnection* connection,
            const device::UidRegistry& registry,
            std::uint16_t threshold) :

        ThresholdObservable {threshold}
    {
        ambient_light_create (
                &sensor,
                registry.getUid (AMBIENT_LIGHT_DEVICE_IDENTIFIER).c_str (),
                connection);

        init ();

        ambient_light_register_callback (
                &sensor,
                AMBIENT_LIGHT_CALLBACK_ILLUMINANCE_REACHED,
                wrapper (), this);
    }

    Illuminance::~Illuminance ()
    { ambient_light_destroy (&sensor); }

    std::uint16_t Illuminance::read ()
    {
        std::uint16_t value;

        if (ambient_light_get_illuminance (&sensor, &value) < 0) {
            throw std::runtime_error ("Cannot get the illuminance.");
        }

        return value;
    }

    void Illuminance::adjust (std::uint16_t low, std::uint16_t high)
    {
        int result = ambient_light_set_illuminance_callback_threshold (
                &sensor, 'o', low, high);

        if (result < 0) {
            throw std::runtime_error ("Cannot adjust the threshold of the illuminance.");
        }
    }

    void Illuminance::valueChanged (std::uint16_t current)
    {
        value (current);
        triggerAdjust ();
    }
}}

