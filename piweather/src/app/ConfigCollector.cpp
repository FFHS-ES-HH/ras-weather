/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * ConfigCollector.cpp is free software copyrighted by David Daniel.
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
#include    "app/ConfigCollector.hpp"

#include    <fstream>
#include    <sstream>
#include    <stdexcept>
#include    <limits>

#include    <memory>

#include    <functional>
#include    <map>

#include    <array>
#include    <iterator>
#include    <iostream>

#include    <getopt.h>

namespace piw { namespace app {

    namespace {

        std::string& trim (std::string& value)
        {
            value.erase (0, value.find_first_not_of (' '));

            std::string::size_type pos = value.find_last_not_of (' ');
            if (pos != std::string::npos && ++pos < value.size ()) {
                value.erase (pos);
            }

            return value;
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

                std::string::size_type pos = value.find_first_of ('#');
                if (pos != std::string::npos) {
                    value.erase (pos);
                }

                trim (value);

                pos = value.find_first_of ('"');
                if (pos != std::string::npos) {
                    value.erase (0, ++pos);

                    pos = value.find_last_of ('"');
                    if (pos != std::string::npos) {
                        value.erase (pos);
                    }
                }
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
                { "database", ConfigEntry (config.dbPath) }
            };

            return map;
        }

        void handleParam (const ConfigMap& configMap, const std::string& name, std::istream& input)
        {
            ConfigMap::const_iterator entry = configMap.find (name);
            if (entry == configMap.end ()) {
                throw std::runtime_error ("The requested configuration entry does not exist.");
            }

            entry->second (input);
            input.clear ();
        }

        void usage ()
        {
            std::array<const char*, 7> lines {

                "Usage: piweather [-h] [-c {Configuration}] [-d {Database}] [-H {Host}] [-p {Port}]",
                "",
                "-c|--configuration {Configuration}     The path to the configuration file.",
                "-h|--help                              Print this help and exit.",
                "-p|--port {Port}                       Connect to port {Port}.",
                "-H|--host {Host}                       Connect to host {Host}.",
                "-d|--database {Database}               The path to the database to use."
            };

            std::copy (
                    lines.begin (),
                    lines.end (),
                    std::ostream_iterator<const char*> (
                        std::cout, "\n"));

            std::cout << std::endl;
        }
    }

    Configuration ConfigCollector::read (const std::string& path) const
    {
        std::ifstream ifs (path.c_str ());
        if (!ifs.is_open ()) {
            throw std::runtime_error ("Cannot open the given configuration file."); 
        }

        std::unique_ptr<std::ifstream, IfstreamCloser> input (&ifs);
        input->exceptions (std::ifstream::failbit | std::ifstream::badbit);

        Configuration config = Configuration ();
        ConfigMap configMap = getConfigMap (config);

        char eof = std::char_traits<char>::eof ();
        std::string name;
        constexpr std::streamsize ignoreCount = std::numeric_limits<std::streamsize>::max ();

        while (input->peek () != eof) {

            char next;
            switch ((next = input->get ())) {
                case '\n':
                case ' ':
                    break;
                case '#':
                    input->ignore (ignoreCount, '\n');
                    break;
                case '=':
                    input->ignore (ignoreCount, ' ');
                    {
                        ConfigMap::iterator value = configMap.find (trim (name));
                        if (value != configMap.end ()) {
                            value->second (*input);
                        }
                        name.clear ();
                    }
                    break;
                default:
                    name += next;
            }
        }

        return config;
    }

    ConfigCollector::UserAction ConfigCollector::collect (Configuration& config, int argc, char** argv) const
    {
        const option options [] = {
            { "configuration", 1, 0, 'c' },
            { "host", 1, 0, 'H' },
            { "port", 1, 0, 'p' },
            { "database", 1, 0, 'd' },
            { "help", 1, 0, 'h' },
            { 0, 0, 0, 0 },
        };

        int index;
        int current;
        std::istringstream input;
        std::string configFile;
        UserAction action = Run;
        ConfigMap configMap = getConfigMap (config);

        while ((current = getopt_long (argc, argv, "c:H:p:d:h", options, &index)) != -1) {
            switch (current) {
                case 'c':
                    configFile = optarg;
                    break;
                case 'H':
                    input.str (optarg);
                    handleParam (configMap, "host", input);
                    break;
                case 'p':
                    input.str (optarg);
                    handleParam (configMap, "port", input);
                    break;
                case 'd':
                    input.str (optarg);
                    handleParam (configMap, "database", input);
                    break;
                case 'h':
                    action = ShowUsage;
                    usage ();
                    break;
                default:
                    action = Unknown;
                    break;
            }
        }

        if (action == Run) {
            const char* path = 0;
            if (!configFile.empty ()) {
                path = configFile.c_str ();
            }
#ifdef      DEFAULT_CONFIG_PATH
            else {
                path = DEFAULT_CONFIG_PATH;
            }
#endif

            if (path != 0) {
                Configuration fromFile = read (path);
                config.merge (fromFile);
            }
        }

        return action;
    }
}}

