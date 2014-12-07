/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * AbortableTimer.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_APP_ABORTABLETIMER_INC
#define PIW_APP_ABORTABLETIMER_INC

#include    <cstddef>
#include    <functional>
#include    <thread>
#include    <mutex>
#include    <condition_variable>
#include    <atomic>

namespace piw { namespace app {

    class AbortableTimer
    {
        public:
            AbortableTimer ();
            ~AbortableTimer ();

            template<typename T>
                AbortableTimer& start (T, std::size_t);

            template<typename T>
                AbortableTimer& startOnce (T, std::size_t);

            AbortableTimer& shutdown ();
            bool running () const;

        private:
            static std::thread createThread (const std::function<void ()>&);
            void run ();

        private:
            std::function<void ()> event;
            std::atomic_bool running;
            std::thread thread;
            std::mutex mutex;
            std::condition_variable condition;
    };

    /**
     * Constructs a new AbortableTimer.
     */
    template<typename T>
        AbortableTimer::AbortableTimer () : 
            event {},
            running {false},
            msecs {0},
            thread {},
            mutex {},
            condition {}
    {}

    /**
     * Constructs a new AbortableTimer.
     */
        AbortableTimer::AbortableTimer () :
            event {},
            running {false},
            msecs {0},
            thread {},
            mutex {},
            condition {}
    {}

    inline AbortableTimer::operator bool () const
    { return event; }
}}

#endif /* PIW_APP_ABORTABLETIMER_INC */

