/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * SignalHandler.cpp is free software copyrighted by David Daniel.
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
#include    "app/SignalHandler.hpp"

#include    <chrono>

namespace piw { namespace app {

    SignalHandler::SignalHandler () :
        mutex (),
        condition (),
        signal (0),
        triggered (false)
    {
        struct sigaction handler;
        handler.sa_sigaction = SignalHandler::handle;
        handler.sa_flags = SA_SIGINFO;
        sigemptyset (&handler.sa_mask);

        sigaction (SIGTERM, &handler, nullptr);
        sigaction (SIGINT, &handler, nullptr);
    }

    void SignalHandler::handle (int n, siginfo_t*, void*)
    {
        SignalHandler& self = instance ();

        self.signal.store (n);
        self.triggered.store (true);
        self.condition.notify_all ();
    }

    SignalHandler& SignalHandler::instance ()
    {
        static SignalHandler handler;

        return handler;
    }

    int SignalHandler::wait (std::size_t milliseconds)
    {
        SignalHandler& self = SignalHandler::instance ();
        bool running = true;
        int result = 0;

        while (running) {

            std::unique_lock<std::mutex> lock (self.mutex);

            std::cv_status waitResult = std::cv_status::no_timeout;

            if (milliseconds > 0) {
                waitResult = self.condition.wait_for (
                        lock, std::chrono::milliseconds {milliseconds});
            }
            else {
                self.condition.wait (lock);
            }

            if (self.triggered.load ()) {

                result = self.signal.load ();

                self.triggered.store (running = false);
            }
            else if (waitResult == std::cv_status::timeout) {
                running = false;
            }
        }

        return result;
    }
}}
