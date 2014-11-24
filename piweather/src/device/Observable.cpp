/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Observable.cpp is free software copyrighted by David Daniel.
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
#include    <device/Observable.hpp>
#include    <device/Observer.hpp>

#include    <algorithm>

namespace piw { namespace device {

    namespace {
        typedef std::vector<Observer*> Observers;

        inline Observers::iterator atObserver (
                Observers& observers,
                const Observer* which)
        {
            return std::find (observers.begin (), observers.end (), which);
        }
    }

    void Observable::addObserver (Observer& observer)
    {
        if (atObserver (observers_, &observer) == observers_.end ()) {
            observers_.push_back (&observer);
        }
    }

    void Observable::removeObserver (const Observer& observer)
    {
        Observers::iterator existing = atObserver (observers_, &observer);

        if (existing != observers_.end ()) {
            observers_.erase (existing);
        }
    }

    void Observable::notifyObservers ()
    {
        for (Observers::value_type o : observers_) {
            try {
                o->valueChanged ();
            }
            catch (const std::exception&) { /* ignored */ }
        }
    }
}}

