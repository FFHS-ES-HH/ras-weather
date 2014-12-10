/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * ConnectedDevice.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_DEVICE_CONNECTEDDEVICE_INC
#define PIW_DEVICE_CONNECTEDDEVICE_INC

#include    <device/Connection.hpp>

namespace piw { namespace device {

    class ConnectedDevice
    {
        public:
            virtual ~ConnectedDevice ();

        protected:
            ConnectedDevice (const Connection&);

            IPConnection* connection ();

        private:
            Connection connection_;
    };

    inline ConnectedDevice::ConnectedDevice (const Connection& connection) :
        connection_ (connection)
    {}

    inline IPConnection* ConnectedDevice::connection ()
    { return connection_.get (); }
}}

#endif /* PIW_DEVICE_CONNECTEDDEVICE_INC */

