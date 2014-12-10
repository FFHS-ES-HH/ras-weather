/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Humidity.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_SENSORS_HUMIDITY_INC
#define PIW_SENSORS_HUMIDITY_INC

#include    <bricklet_humidity.h>
#include    <ip_connection.h>

#include    <device/UidRegistry.hpp>
#include    <device/ConnectedDevice.hpp>
#include    <sensors/ThresholdObservable.hpp>

namespace piw { namespace sensors {

    class Humidity : public ThresholdObservable<std::uint16_t>, public ConnectedDevice
    {
        public:
            Humidity (const Connection&, const device::UidRegistry&, std::uint16_t);
            virtual ~Humidity ();

            double humidity () const;

        protected:
            virtual void adjust (std::uint16_t, std::uint16_t);
            virtual std::uint16_t read ();
            virtual void valueChanged (std::uint16_t);

        private:
            ::Humidity sensor;
    };

    inline double Humidity::humidity () const
    { return value () / 10.0; }
}}

#endif /* PIW_SENSORS_HUMIDITY_INC */

