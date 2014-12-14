/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * SignalHandler.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_APP_SIGNALHANDLER_INC
#define PIW_APP_SIGNALHANDLER_INC

#include    <cstdint>
#include    <mutex>
#include    <condition_variable>
#include    <atomic>

#include    <signal.h>

namespace piw { namespace app {

    class SignalHandler
    {
        public:
            static int wait (std::size_t);

        private:
            SignalHandler ();

            static SignalHandler& instance ();
            static void handle (int, siginfo_t*, void*);

        private:
            std::mutex mutex;
            std::condition_variable condition;
            std::atomic_int signal;
            std::atomic_bool triggered;
    };
}}

#endif /* PIW_APP_SIGNALHANDLER_INC */

