#include "api_v1_auth.h"
#include <json/json.h>
#include "../../utils/token.hpp"
#include "../../utils/response.hpp"
#include "../../utils/AppError.hpp"
#include "../../repositories/user/user.repository.hpp"

using namespace api::v1;

void auth::login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    try
    {
        auto reqBody=req->getJsonObject();
        if(!reqBody || !reqBody->isMember("email") || !reqBody->isMember("password"))
            throw AppError("Email and password are required", k400BadRequest);
        const std::string email=(*reqBody)["email"].asString();
        const std::string password=(*reqBody)["password"].asString();
        user u=UserRepository::getUserByEmail(email);
        if(u.id == 0)
            throw AppError("User not found", k404NotFound);
        if(!Auth::comparePassword(password,u.passwordHash))
            throw AppError("Invalid password", k400BadRequest);

        Json::Value response;
        response["token"] = Auth::createToken(u.id,u.role);
        response["user_id"] = u.id;
        response["name"] = u.name;
        response["email"] = u.email;
        callback(Response::success(k200OK,"Login successful",response));
    }
    catch(const AppError& e)
    {
        LOG_ERROR << e.what();
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(const std::exception& e)
    {
        LOG_ERROR << e.what();
        callback(Response::error(k400BadRequest, "Invalid request"));
    }   
}

// void auth::signup(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
//     try
//     {
//         auto reqBody=req->getJsonObject();
//         if(!reqBody || !reqBody->isMember("name") || !reqBody->isMember("email") || !reqBody->isMember("password"))
//             throw AppError("Name, email and password are required", k400BadRequest);
//         const std::string name=(*reqBody)["name"].asString();
//         const std::string email=(*reqBody)["email"].asString();
//         const std::string password=(*reqBody)["password"].asString();
//         if(UserRepository::getUserByEmail(email).id != 0)
//             throw AppError("Email already in use", k400BadRequest);
        
//         user u;
//         u.name=name;
//         u.email=email;
//         u.passwordHash=Auth::getHashPassword(password);
//         u.role="user";
//         int userId=UserRepository::createUser(u);
//         if(userId==0)
//             throw AppError("Failed to create user", k500InternalServerError);
        
//         Json::Value response;
//         response["token"] = Auth::createToken(userId,u.role);
//         response["user_id"] = userId;
//         response["name"] = name;
//         response["email"] = email;
//         callback(Response::success(k201Created,"Signup successful",response));
//     }
//     catch(const AppError& e)
//     {
//         LOG_ERROR << e.what();
//         callback(Response::error(e.statusCode, e.what()));
//     }
//     catch(const std::exception& e)
//     {
//         LOG_ERROR << e.what();
//         callback(Response::error(k400BadRequest, "Invalid request"));
//     }   
// }
