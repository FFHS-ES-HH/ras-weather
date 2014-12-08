/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Configuration.cpp is free software copyrighted by David Daniel.
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
#include    "app/Configuration.hpp"

#include    <stdexcept>

#ifdef      CONFIG_H
#include    <config.h>
#endif

namespace piw { namespace app {

    namespace {

            struct UnaryBool
            {
                template<typename T>
                    static bool value (T&&);

                template<typename Rep, typename Period>
                    static bool value (const std::chrono::duration<Rep, Period>&);

                template<typename Rep, typename Period>
                    static bool value (std::chrono::duration<Rep, Period>&);
            };

        template<typename T>
            bool UnaryBool::value (T&& v)
            { return v; }

        template<>
            bool UnaryBool::value (const std::string& v)
            { return !v.empty (); }

        template<>
            bool UnaryBool::value (std::string& v)
            { return !v.empty (); }

        template<>
            bool UnaryBool::value (const std::chrono::seconds& v)
            { return v != std::chrono::seconds::zero (); }

        template<>
            bool UnaryBool::value (std::chrono::seconds& v)
            { return v != std::chrono::seconds::zero (); }

        template<typename Rep, typename Period>
            bool UnaryBool::value (const std::chrono::duration<Rep, Period>& v)
            { return v != std::chrono::duration<Rep, Period>::zero (); }

        template<typename Rep, typename Period>
            bool UnaryBool::value (std::chrono::duration<Rep, Period>& v)
            { return value (const_cast<const std::chrono::duration<Rep, Period>&> (v)); }

        template<typename T>
            void mergeValue (T& first, const T& second, bool overwrite)
            {
                bool f = UnaryBool::value (first);
                bool s = UnaryBool::value (second);

                if (!f || (overwrite && s)) {
                    first = second;
                }
            }
    }

    /**
     * Merges this configuration with the given one.
     * @param other the configuration to merge with
     * @param overwrite whether the values from other should overwrite existing
     *                  values from this configuration
     * @return this configuration
     */
    Configuration& Configuration::merge (const Configuration& other, bool overwrite)
    {
        mergeValue (pollInterval, other.pollInterval, overwrite);
        mergeValue (temperatureSensitivity, other.barometerSensitivity, overwrite);
        mergeValue (barometerSensitivity, other.barometerSensitivity, overwrite);
        mergeValue (humiditySensitivity, other.humiditySensitivity, overwrite);
        mergeValue (illuminanceSensitivity, other.illuminanceSensitivity, overwrite);
        mergeValue (host, other.host, overwrite);
        mergeValue (port, other.port, overwrite);
        mergeValue (button, other.button, overwrite);
        mergeValue (dbPath, other.dbPath, overwrite);
        mergeValue (saveInterval, other.saveInterval, overwrite);
        mergeValue (lcdTimeout, other.lcdTimeout, overwrite);

        return *this;
    }
}}

