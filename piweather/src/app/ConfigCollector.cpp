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
#include    <chrono>

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

        template<typename T>
            void readValue (std::istream& input, T& value)
            {
                input >> value;
            }

        template<>
            void readValue<std::string> (std::istream& input, std::string& value)
            {
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

        template<typename Rep, typename Period>
            void readValue (std::istream& input, std::chrono::duration<Rep, Period>& value)
            {
                Rep count;
                input >> count;
                value = std::chrono::duration<Rep, Period> {count};
            }

        class Param
        {
            public:
                template<typename T>
                    Param (T&);

                template<typename T, typename O, template<typename, typename> class Compound>
                    Param (Compound<T, O>&);

                void operator() (std::istream&) const;

            private:
                std::function<void (std::istream&)> setter;
        };

        using namespace std::placeholders;

        template<typename T>
            Param::Param (T& value) :
                setter {std::bind (&readValue<T>, _1, std::ref (value))}
            {}

        template<typename T, typename V, template<typename, typename> class Compound>
            Param::Param (Compound<T, V>& value) :
                setter {std::bind (&readValue<T, V>, _1, std::ref (value))}
            {}

        void Param::operator() (std::istream& input) const
        { setter (input); }

        typedef std::map<std::string, Param> Params;

        Params getConfigParams (Configuration& config)
        {
            Params params {
                { "host", Param (config.host) },
                { "port", Param (config.port) },
                { "barometer-sensitivity", Param (config.barometerSensitivity) },
                { "humidity-sensitivity", Param (config.humiditySensitivity) },
                { "poll-interval", Param (config.pollInterval) },
                { "temperature-sensitivity", Param (config.temperatureSensitivity) },
                { "illuminance-sensitivity", Param (config.illuminanceSensitivity) },
                { "button", Param (config.button) },
                { "database", Param (config.dbPath) },
                { "store-interval", Param (config.saveInterval) },
                { "lcd-timeout", Param (config.lcdTimeout) }
            };

            return params;
        }

        void handleParam (const Params& params, const std::string& name, std::istream& input)
        {
            Params::const_iterator entry = params.find (name);
            if (entry == params.end ()) {
                throw std::runtime_error ("The requested configuration entry does not exist.");
            }

            entry->second (input);
            input.clear ();
        }

        void usage ()
        {
            const char* const lines [] = {

                "Usage: piweather [-h] [-c {Configuration}] [-d {Database}] [-H {Host}] [-p {Port}]",
                "",
                "-c|--configuration {Configuration}     The path to the configuration file.",
                "-h|--help                              Print this help and exit.",
                "-p|--port {Port}                       Connect to port {Port}.",
                "-H|--host {Host}                       Connect to host {Host}.",
                "-d|--database {Database}               The path to the database to use."
            };

            std::copy (
                    lines,
                    lines + (sizeof lines / sizeof lines [0]),
                    std::ostream_iterator<const char*> (std::cout, "\n"));

            std::cout << std::endl;
        }
    }

    Configuration ConfigCollector::read (const std::string& path) const
    {
        std::ifstream input (path.c_str ());
        if (!input.is_open ()) {
            throw std::runtime_error ("Cannot open the given configuration file."); 
        }

        input.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        Configuration config = Configuration ();
        Params params = getConfigParams (config);

        constexpr char Eof = std::char_traits<char>::eof ();
        constexpr std::streamsize MaximumSize = std::numeric_limits<std::streamsize>::max ();
        std::string name;

        while (input.peek () != Eof) {

            char next = input.get ();
            switch (next) {
                case '\n':
                case '\r':
                case '\t':
                case '\v':
                case '\f':
                case '\a':
                case '\b':
                case ' ':
                    break;
                case '#':
                    input.ignore (MaximumSize, '\n');
                    break;
                case '=':
                    input.ignore (MaximumSize, ' ');
                    {
                        Params::iterator atParam = params.find (trim (name));
                        if (atParam != params.end ()) {
                            atParam->second (input);
                        }
                        name.clear ();
                    }
                    break;
                default:
                    name += next;
            }
        }

        input.clear ();
        input.close ();

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
        Params params = getConfigParams (config);

        while ((current = getopt_long (argc, argv, "c:H:p:d:h", options, &index)) != -1) {
            switch (current) {
                case 'c':
                    configFile = optarg;
                    break;
                case 'H':
                    input.str (optarg);
                    handleParam (params, "host", input);
                    break;
                case 'p':
                    input.str (optarg);
                    handleParam (params, "port", input);
                    break;
                case 'd':
                    input.str (optarg);
                    handleParam (params, "database", input);
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
                config.merge (read (path));
            }
        }

        return action;
    }
}}

