/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Barometer.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_SENSORS_BAROMETER_INC
#define PIW_SENSORS_BAROMETER_INC

#include    "device/UidRegistry.hpp"
#include    "sensors/ThresholdObservable.hpp"

#include    "ip_connection.h"
#include    "bricklet_barometer.h"

#include    <memory>

namespace piw { namespace sensors {

    class Barometer : public ThresholdObservable<std::int32_t>
    {
        friend class ThresholdObservable;

        public:
            Barometer (IPConnection*, const device::UidRegistry&, std::int32_t);
            virtual ~Barometer ();

            double pressure () const;

        protected:
            virtual void adjust ();
            virtual std::int32_t read ();

        protected:
            virtual void valueChanged (std::int32_t);

        private:
            std::unique_ptr<::Barometer> barometer;
    };

    inline double Barometer::pressure () const
    { return value (); }
}}

#endif /* PIW_SENSORS_BAROMETER_INC */

