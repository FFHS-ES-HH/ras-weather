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

        typedef std::chrono::system_clock::time_point DateTime;

        namespace chrono = std::chrono;

        struct Sql
        {
            static const std::string insert;
        };

        template<typename T>
            void bind_value (T value, int index, sqlite3_stmt* stmt)
            {
                if (sqlite3_bind_int (stmt, index, value) != SQLITE_OK) {
                    throw std::runtime_error ("Cannot bind the given value.");
                }
            }

        template<>
            void bind_value<double> (double value, int index, sqlite3_stmt* stmt)
            {
                if (sqlite3_bind_double (stmt, index, value) != SQLITE_OK) {
                    throw std::runtime_error ("Cannot bind the given double value.");
                }
            }

        template<typename T>
            void bind_reference (const T& value, int index, sqlite3_stmt* stmt)
            {
                if (sqlite3_bind_text (stmt, index, value.c_str (), value.size (), 0) != SQLITE_OK) {
                    throw std::runtime_error ("Cannot bind the given string.");
                }
            }

        template<>
            void bind_reference<DateTime> (
                    const DateTime& value,
                    int index,
                    sqlite3_stmt* stmt)
            {
                std::uint64_t seconds = chrono::duration_cast<chrono::seconds> (
                        value.time_since_epoch ())
                    .count ();

                if (sqlite3_bind_int64 (stmt, index, seconds) != SQLITE_OK) {
                    throw std::runtime_error ("Cannot bind the given date.");
                }
            }

        struct StatementCloser
        {
            void operator() (sqlite3_stmt*) const;
        };

        inline void StatementCloser::operator() (sqlite3_stmt* stmt) const
        {
            sqlite3_finalize (stmt);
        }

        const std::string Sql::insert {

            "INSERT INTO piwvalues ("

                "temperature, humidity, pressure, illuminance, created_at"

                ") VALUES (?, ?, ?, ?, ?)"
        };
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
        if (sqlite3_exec (handle_, "BEGIN TRANSACTION", 0, 0, 0) != SQLITE_OK) {
            throw std::runtime_error ("Cannot begin a transaction.");
        }

        try {
            sqlite3_stmt* stmt {0};

            int result = sqlite3_prepare_v2 (
                    handle_, Sql::insert.c_str (), Sql::insert.size (), &stmt, 0);

            if (result != SQLITE_OK) {
                throw std::runtime_error ("Cannot prepare to insert values.");
            }

            std::unique_ptr<sqlite3_stmt, StatementCloser> statement {stmt};

            int i {0};
            bind_value (values.temperature, ++i, stmt);
            bind_value (values.humidity, ++i, stmt);
            bind_value (values.pressure, ++i, stmt);
            bind_value (values.illuminance, ++i, stmt);
            bind_reference (values.created_at, ++i, stmt);

            if (sqlite3_step (statement.get ()) != SQLITE_OK) {
                throw std::runtime_error ("Error on inserting values.");
            }

            sqlite3_exec (handle_, "COMMIT", 0, 0, 0);
        }
        catch (const std::exception&) {
            sqlite3_exec (handle_, "ROLLBACK", 0, 0, 0);
            throw;
        }

        return *this;
    }
}}

