#include"./user.repository.hpp"
#include"../../models/User.h"
#include"../../config/database.h"
#include"../../services/S3Service.h"

using namespace drogon_model::ieee_conference_db;
using namespace drogon::orm;

int UserRepository::createUser(const user& u){
    try
    {
        Users user;
        user.setUsername(u.name);
        user.setEmail(u.email);
        user.setPasswordHash(u.passwordHash);
        user.setRole(u.role);
        if(u.provider != "") user.setProvider(u.provider);
        if(u.profilePictureUrl != "") user.setProfilePictureObjectKey(u.profilePictureUrl);
        Mapper<Users> mapper(Database::getClient());
        mapper.insert(user);
        return user.getValueOfId();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }
}

std::vector<user> UserRepository::getUsers(int page,int limit){
    try
    {
        std::vector<user> users;
        Mapper<Users> mapper(Database::getClient());
        auto dbUsers=mapper.limit(limit).offset((page-1)*limit).findAll();
        for(const auto& dbUser:dbUsers){
            user u;
            u.id=dbUser.getValueOfId();
            u.name=dbUser.getValueOfUsername();
            u.email=dbUser.getValueOfEmail();
            u.passwordHash=dbUser.getValueOfPasswordHash();
            u.role=dbUser.getValueOfRole();
            u.provider=dbUser.getValueOfProvider();
            if(dbUser.getValueOfProfilePictureObjectKey() != ""&&u.profilePictureUrl.find("https://") == std::string::npos) u.profilePictureUrl=getSignedUrl(dbUser.getValueOfProfilePictureObjectKey());
            users.push_back(u);
        }
        return users;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return {};
    }
}

user UserRepository::getUser(int id){
    try
    {
        Mapper<Users> mapper(Database::getClient());
        auto dbUser=mapper.findByPrimaryKey(id);
        user u;
        u.id=dbUser.getValueOfId();
        u.name=dbUser.getValueOfUsername();
        u.email=dbUser.getValueOfEmail();
        u.passwordHash=dbUser.getValueOfPasswordHash();
        u.role=dbUser.getValueOfRole();
        u.provider=dbUser.getValueOfProvider();
        if(dbUser.getValueOfProfilePictureObjectKey() != ""&&u.profilePictureUrl.find("https://") == std::string::npos) u.profilePictureUrl=getSignedUrl(dbUser.getValueOfProfilePictureObjectKey());
        return u;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return {};
    }
}

user UserRepository::getUserByEmail(const std::string& email){
    try
    {
        Mapper<Users> mapper(Database::getClient());
        auto dbUser=mapper.findOne(Criteria(Users::Cols::_email, email));
        user u;
        u.id=dbUser.getValueOfId();
        u.name=dbUser.getValueOfUsername();
        u.email=dbUser.getValueOfEmail();
        u.passwordHash=dbUser.getValueOfPasswordHash();
        u.role=dbUser.getValueOfRole();
        u.provider=dbUser.getValueOfProvider();
        if(dbUser.getValueOfProfilePictureObjectKey() != ""&&u.profilePictureUrl.find("https://") == std::string::npos) u.profilePictureUrl=getSignedUrl(dbUser.getValueOfProfilePictureObjectKey());
        return u;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return {};
    }
}

std::string getS3KeyFromUrl(const std::string &url){
    size_t pos = url.find("?");
    std::string key;
    if (pos != std::string::npos) {
        key = url.substr(0, pos);
    }
    size_t lastSlash = key.find_last_of("/");
    if (lastSlash != std::string::npos) {
        return key.substr(lastSlash + 1);
    }
    return key;
}

bool UserRepository::updateUser(int id,const user& u){
    try
    {
        Mapper<Users> mapper(Database::getClient());
        auto dbUser=mapper.findByPrimaryKey(id);
        dbUser.setUsername(u.name);
        dbUser.setEmail(u.email);
        dbUser.setPasswordHash(u.passwordHash);
        dbUser.setRole(u.role);
        if(u.profilePictureUrl != "") dbUser.setProfilePictureObjectKey(getS3KeyFromUrl(u.profilePictureUrl));
        mapper.update(dbUser);
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool UserRepository::deleteUser(int id){
    try
    {
        Mapper<Users> mapper(Database::getClient());
        auto dbUser=mapper.findByPrimaryKey(id); // Check if the user exists
        if(!dbUser.getId()){
            return false; // User does not exist
        }
        mapper.deleteByPrimaryKey(id);
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}