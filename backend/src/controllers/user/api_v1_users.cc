#include "api_v1_users.h"
#include <json/json.h>
#include "../../utils/response.hpp"
#include "../../utils/AppError.hpp"
#include "../../repositories/user/user.repository.hpp"
#include "../../services/S3Service.h"
#include "../../utils/token.hpp"

using namespace api::v1;


void users::getUser(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    try
    {
        auto query=req->getParameters();
        if(query.find("id")==query.end())
            throw AppError("User ID is required", k400BadRequest);
        int id=std::stoi(query["id"]);
        user u=UserRepository::getUser(id);
        if(u.id == 0)
            throw AppError("User not found", k404NotFound);
        
        Json::Value response;
        response["id"]=u.id;
        response["name"]=u.name;
        response["email"]=u.email;
        callback(Response::success(k200OK,"User retrieved successfully",response));
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

void users::updateUser(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    try
    {
        auto id=req->getParameter("userId");
        if(id.empty())
            throw AppError("User ID is required", k400BadRequest);
        int userId=std::stoi(id);
        auto reqBody=req->getJsonObject();
        if(reqBody==nullptr)
            throw AppError("Invalid request body", k400BadRequest);
        
        user u;
        u.id=userId;
        if(reqBody->isMember("name"))
            u.name=(*reqBody)["name"].asString();
        if(reqBody->isMember("email"))
            u.email=(*reqBody)["email"].asString();
        if(reqBody->isMember("password"))
            u.password=Auth::getHashPassword((*reqBody)["password"].asString());
        if(reqBody->isMember("profilePicture"))
            u.profile_picture_object_key=(*reqBody)["profilePicture"].asString();
        
        if(!UserRepository::updateUser(userId,u))
            throw AppError("Failed to update user", k500InternalServerError);
        
        callback(Response::success(k200OK,"User updated successfully"));
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


void users::deleteUser(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int id){
    try
    {
        if(!UserRepository::deleteUser(id))
            throw AppError("User not found or already deleted", k404NotFound);
        
        callback(Response::success(k200OK,"User deleted successfully"));
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

void users::uploadProfilePicture(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    try
    {
        MultiPartParser fileParser;
        fileParser.parse(req);
        if(fileParser.getFiles().empty())
            throw AppError("No file uploaded", k400BadRequest);
        size_t num_of_files = fileParser.getFiles().size();
        if(num_of_files > 1)
            throw AppError("Multiple files uploaded. Only one file is allowed.", k400BadRequest);
        
        // upload file to S3 and get the object key
        auto id=req->getParameter("userId");
        if(id.empty())  throw AppError("User ID is required", k400BadRequest);
        int userId=std::stoi(id);
        auto file=fileParser.getFiles()[0];
        const std::string fileName=file.getFileName()+"_"+id; // to avoid name collision
        std::string objectKey=putObject(fileName,file);
        // update user profile picture
        UserRepository::updateUser(userId, user{.profile_picture_object_key=objectKey});
        callback(Response::success(k200OK,"Profile picture uploaded successfully"));
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
