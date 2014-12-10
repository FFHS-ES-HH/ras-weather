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

#include    <ifaddrs.h>
#include    <netdb.h>

namespace piw { namespace device {

    namespace {

        struct IfaceCloser
        {
            IfaceCloser (ifaddrs*&);
            ~IfaceCloser ();

            ifaddrs*& ifaces;
        };

        inline IfaceCloser::IfaceCloser (ifaddrs*& interfaces) :
            ifaces (interfaces)
        {}

        inline IfaceCloser::~IfaceCloser ()
        { freeifaddrs (ifaces); }
    }

    IpAddress::IpAddress () :
        address {}
    {
        ifaddrs* interfaces;

        if (getifaddrs (&interfaces) < 0) {
            throw std::runtime_error (
                    "Cannot reach the network interfaces in order to get the ip address.");
        }

        IfaceCloser c {interfaces};
        const std::string loopback {"lo"};

        for (ifaddrs* iface = interfaces; iface != nullptr; iface = iface->ifa_next) {

            if (!iface->ifa_addr) {
                continue;
            }

            int family = iface->ifa_addr->sa_family;

            if (family != AF_INET /* && family != AF_INET6 */) {
                continue;
            }

            if (loopback.compare (0, 2, iface->ifa_name) == 0) {
                continue;
            }

            char ip [128] = {0};

            int error = getnameinfo (iface->ifa_addr,
                    family == AF_INET ? sizeof (sockaddr_in) : sizeof (sockaddr_in6),
                    ip, sizeof ip, nullptr, 0, NI_NUMERICHOST);

            if (error) {
                throw std::runtime_error ("Cannot get the ip address of the interface.");
            }

            address.clear ();
            for (const char* current = ip; *current != '\0'; ++current) {
                address += *current;
            }

            break;
        }
    }
}}
