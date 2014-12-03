/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * IpAddress.cpp is free software copyrighted by David Daniel.
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
#include    <device/IpAddress.hpp>

#include    <stdexcept>

#include    <unistd.h>
#include    <netdb.h>
#include    <arpa/inet.h>

namespace piw { namespace device {

    IpAddress& IpAddress::reset ()
    {
        char hostname [128];

        if (gethostname (hostname, sizeof hostname) < 0) {
            throw std::runtime_error ("Cannot get the hostname of the device.");
        }

        struct hostent host;
        struct hostent* result;
        char buffer [128];
        int error;

        if (gethostbyname_r (hostname, &host, buffer, sizeof buffer, &result, &error) != 0
                || result == nullptr)
        {
            std::cerr << std::strerror (error) << std::endl;
            throw std::runtime_error ("Cannot get the host by name.");
        }

        char ip [INET_ADDRSTRLEN + 1] = { 0 };

        if (!inet_ntop (AF_INET, result->h_addr, ip, sizeof ip)) {
            throw std::runtime_error (
                    "Cannot convert the ip address to a human readable string.");
        }

        address = ip;

        return *this;
    }
}}
