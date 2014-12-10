/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Connection.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_DEVICE_CONNECTION_INC
#define PIW_DEVICE_CONNECTION_INC

#include    <ip_connection.h>

#include    <cstdint>
#include    <memory>

namespace piw { namespace device {

    class Connection
    {
        public:
            Connection (const std::string&, std::uint16_t);
            ~Connection ();

            Connection (const Connection&);
            Connection& operator= (const Connection&);

            Connection (Connection&&) = default;
            Connection& operator= (Connection&&) = default;

            IPConnection* get () const;

        private:
            std::unique_ptr<IPConnection> connection_;
            std::string host_;
            std::uint16_t port_;
    };

    inline IPConnection* Connection::get () const
    { return connection_.get (); }
}}

#endif /* PIW_DEVICE_CONNECTION_INC */

