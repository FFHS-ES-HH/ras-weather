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

#include    <asm/types.h>
#include    <sys/socket.h>
#include    <linux/rtnetlink.h>

#include    <sys/select.h>
#include    <sys/types.h>
#include    <unistd.h>

#include    <ifaddrs.h>
#include    <net/if.h>
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

    /**
     * Constructs a new IpAddress.
     */
    IpAddress::IpAddress () :
        socket_ (0)
    {
        struct sockaddr_nl sa;

        memset (&sa, 0, sizeof (sa));
        sa.nl_family = AF_NETLINK;
        sa.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR;

        socket_ = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
        bind (socket_, (struct sockaddr *) &sa, sizeof (sa));

        pipe (pipe_);
    }

    IpAddress::~IpAddress ()
    {
    }

    IpAddress::get ()
    {
        std::wstring address;
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

            if ((iface->ifa_flags & IFF_UP) == 0) {
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

        if (address.empty ()) {
            address = L"Kein Netz erreichbar";
        }

        return address;
    }
}}
