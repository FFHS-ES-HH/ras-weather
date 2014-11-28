/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * ThresholdObservable.hpp is free software copyrighted by David Daniel.
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
#ifndef PIW_SENSORS_THRESHOLDABLE_INC
#define PIW_SENSORS_THRESHOLDABLE_INC

#include    <stdexcept>
#include    <cstdint>

#include    <device/Observable.hpp>

namespace piw { namespace sensors {

    template<typename T>
        class ThresholdObservable : public device::Observable
    {
        public:
            ThresholdObservable (T);
            virtual ~ThresholdObservable () {}

            virtual void threshold (T);
            virtual T threshold () const;
            virtual T value () const;

        protected:
            void init ();
            void value (T);
            void triggerAdjust ();

            static void* wrapper ();
            static void wrap (T, void*);

        protected:
            virtual T read () = 0;
            virtual void adjust (T min, T max) = 0;
            virtual void valueChanged (T) = 0;

        private:
            T threshold_;
            T value_;
    };

    /**
     * Constructs a new ThresholdObservable.
     */
    template<typename T>
        inline ThresholdObservable<T>::ThresholdObservable (T threshold) :
            threshold_ (threshold)
    {}

    template<typename T>
        inline T ThresholdObservable<T>::threshold () const
        { return threshold_; }

    template<typename T>
        inline T ThresholdObservable<T>::value () const
        { return value_; }

    template<typename T>
        inline void ThresholdObservable<T>::value (T value)
        { value_ = value; }

    template<typename T>
        void ThresholdObservable<T>::wrap (T value, void* user_data)
        {
            ThresholdObservable* derived = static_cast<ThresholdObservable*> (user_data);

            try {
                derived->valueChanged (value);
            }
            catch (const std::exception&) { /* ignored */ }
        }

    template<typename T>
        inline void* ThresholdObservable<T>::wrapper ()
        {
            return reinterpret_cast<void*> (&ThresholdObservable<T>::wrap);
        }

    template<typename T>
        void ThresholdObservable<T>::init ()
        {
            value (read ());
            triggerAdjust ();
        }

    template<typename T>
        void ThresholdObservable<T>::triggerAdjust ()
        {
            T current {value ()};
            T thresh {threshold ()};
            adjust (current - thresh, current + thresh);
        }

    template<typename T>
        void ThresholdObservable<T>::threshold (T threshold)
        {
            threshold_ = threshold;
            triggerAdjust ();
        }
}}

#endif /* PIW_SENSORS_THRESHOLDABLE_INC */

