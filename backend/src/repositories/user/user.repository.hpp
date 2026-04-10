#include <iostream>
#include <vector>
struct user
{
    int id;
    std::string name;
    std::string email;
    std::string password;
    std::string profile_picture_object_key;
};

namespace UserRepository{
    int createUser(const user& u);
    std::vector<user> getUsers(int page,int limit);
    user getUser(int id);
    user getUserByEmail(const std::string& email);
    bool updateUser(int id,const user& u);
    bool deleteUser(int id);
}