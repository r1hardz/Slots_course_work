#include "Database.h"
#include <iostream>

using namespace std;

Database::Database(const std::string& dbPath) : db(nullptr)
{
    // try to open or create database
    bool opened_ok = (sqlite3_open(dbPath.c_str(), &db) == SQLITE_OK);

    if (!opened_ok) {
        cout << "Error opening database: " << sqlite3_errmsg(db) << endl;
        db = nullptr;
    }
    else {
        cout << "Database opened successfully" << endl;
        createTables();

        if (db) {
            // set database timeout to 2 seconds
            sqlite3_busy_timeout(db, 2000);

            char* error_message = nullptr;
            string sql = "PRAGMA journal_mode=WAL;";

            if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &error_message) != SQLITE_OK) {
                cout << "Error setting WAL mode: " << error_message << endl;
                sqlite3_free(error_message);
            }
        }
    }
}

Database::~Database()
{
    close();
}

bool Database::open()
{
    // try to open the database file
    bool opened_ok = (sqlite3_open("user_database.db", &db) == SQLITE_OK);

    if (!opened_ok) {
        cout << "Failed to open database: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    return true;
}

void Database::close()
{
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

void Database::createTables()
{
    // SQL command to create users table if it doesn't exist
    string sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT NOT NULL UNIQUE, "
        "password TEXT NOT NULL, "
        "credits INTEGER DEFAULT 100);";

    char* error_message = nullptr;
    bool created_ok = (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &error_message) == SQLITE_OK);

    if (!created_ok) {
        cout << "Error creating tables: " << error_message << endl;
        sqlite3_free(error_message);
    }
    else {
        cout << "Tables created successfully" << endl;
    }
}

bool Database::addUser(const string& username, const string& password)
{
    // SQL command to add new user with username and password
    string sql = "INSERT INTO users (username, password) VALUES (?, ?);";

    // prepare the database for new command
    sqlite3_stmt* prepared_sql;
    bool prepared_ok = (sqlite3_prepare_v2(db, sql.c_str(), -1, &prepared_sql, nullptr) == SQLITE_OK);

    // if preparation failed return
    if (!prepared_ok) {
        cout << "Error preparing database: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    // add username and password to command
    sqlite3_bind_text(prepared_sql, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(prepared_sql, 2, password.c_str(), -1, SQLITE_STATIC);

    // try to create new user
    bool user_added = (sqlite3_step(prepared_sql) == SQLITE_DONE);

    // return result
    sqlite3_finalize(prepared_sql);
    return user_added;
}

bool Database::loginUser(const string& username, const string& password)
{
    // SQL command to get password for username
    string sql = "SELECT password FROM users WHERE username = ?;";

    // prepare database for command
    sqlite3_stmt* prepared_sql;
    bool prepared_ok = (sqlite3_prepare_v2(db, sql.c_str(), -1, &prepared_sql, nullptr) == SQLITE_OK);
    bool login_ok = false;

    if (prepared_ok) {
        // Add username to command
        sqlite3_bind_text(prepared_sql, 1, username.c_str(), -1, SQLITE_STATIC);

        // check if user exists and password matches
        if (sqlite3_step(prepared_sql) == SQLITE_ROW) {
            const char* db_password = reinterpret_cast<const char*>(sqlite3_column_text(prepared_sql, 0));
            login_ok = (password == db_password);
        }
    }
    sqlite3_finalize(prepared_sql);
    return login_ok;
}

double Database::getUserCredits(const string& username)
{
    // Start with 0 credits
    double user_credits = 0.0;

    string sql = "SELECT credits FROM users WHERE username = ?;";

    sqlite3_stmt* prepared_sql;
    bool prepared_ok = (sqlite3_prepare_v2(db, sql.c_str(), -1, &prepared_sql, nullptr) == SQLITE_OK);

    if (prepared_ok) {
        // add the username to command
        sqlite3_bind_text(prepared_sql, 1, username.c_str(), -1, SQLITE_STATIC);

        // If we found the user, get their credits
        if (sqlite3_step(prepared_sql) == SQLITE_ROW) {
            user_credits = sqlite3_column_double(prepared_sql, 0);
        }
    }
    else {
        cout << "Error getting credits: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(prepared_sql);
    return user_credits;
}

bool Database::updateUserCredits(const string& username, double new_credits)
{
    // do not allow negative credits
    if (new_credits < 0) {
        new_credits = 0;
    }

    // SQL command to update user's credits
    string sql = "UPDATE users SET credits = ? WHERE username = ?;";

    sqlite3_stmt* prepared_sql;
    bool prepared_ok = (sqlite3_prepare_v2(db, sql.c_str(), -1, &prepared_sql, nullptr) == SQLITE_OK);

    if (prepared_ok) {
        // add new credit amount and username to command
        sqlite3_bind_double(prepared_sql, 1, new_credits);
        sqlite3_bind_text(prepared_sql, 2, username.c_str(), -1, SQLITE_STATIC);

        // try to update the credits
        if (sqlite3_step(prepared_sql) != SQLITE_DONE) {
            cout << "Error updating credits: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(prepared_sql);
            return false;
        }
    }

    sqlite3_finalize(prepared_sql);

    cout << "Credits updated for " << username << ": " << new_credits << " euros" << endl;
    return true;
}