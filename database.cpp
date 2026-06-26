#include "database.h"
#include <iostream>
#include <ctime>

DatabaseManager::DatabaseManager() {
    conn = mysql_init(NULL);
}

bool DatabaseManager::Connect(const std::string& host, const std::string& user, const std::string& pass, const std::string& db) {
    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), pass.c_str(), db.c_str(), 0, NULL, 0)) {
        std::cerr << "MySQL Connection Failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    std::string schema = "CREATE TABLE IF NOT EXISTS crypto_audit ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "timestamp VARCHAR(50), "
        "operation VARCHAR(50), "
        "filename VARCHAR(100), "
        "status VARCHAR(50));";
    mysql_query(conn, schema.c_str());
    return true;
}

void DatabaseManager::LogAudit(const std::string& operation, const std::string& filename, const std::string& status) {
    time_t now = time(0);
    std::string ts = ctime(&now);
    if (!ts.empty()) ts.pop_back();
    std::string query = "INSERT INTO crypto_audit (timestamp, operation, filename, status) VALUES ('"
        + ts + "', '" + operation + "', '" + filename + "', '" + status + "');";
    mysql_query(conn, query.c_str());
}

DatabaseManager::~DatabaseManager() {
    if (conn) mysql_close(conn);
}