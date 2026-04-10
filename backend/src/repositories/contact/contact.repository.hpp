#pragma once

#include"../../config/database.h"
#include"../../models/Contacts.h"
#include<vector>

using namespace drogon_model::icnari_conference_db;
using namespace drogon::orm;
using std::vector;

struct contactStruct{
    int id;
    std::string name;
    std::string email;
    std::string phone;
    std::string subject;
    std::string message;
    std::string created_at;
};

namespace ContactRepository {
    vector<contactStruct> getContacts(int page,int limit);
    int createContact(const contactStruct& contact);
    bool deleteContact(int id);
    contactStruct getContactById(int id);
}