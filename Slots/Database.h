#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

class Database {
public:
    //  database file path
    Database(const std::string& dbPath);
    ~Database();

    // connection methods
    bool open();
    void close();
    void createTables();

    // user management functions
    bool addUser(
        const std::string& username,
        const std::string& password
    );
    bool loginUser(
        const std::string& username,
        const std::string& password
    );

    // getting, updating credit functions
    double getUserCredits(const std::string& username);
    bool updateUserCredits(
        const std::string& username,
        double newCredits
    );

private:
    sqlite3* db;  
};

#endif