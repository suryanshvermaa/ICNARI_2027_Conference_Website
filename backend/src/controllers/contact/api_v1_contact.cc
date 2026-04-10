#include "api_v1_contact.h"
#include <json/json.h>
#include "../../utils/response.hpp"
#include "../../utils/AppError.hpp"
#include "../../repositories/contact/contact.repository.hpp"
#include "../../services/S3Service.h"

using namespace api::v1;

void contact::createContact(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    try{
        auto json=req->getJsonObject();
        if(!json){
            throw AppError("Invalid JSON", k400BadRequest);
        }
        if(!json->isMember("name")||!json->isMember("email")||!json->isMember("subject")||!json->isMember("message")){
            throw AppError("Missing required fields", k400BadRequest);
        }
        contactStruct contact;
        contact.name=(*json)["name"].asString();
        contact.email=(*json)["email"].asString();
        if(json->isMember("phone")){
            contact.phone=(*json)["phone"].asString();
        }
        contact.subject=(*json)["subject"].asString();
        contact.message=(*json)["message"].asString();

        int id=ContactRepository::createContact(contact);
        Json::Value data;
        data["id"]=id;
        callback(Response::success(k201Created,"Contact created successfully",data));
    }catch(const AppError& err){
        callback(Response::error(err.statusCode, err.what()));
    }catch(const std::exception& err){
        callback(Response::error(k500InternalServerError, err.what()));
    }
}

void contact::deleteContact(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int contactId){
    try{
        bool success=ContactRepository::deleteContact(contactId);
        if(success){
            callback(Response::success(k200OK,"Contact deleted successfully"));
        }else{
            throw AppError("Contact not found", k404NotFound);
        }
    }catch(const AppError& err){
        callback(Response::error(err.statusCode, err.what()));
    }catch(const std::exception& err){
        callback(Response::error(k500InternalServerError, err.what()));
    }
}

void contact::getContact(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int contactId){
    try{
        auto contact=ContactRepository::getContactById(contactId);
        if(contact.id==0){
            throw AppError("Contact not found", k404NotFound);
        }
        Json::Value data;
        data["id"]=contact.id;
        data["name"]=contact.name;
        data["email"]=contact.email;
        data["phone"]=contact.phone;
        data["subject"]=contact.subject;
        data["message"]=contact.message;
        data["created_at"]=contact.created_at;
        callback(Response::success(k200OK,"Contact retrieved successfully",data));
    }catch(const AppError& err){
        callback(Response::error(err.statusCode, err.what()));
    }catch(const std::exception& err){
        callback(Response::error(k500InternalServerError, err.what()));
    }
}

void contact::getContacts(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    try{
        auto queryParameters=req->getParameters();
        int page=1;
        int limit=10;
        if(queryParameters.find("page")!=queryParameters.end()){
            page=std::stoi(queryParameters["page"]);
        }
        if(queryParameters.find("limit")!=queryParameters.end()){
            limit=std::stoi(queryParameters["limit"]);
        }
        auto contacts=ContactRepository::getContacts(page, limit);
        Json::Value data;
        for(const auto& contact:contacts){
            Json::Value contactJson;
            contactJson["id"]=contact.id;
            contactJson["name"]=contact.name;
            contactJson["email"]=contact.email;
            contactJson["phone"]=contact.phone;
            contactJson["subject"]=contact.subject;
            contactJson["message"]=contact.message;
            contactJson["created_at"]=contact.created_at;
            data.append(contactJson);
        }
        callback(Response::success(k200OK,"Contacts retrieved successfully",data));
    }catch(const AppError& err){
        callback(Response::error(err.statusCode, err.what()));
    }catch(const std::exception& err){
        callback(Response::error(k500InternalServerError, err.what()));
    }
}

