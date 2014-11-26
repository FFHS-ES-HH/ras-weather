/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * ConfigReader.cpp is free software copyrighted by David Daniel.
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
#include    "app/ConfigReader.hpp"

#include    <fstream>
#include    <memory>
#include    <stdexcept>
#include    <functional>

namespace piw { namespace app {

    namespace {
        struct IfstreamCloser
        {
            void operator() (std::ifstream*) const;
        };

        void IfstreamCloser::operator() (std::ifstream* input) const
        {
            input->exceptions (std::ifstream::goodbit);
            input->close ();
        }

        template<typename T>
            struct ConfigSetter
            {
                typedef Configuration& (Configuration::*type) (const T&);

                static void set (const T&, type, Configuration&);
            };

        template<typename T>
            inline void ConfigSetter<T>::set (
                    const T& value,
                    ConfigSetter<T>::type setter,
                    Configuration& config)
            {
                (config.*setter) (value);
            }

        template<typename T>
             void setConfigValue (
                     std::istream& input,
                     Configuration& config,
                     typename ConfigSetter<T>::type setter)
            {
                T value;
                input >> value;
                ConfigSetter<T>::set (value, setter, config);
            }

        class ConfigEntry
        {
            public:
                template<typename T>
                    ConfigEntry (typename ConfigSetter<T>::type);

                void operator() (std::istream&, Configuration&) const;

            private:
                std::function<void (std::istream&, Configuration&)> setter;
        };

        using namespace std::placeholders;

        template<typename T>
            ConfigEntry::ConfigEntry (typename ConfigSetter<T>::type s) :
                setter (std::bind (&setConfigValue<T>, _1, _2, s))
            {}

        void ConfigEntry::operator() (std::istream& input, Configuration& config) const
        { setter (input, config); }
    }

    void ConfigReader::read (Configuration& config)
    {
        std::ifstream ifs (path.c_str ());
        if (!ifs.is_open ()) {
            throw std::runtime_error ("Cannot open the given configuration file."); 
        }

        std::unique_ptr<std::ifstream, IfstreamCloser> input (&ifs);

        input->exceptions (std::ifstream::failbit | std::ifstream::badbit);
    }
}}

