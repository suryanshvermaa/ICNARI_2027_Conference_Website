#include "api_v1_notifications.h"
#include "../../utils/response.hpp"
#include "../../utils/AppError.hpp"
#include "../../repositories/notifications/notification.repository.hpp"

using namespace api::v1;

void notifications::getNotifications(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    try
    {
        auto query=req->getParameters();
        int page=1;
        int limit=10;
        std::string type="recent_update";
        if(query.find("page")!=query.end())
            page=std::stoi(query["page"]);
        if(query.find("limit")!=query.end())
            limit=std::stoi(query["limit"]);
        if(query.find("type")!=query.end())
            type=query["type"];
        // type checking isValid
        bool isValidType=false;
        for(const auto& validType:validTypes)
        {
            if(type==validType)
            {
                isValidType=true;
                break;
            }
        }
        if(!isValidType) throw AppError("Invalid Notification Type!",k404NotFound);
        
        auto notifications=NotificationRepository::getNotifications(page,limit,type);
        Json::Value response(Json::arrayValue);
        for(const auto& n:notifications){
            Json::Value notificationJson;
            notificationJson["id"]=n.id;
            notificationJson["title"]=n.title;
            notificationJson["description"]=n.description;
            notificationJson["type"]=n.type;
            notificationJson["link"]=n.link;
            notificationJson["priority"]=n.priority;
            notificationJson["createdAt"]=n.createdAt;
            response.append(notificationJson);
        }
        callback(Response::success(k200OK,"Notifications retrieved successfully",response));
        
    }
    catch(const AppError& e)
    {
        LOG_ERROR<<e.what();
        callback(Response::error(e.statusCode,e.what()));
    }
    catch(const std::exception& e)
    {
        LOG_ERROR<<e.what();
        callback(Response::error(k400BadRequest,e.what()));
    }
}

void notifications::getNotification(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int id)
{
    try
    {
        auto notification=NotificationRepository::getNotification(id);
        if(notification.id==0) throw AppError("Notification not found!",k404NotFound);
        Json::Value notificationJson;
        notificationJson["id"]=notification.id;
        notificationJson["title"]=notification.title;
        notificationJson["description"]=notification.description;
        notificationJson["type"]=notification.type;
        notificationJson["link"]=notification.link;
        notificationJson["priority"]=notification.priority;
        notificationJson["createdAt"]=notification.createdAt;
        callback(Response::success(k200OK,"Notification retrieved successfully",notificationJson));
    }
    catch(const AppError& e)
    {
        LOG_ERROR<<e.what();
        callback(Response::error(e.statusCode,e.what()));
    }
    catch(const std::exception& e)
    {
        LOG_ERROR<<e.what();
        callback(Response::error(k400BadRequest,e.what()));
    }
}

void notifications::createNotification(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    try
    {
        auto json=req->getJsonObject();
        notification n;
        if(!json->isMember("title") || !json->isMember("description") || !json->isMember("type") || !json->isMember("link") || !json->isMember("priority"))
            throw AppError("Missing required fields!",k400BadRequest);
        n.title=(*json)["title"].asString();
        n.description=(*json)["description"].asString();
        n.type=(*json)["type"].asString();
        n.link=(*json)["link"].asString();
        n.priority=(*json)["priority"].asInt();
        int id=NotificationRepository::createNotification(n);
        if(id==0) throw AppError("Failed to create notification!",k500InternalServerError);
        callback(Response::success(k201Created,"Notification created successfully",Json::Value(id)));
    }
    catch(const AppError& e)
    {
        LOG_ERROR<<e.what();
        callback(Response::error(e.statusCode,e.what()));
    }
    catch(const std::exception& e)
    {
        LOG_ERROR<<e.what();
        callback(Response::error(k400BadRequest,e.what()));
    }
}

void notifications::updateNotification(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int id)
{
    try
    {
        auto json=req->getJsonObject();
        notification n;
        if(json->isMember("title")) n.title=(*json)["title"].asString();
        if(json->isMember("type")) n.type=(*json)["type"].asString();
        if(json->isMember("description")) n.description=(*json)["description"].asString();
        if(json->isMember("link")) n.link=(*json)["link"].asString();
        if(json->isMember("priority")) n.priority=(*json)["priority"].asInt();
        bool success=NotificationRepository::updateNotification(id,n);
        if(!success) throw AppError("Failed to update notification!",k500InternalServerError);
        callback(Response::success(k200OK,"Notification updated successfully"));
    }
    catch(const AppError& e)
    {
        LOG_ERROR<<e.what();
        callback(Response::error(e.statusCode,e.what()));
    }
    catch(const std::exception& e)
    {
        LOG_ERROR<<e.what();
        callback(Response::error(k400BadRequest,e.what()));
    }
}

void notifications::deleteNotification(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int id)
{
    try
    {
        bool success=NotificationRepository::deleteNotification(id);
        if(!success) throw AppError("Failed to delete notification!",k500InternalServerError);
        callback(Response::success(k200OK,"Notification deleted successfully"));
    }
    catch(const AppError& e)
    {
        LOG_ERROR<<e.what();
        callback(Response::error(e.statusCode,e.what()));
    }
    catch(const std::exception& e)
    {
        LOG_ERROR<<e.what();
        callback(Response::error(k400BadRequest,e.what()));
    }
}
