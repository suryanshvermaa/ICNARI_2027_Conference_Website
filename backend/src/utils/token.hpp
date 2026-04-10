#pragma once

#include<iostream>

struct data{
    int userId;
};
namespace Auth {
    std::string getHashPassword(const std::string &password);
    bool comparePassword(const std::string &password, const std::string &hash);
    std::string createToken(const int userId);
    data decodeAndverifyToken(const std::string &token);
}