#include <drogon/orm/DbClient.h>
#include <drogon/orm/Exception.h>

#include <json/json.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h> // getpass (Linux)

#include "../src/utils/token.hpp"

static std::string readFileToString(const std::string &path) {
  std::ifstream in(path);
  if (!in) {
    throw std::runtime_error("Cannot open file: " + path);
  }
  std::ostringstream ss;
  ss << in.rdbuf();
  return ss.str();
}

static Json::Value parseJsonFile(const std::string &path) {
  const std::string content = readFileToString(path);

  Json::CharReaderBuilder builder;
  builder["collectComments"] = false;
  std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

  Json::Value root;
  std::string errs;
  const bool ok = reader->parse(content.data(), content.data() + content.size(), &root, &errs);
  if (!ok) {
    throw std::runtime_error("Invalid JSON in " + path + ": " + errs);
  }
  return root;
}

static std::shared_ptr<drogon::orm::DbClient> makePgClientFromConfig(
    const std::string &configPath,
    const std::string &clientName = "default") {
  const Json::Value root = parseJsonFile(configPath);
  const Json::Value dbClients = root.isMember("db_clients") ? root["db_clients"]
                              : root.isMember("dbClients")  ? root["dbClients"]
                                                             : Json::Value(Json::arrayValue);

  if (!dbClients.isArray() || dbClients.empty()) {
    throw std::runtime_error(
        "No DB clients found in " + configPath + " (expected db_clients/dbClients array)");
  }

  Json::Value selected;
  for (const auto &c : dbClients) {
    if (c.isObject() && c.isMember("name") && c["name"].asString() == clientName) {
      selected = c;
      break;
    }
  }
  if (selected.isNull()) {
    selected = dbClients[0];
  }

  const std::string rdbms = selected.get("rdbms", "postgresql").asString();
  if (rdbms != "postgresql") {
    throw std::runtime_error("Unsupported rdbms: " + rdbms);
  }

  const std::string host = selected.get("host", "127.0.0.1").asString();
  const int port = selected.get("port", 5432).asInt();
  const std::string dbname = selected.get("dbname", "").asString();
  const std::string user = selected.get("user", "").asString();
  const std::string passwd = selected.get("passwd", "").asString();
  const std::size_t connNum = static_cast<std::size_t>(selected.get("connection_number", 1).asUInt());

  if (dbname.empty() || user.empty()) {
    throw std::runtime_error("Missing dbname/user in " + configPath);
  }

  std::ostringstream conn;
  conn << "host=" << host << " port=" << port << " dbname=" << dbname << " user=" << user;
  if (!passwd.empty()) {
    conn << " password=" << passwd;
  }

  return drogon::orm::DbClient::newPgClient(conn.str(), connNum);
}

static std::string promptLine(const char *label, bool allowEmpty = false) {
  while (true) {
    std::cout << label;
    std::string s;
    std::getline(std::cin, s);
    if (allowEmpty || !s.empty()) return s;
  }
}

static std::string promptPassword(const char *label) {
  while (true) {
    char *p = ::getpass(label); // input hidden
    if (p && *p) return std::string(p);
  }
}

int main(int argc, char **argv) {
  try {
    const std::string configPath = (argc > 1) ? argv[1] : std::string("./config/config.json");
    auto db = makePgClientFromConfig(configPath, "default");

    const std::string email = promptLine("Email: ");
    const std::string username = promptLine("Username: ");
    const std::string password = promptPassword("Password: ");
    const std::string confirm = promptPassword("Confirm Password: ");

    if (password != confirm) {
      std::cerr << "Error: passwords do not match.\n";
      return 2;
    }

    const std::string passwordHash = Auth::getHashPassword(password);
    const std::string role = "admin";

    // NOTE: Use the correct column name in your DB.
    // If your table column is passwordHash (camelCase), use "passwordHash".
    // If it's password_hash (snake_case), keep password_hash.
    auto r = db->execSqlSync(
        "INSERT INTO users (email, username, password_hash, role) "
        "VALUES ($1, $2, $3, $4)",
        email, username, passwordHash, role);

    std::cout << "User created: " << email << " (role=" << role
              << "), rows=" << r.affectedRows() << "\n";
    return 0;

  } catch (const drogon::orm::DrogonDbException &e) {
    std::cerr << "DB error: " << e.base().what() << "\n";
    return 3;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 4;
  }
}