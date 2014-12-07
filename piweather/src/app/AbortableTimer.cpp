/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * AbortableTimer.cpp is free software copyrighted by David Daniel.
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
#include    "app/AbortableTimer.hpp"

#include    <chrono>

namespace piw { namespace app {

    namespace {
        typedef std::unique_lock<std::mutex> Lock;
    }

    /**
     * AbortableTimer::cancel
     */
    void AbortableTimer::cancel ()
    {
        Lock lock (mutex);

        if (running.load ()) {
            running.store (false);
            condition.notify_one ();
            thread.join ();
        }
    }

    /**
     * AbortableTimer::reschedule
     */
    void AbortableTimer::reschedule (std::size_t ms)
    {
        Lock lock (mutex);

        if (! running.load ()) {
            running.store (true);
            thread = std::thread {std::bind (&AbortableTimer::run, this)};
        }
    }

    /**
     * AbortableTimer::run
     * @return  
     */
    void AbortableTimer::run (bool once, std::size_t timeout)
    {
        std::size_t ms = timeout;

        while (running.load ()) {

            typedef std::cv_status Status;
            typedef std::time_point<std::chrono::system_clock> DateTime;
            typedef std::chrono::system_clock Clock;

            using std::chrono::milliseconds;

            DateTime start;

            Status status;
            {
                Lock lock (mutex);
                start = Clock::now ();
                status = condition.wait_for (lock, milliseconds (ms));
            }

            if (running.load ()) {

                if (status == Status::timeout) {
                    event ();
                    ms = msecs;
                }
                else {
                    ms -= std::chrono::duration_cast<milliseconds> (
                            Clock::now () - start)
                        .count ();
                }
            }
        }
    }
}}

