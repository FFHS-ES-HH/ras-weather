/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * AsyncTimer.cpp is free software copyrighted by David Daniel.
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
#include    "app/AsyncTimer.hpp"

#include    <chrono>
#include    <tuple>

namespace piw { namespace app {

    namespace {
        typedef std::lock_guard<std::mutex> Lock;
        typedef std::unique_lock<std::mutex> ULock;
    }

    AsyncTimer::~AsyncTimer ()
    {
        try {
            ULock lock (mutex);

            isAlive = false;
            isRunning = false;
            lock.unlock ();

            condition.notify_one ();
            thread.join ();
        }
        catch (...) { /* ignored */ }
    }

    /**
     * AsyncTimer::shutdown
     */
    AsyncTimer& AsyncTimer::stop ()
    {
        Lock lock (mutex);

        if (isRunning) {
            isRunning = false;
            condition.notify_one ();
        }

        return *this;
    }

    bool AsyncTimer::running () const
    {
        Lock lock (mutex);
        return isRunning;
    }

    AsyncTimer& AsyncTimer::start (std::size_t msecs, bool onlyOnce)
    {
        if (running ()) {
            stop ();
        }

        Lock lock (mutex);

        this->msecs = msecs;
        this->onlyOnce = onlyOnce;
        isRunning = true;

        condition.notify_one ();

        return *this;
    }

    /**
     * AsyncTimer::run
     */
    void AsyncTimer::run ()
    {
        std::size_t ms = 0;
        bool stillAlive = false;

        {
            Lock lock (mutex);
            ms = msecs;
            stillAlive = isAlive;
        }

        while (stillAlive) {

            typedef std::chrono::system_clock Clock;
            typedef std::chrono::time_point<Clock> DateTime;

            DateTime start = Clock::now ();
            bool stillRunning = false;

            typedef std::cv_status Status;
            using std::chrono::milliseconds;

            Status status = Status::timeout;
            {
                ULock lock (mutex);

                stillRunning = isRunning;
                stillAlive = isAlive;

                if (isRunning) {
                    status = condition.wait_for (lock, milliseconds {ms});
                    stillRunning = isRunning;
                    stillAlive = isAlive;
                }
                else if (isAlive) {
                    condition.wait (lock);
                    stillRunning = isRunning;
                    stillAlive = isAlive;
                }
            }

            if (stillRunning) {

                if (status == Status::timeout) {

                    event ();

                    Lock lock (mutex);

                    stillAlive = isAlive;
                    ms = msecs;

                    if (onlyOnce) {
                        isRunning = stillRunning = false;
                    }
                    else {
                        stillRunning = isRunning;
                    }
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

