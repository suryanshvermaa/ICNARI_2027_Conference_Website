#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
namespace v1
{
class contact : public drogon::HttpController<contact>
{
  public:
    METHOD_LIST_BEGIN

    METHOD_ADD(contact::createContact, "", Post); // path -> /api/v1/contact
    METHOD_ADD(contact::deleteContact, "", Delete); // path -> /api/v1/contact
    METHOD_ADD(contact::getContact, "/{contactId}", Get); // path -> /api/v1/contact/{contactId}
    METHOD_ADD(contact::getContacts, "", Get); // path -> /api/v1/contact
    
    METHOD_LIST_END
    void createContact(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void deleteContact(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int contactId);
    void getContact(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int contactId);
    void getContacts(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};
}
}
