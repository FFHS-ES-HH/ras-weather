/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Temperature.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_SENSORS_TEMPERATURE_INC
#define PIW_SENSORS_TEMPERATURE_INC

#include    <device/Observable.hpp>
#include    <device/UidRegistry.hpp>
#include    <device/ConnectedDevice.hpp>

#include    <sensors/ThresholdObservable.hpp>

#include    <bricklet_barometer.h>
#include    <ip_connection.h>
#include    <chrono>

namespace piw { namespace sensors {

    class Temperature : public ThresholdObservable<std::int16_t>, public ConnectedDevice
    {
        public:
            Temperature (
                    const Connection&,
                    const device::UidRegistry&,
                    std::int16_t = 10,
                    const std::chrono::milliseconds& = std::chrono::milliseconds {500});

            virtual ~Temperature ();

            double celsius () const;

        protected:
            virtual std::int16_t read ();
            virtual void adjust (std::int16_t, std::int16_t) {}

            virtual void valueChanged (std::int16_t) {}
            void check (std::int32_t);

            static void wrap (std::int32_t, void*);

        private:
            Barometer barometer;
    };

    inline double Temperature::celsius () const
    { return value () / 100.0; }
}}

#endif /* PIW_SENSORS_TEMPERATURE_INC */

