/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * AsyncTimer.hpp is free software copyrighted by David Daniel.
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
#include    <stdexcept>

namespace piw { namespace app {

    class AsyncTimer
    {
        public:
            template<typename Func>
                AsyncTimer (Func&&);

            AsyncTimer () = delete;
            ~AsyncTimer ();

            AsyncTimer (const AsyncTimer&) = delete;
            AsyncTimer& operator= (const AsyncTimer&) = delete;

            template<typename Rep, typename Period>
                AsyncTimer& start (const std::chrono::duration<Rep, Period>&, bool = false);

            AsyncTimer& start (std::size_t, bool = false);
            AsyncTimer& stop ();

            bool running () const;

        private:
            void run ();

        private:
            bool isRunning;
            bool isAlive;
            bool onlyOnce;
            std::size_t msecs;
            std::function<void ()> event;
            mutable std::mutex mutex;
            std::condition_variable condition;
            std::thread thread;
    };

    template<typename Func>
        AsyncTimer::AsyncTimer (Func&& fn) :
            isRunning {false},
            isAlive {true},
            onlyOnce {false},
            msecs {0},
            event {std::forward<Func> (fn)},
            mutex {},
            condition {},
            thread {&AsyncTimer::run, this}
    {}

    template<typename Rep, typename Period>
        AsyncTimer& AsyncTimer::start (const std::chrono::duration<Rep, Period>& d, bool once)
        {
            return start (
                    std::chrono::duration_cast<std::chrono::milliseconds> (d).count (),
                    once);
        }
}}

#endif /* PIW_APP_ABORTABLETIMER_INC */

