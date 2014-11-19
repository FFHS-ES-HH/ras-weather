/*
 * LICENSE
 *
 * Copyright (c) 2014, David Daniel (dd), david@daniels.li
 *
 * Database.cpp is free software copyrighted by David Daniel.
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
#include    "db/Database.hpp"

#include    <stdexcept>
#include    <chrono>
#include    <memory>

#include    <sqlite3.h>

namespace piw { namespace db {

    namespace {

        template<typename T>
            void bind_value (T value, int index, sqlite3_stmt* stmt)
            {
                if (sqlite3_bind_text (stmt, index, value.c_str (), value.size (), 0) != SQLITE_OK) {
                    throw std::exception ();
                }
            }

        template<>
            void bind_value<double> (double value, int index, sqlite3_stmt* stmt)
            {
                if (sqlite3_bind_double (stmt, index, value) != SQLITE_OK) {
                    throw std::exception ();
                }
            }

        struct Sql
        {
            static const std::string insert;
        };

        const std::string Sql::insert {
            "insert into piwvalues ("
                "temperature, "
                "humidity, "
                "pressure, "
                "illumination, "
                "date"
                ") values (?, ?, ?, ?, ?);"
        };

        void bind_values (const Values& values, sqlite3_stmt* stmt)
        {

            int i {0};
            bind_value (values.temperature, ++i, stmt);
            bind_value (values.humidity, ++i, stmt);
            bind_value (values.pressure, ++i, stmt);
            bind_value (values.illumination, ++i, stmt);

            // TODO
            // BIND_STRING (++i, date, stmt)
        }

        struct StatementCloser
        {
            void operator() (sqlite3_stmt*) const;
        };

        inline void StatementCloser::operator() (sqlite3_stmt* stmt) const
        {
            sqlite3_finalize (stmt);
        }
    }

    /**
     * Constructs a new Database.
     * @param path the path to the database
     */
    Database::Database (const std::string& path) :
        handle_ (0)
    {
        if (sqlite3_open (path.c_str (), &handle_) != SQLITE_OK) {
            throw std::runtime_error ("Cannot open the database.");
        }
    }

    /**
     * Closes the database.
     */
    Database::~Database ()
    {
        sqlite3_close (handle_);
    }

    /**
     * Stores the given values.
     * @param values the values to store
     * @return  this database
     */
    Database& Database::storeValues (const Values& values)
    {
        std::string sql;
        sqlite3_stmt* stmt (0);

        int result = sqlite3_prepare (
                handle_, Sql::insert.c_str (), Sql::insert.size (), &stmt, 0);

        if (result != SQLITE_OK) {
            throw std::runtime_error ("Cannot prepare to insert values.");
        }

        std::unique_ptr<sqlite3_stmt, StatementCloser> statement (stmt);

        try {

            bind_values (values, statement.get ());

            if (sqlite3_step (statement.get ()) != SQLITE_OK) {
                throw std::exception ();
            }
        }
        catch (const std::exception&) {
            throw std::runtime_error ("Error on inserting values.");
        }

        return *this;
    }
}}

