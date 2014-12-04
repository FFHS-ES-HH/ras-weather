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

#include    <net/if.h>
#include    <sys/types.h>
#include    <ifaddrs.h>
#include    <sys/socket.h>
#include    <netdb.h>

namespace piw { namespace device {

    namespace {
        struct AddrFree
        {
            AddrFree (struct ifaddrs*);
            ~AddrFree ();

            struct ifaddrs* address;
        };

        AddrFree::AddrFree (struct ifaddrs* a) :
            address (a)
        {}

        AddrFree::~AddrFree ()
        { freeifaddrs (address); }
    }

    IpAddress& IpAddress::reset ()
    {
        struct ifaddrs* addresses;
        char ip [NI_MAXHOST] = {0};

        if (getifaddrs (&addresses) < 0) {
            throw std::runtime_error ("Cannot reach the interfaces.");
        }

        AddrFree a (addresses);

        const std::string lo {"lo"};

        for (struct ifaddrs* current = addresses; current != nullptr; current = current->ifa_next) {

            if (!current->ifa_addr) {
                continue;
            }

            int family = current->ifa_addr->sa_family;

            if (family != AF_INET /* && family != AF_INET6 */) {
                continue;
            }

            if (lo.compare (0, 2, current->ifa_name) == 0) continue;

            int result = getnameinfo (current->ifa_addr,
                    family == AF_INET
                    ? sizeof (struct sockaddr_in)
                    : sizeof (struct sockaddr_in6),
                    ip, sizeof ip, nullptr, 0, NI_NUMERICHOST);

            if (result) {
                throw std::runtime_error ("Cannot get the ip address.");
            }

            address = ip;
            break;
        }

        return *this;
    }
}}
