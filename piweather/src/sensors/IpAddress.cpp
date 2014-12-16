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
#include    <sensors/IpAddress.hpp>

#include    <stdexcept>
#include    <algorithm>
#include    <thread>
#include    <cstring>

#include    <sys/socket.h>
#include    <linux/rtnetlink.h>
#include    <sys/select.h>
#include    <unistd.h>

#include    <ifaddrs.h>
#include    <net/if.h>
#include    <netdb.h>

namespace piw { namespace sensors {

    using sensors::SensorDevice;

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

        class IfListener
        {
            struct SocketCloser
            {
                SocketCloser (int, int);
                ~SocketCloser ();

                int quitPipe;
                int sock;
            };

            public:
                typedef void (IpAddress::*Callback) ();

            public:
                IfListener (int, Callback, IpAddress&);

                void operator() ();

            private:
                int quitPipe_;
                int socket_;
                Callback callback_;
                IpAddress& ip_;
        };

        IfListener::SocketCloser::SocketCloser (int quitP, int s) :
            quitPipe (quitP),
            sock (s)
        {}

        IfListener::SocketCloser::~SocketCloser ()
        {
            close (quitPipe);
            close (sock);
        }

        IfListener::IfListener (int quitPipe, Callback c, IpAddress& i) :
            quitPipe_ (quitPipe),
            socket_ (0),
            callback_ (c),
            ip_ (i)
        {
            struct sockaddr_nl sa;

            memset (&sa, 0, sizeof sa);
            sa.nl_family = AF_NETLINK;
            sa.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR;

            socket_ = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

            if (socket_ == -1) {
                throw std::runtime_error {"Cannot open a netlink socket."};
            }

            if (bind (socket_, (struct sockaddr *) &sa, sizeof sa) == -1) {
                close (socket_);
                throw std::runtime_error {"Cannot bind to the netlink socket."};
            }
        }

        void IfListener::operator () ()
        {
            bool running = true;
            SocketCloser c {quitPipe_, socket_};

            while (running) {

                fd_set fds;
                FD_ZERO (&fds);
                FD_SET (quitPipe_, &fds);
                FD_SET (socket_, &fds);

                int maxfd = std::max (quitPipe_, socket_) + 1;

                if (select (maxfd, &fds, nullptr, nullptr, nullptr) == -1) {
                    throw std::runtime_error {"Cannot listen on the read pipe and the socket."};
                }

                if (FD_ISSET (quitPipe_, &fds)) {
                    running = false;
                }
                else if (FD_ISSET (socket_, &fds)) {

                    char buf [4096];
                    struct iovec iov = { buf, sizeof buf };
                    struct sockaddr_nl sa;
                    struct msghdr msg;

                    msg = { &sa, sizeof sa, &iov, 1, nullptr, 0, 0 };
                    // empty queue
                    recvmsg (socket_, &msg, 0);

                    (ip_.*callback_) ();
                }
            }
        }
    }

    IpAddress::IpAddress () :
        quitPipe_ (0)
    {
        int pipeHandles [2];

        if (pipe (pipeHandles)) {
            throw std::runtime_error {"Cannot create a pipe."};
        }

        quitPipe_ = pipeHandles [1];

        std::thread thread {
            IfListener {pipeHandles [0], &IpAddress::notifyObservers, *this}};
        thread.detach ();
    }

    IpAddress::~IpAddress ()
    {
        ssize_t written = write (quitPipe_, &quitPipe_, sizeof &quitPipe_);
        (void) written;
        close (quitPipe_);
    }

    SensorDevice::Status IpAddress::status ()
    { return SensorDevice::Available; }

    std::wstring IpAddress::get () const
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
