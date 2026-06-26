#pragma once
#include <string>
#include <mysql.h> 

class DatabaseManager {
private:
    MYSQL* conn;
public:
    DatabaseManager();
    bool Connect(const std::string& host, const std::string& user, const std::string& pass, const std::string& db);
    void LogAudit(const std::string& operation, const std::string& filename, const std::string& status);
    ~DatabaseManager();
};