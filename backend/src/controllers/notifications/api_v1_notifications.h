#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
namespace v1
{
class notifications : public drogon::HttpController<notifications>
{
  public:
    METHOD_LIST_BEGIN

    METHOD_ADD(notifications::getNotifications, "", Get); // path is /api/v1/notifications?page=1&limit=10&type=HIGHLIGHTS
    METHOD_ADD(notifications::getNotification, "/{id}", Get); // path is /
    METHOD_ADD(notifications::createNotification, "", Post,"AuthFilter","AdminFilter"); // path is /api/v1/notifications
    METHOD_ADD(notifications::updateNotification, "/{id}", Put,"AuthFilter","AdminFilter"); // path is /api/v1/notifications/{id}
    METHOD_ADD(notifications::deleteNotification, "/{id}", Delete,"AuthFilter","AdminFilter"); // path is /

    METHOD_LIST_END
    void getNotifications(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getNotification(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int id);
    void createNotification(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void updateNotification(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int id);
    void deleteNotification(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int id);
};
}
}
