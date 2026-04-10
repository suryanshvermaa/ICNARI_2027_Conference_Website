#include"./notification.repository.hpp"
#include"../../models/Notifications.h"
#include"../../config/database.h"

using namespace drogon_model::icnari_conference_db;
using namespace drogon::orm;

int NotificationRepository::createNotification(const notification &n){
    try
    {
        Notifications noti;
        noti.setTitle(n.title);
        noti.setDescription(n.description);
        noti.setType(n.type);
        noti.setLink(n.link);
        noti.setPriority(n.priority);
        Mapper<Notifications> mapper(Database::getClient());
        mapper.insert(noti);
        return noti.getValueOfId();
    }
    catch(const std::exception& e)
    {
        LOG_ERROR<<e.what();
        return 0;
    }
}

bool NotificationRepository::updateNotification(int id,const notification &n){
    try
    {
        Mapper<Notifications> mapper(Database::getClient());
        Notifications noti=mapper.findByPrimaryKey(id);
        if(n.title!="") noti.setTitle(n.title);
        if(n.description!="") noti.setDescription(n.description);
        if(n.type!="") noti.setType(n.type);
        if(n.link!="") noti.setLink(n.link);
        if(n.priority!=0) noti.setPriority(n.priority);
        mapper.update(noti);
        return true;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR << e.what();
        return false;
    } 
}

bool NotificationRepository::deleteNotification(int id){
    try
    {
        Mapper<Notifications> mapper(Database::getClient());
        Notifications noti=mapper.findByPrimaryKey(id);
        if(noti.getValueOfId()==0) return false;
        mapper.deleteByPrimaryKey(id);
        return true;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR << e.what();
        return false;
    }
}

notification NotificationRepository::getNotification(int id){
    try
    {
        Mapper<Notifications> mapper(Database::getClient());
        Notifications noti=mapper.findByPrimaryKey(id);
        notification n;
        n.id=noti.getValueOfId();
        n.title=noti.getValueOfTitle();
        n.link=noti.getValueOfLink();
        n.priority=noti.getValueOfPriority();
        n.type=noti.getValueOfType();
        n.description=noti.getValueOfDescription();
        return n;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR << e.what();
        return {};
    }
    
}

vector<notification> NotificationRepository::getNotifications(int page,int limit,const string &type){
    try
    {
        vector<notification> notifications;
        Mapper<Notifications> mapper(Database::getClient());
        auto dbNotifications=mapper.offset((page-1)*limit).limit(limit).findBy(
            Criteria(Notifications::Cols::_type, type)
        );
        for(auto &dbNoti:dbNotifications){
            notification n;
            n.id=dbNoti.getValueOfId();
            n.title=dbNoti.getValueOfTitle();
            n.link=dbNoti.getValueOfLink();
            n.priority=dbNoti.getValueOfPriority();
            n.type=dbNoti.getValueOfType();
            n.description=dbNoti.getValueOfDescription();
            notifications.push_back(n);
        }
        return notifications;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR << e.what();
        return {};
    }
    
}