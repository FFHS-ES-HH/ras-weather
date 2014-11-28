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
#include    <map>

namespace piw { namespace app {

    namespace {

        void trim (std::string& value) {
            value.erase (0, value.find_first_not_of (' '));

            std::string::size_type pos = value.find_last_not_of (' ');
            if (pos < value.size ()) {
                value.erase (pos);
            }
        }

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
            void readValue (std::istream& input, T& value)
            {
                input >> value;
            }

        template<>
            void readValue<std::string> (std::istream& input, std::string& value)
            {
                value.clear ();
                std::getline (input, value, '\n');

                value.erase (value.find_first_of ('#'));
                trim (value);
            }

        class ConfigEntry
        {
            public:
                template<typename T>
                    ConfigEntry (T&);

                void operator() (std::istream&) const;

            private:
                std::function<void (std::istream&)> setter;
        };

        using namespace std::placeholders;

        template<typename T>
            ConfigEntry::ConfigEntry (T& value) :
                setter (std::bind (&readValue<T>, _1, std::ref (value)))
            {}

        void ConfigEntry::operator() (std::istream& input) const
        { setter (input); }

        typedef std::map<std::string, ConfigEntry> ConfigMap;

        ConfigMap getConfigMap (Configuration& config)
        {
            ConfigMap map {
                { "host", ConfigEntry (config.host) },
                { "port", ConfigEntry (config.port) },
                { "barometer-sensitivity", ConfigEntry (config.barometerSensitivity) },
                { "humidity-sensitivity", ConfigEntry (config.humiditySensitivity) },
                { "poll-interval", ConfigEntry (config.pollInterval) },
                { "temperature-sensitivity", ConfigEntry (config.temperatureSensitivity) },
                { "illuminance-sensitivity", ConfigEntry (config.illuminanceSensitivity) },
                { "button", ConfigEntry (config.button) },
                { "db-path", ConfigEntry (config.dbPath) }
            };

            return map;
        }
    }

    void ConfigReader::read (Configuration& config, const std::string& path)
    {
        std::ifstream ifs (path.c_str ());
        if (!ifs.is_open ()) {
            throw std::runtime_error ("Cannot open the given configuration file."); 
        }

        std::unique_ptr<std::ifstream, IfstreamCloser> input (&ifs);
        input->exceptions (std::ifstream::failbit | std::ifstream::badbit);

        ConfigMap configMap = getConfigMap (config);

        char eof = std::char_traits<char>::eof ();
        while (input->peek () != eof) {
            std::string name;
            bool atName = false;

            char next = input->get ();
            switch (next) {
                case '\n':
                    input->ignore (' ');
                    atName = input->peek () != '#';
                    break;
                case ' ':
                    input->ignore (' ');
                    break;
                case '#':
                    input->ignore ('\n');
                    atName = input->peek () != '#';
                    break;
                case '=':
                    {
                        atName = false;
                        ConfigMap::iterator value = configMap.find (name);
                        if (value != configMap.end ()) {
                            value->second (*input);
                        }
                        name.clear ();
                    }
                    break;
                default:
                    if (atName) {
                        name += next;
                    }
            }
        }
    }

    void ConfigReader::parse (Configuration& config, int argc, char** argv)
    {
        option options [] = {
            { "host", 1, 0, 'h' },

                // { "port", ConfigEntry (config.port) },
                // { "barometer-sensitivity", ConfigEntry (config.barometerSensitivity) },
                // { "humidity-sensitivity", ConfigEntry (config.humiditySensitivity) },
                // { "poll-interval", ConfigEntry (config.pollInterval) },
                // { "temperature-sensitivity", ConfigEntry (config.temperatureSensitivity) },
                // { "illuminance-sensitivity", ConfigEntry (config.illuminanceSensitivity) },
                // { "button", ConfigEntry (config.button) },
                // { "db-path", ConfigEntry (config.dbPath) }
        };

        return {+return_value+};
    }
}}

