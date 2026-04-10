#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
namespace v1
{
class committee : public drogon::HttpController<committee>
{
  public:
    METHOD_LIST_BEGIN
    
    METHOD_ADD(committee::createCommitteeMember, "", Post); // -> path: /api/v1/committee
    METHOD_ADD(committee::getCommitteeMembers, "", Get); // -> path: /api/v1/committee?page={1}&limit={2}
    ADD_METHOD_TO(committee::getCommitteeMember, "/{1}", Get); // -> path: /api/v1/committee/{memberId}
    ADD_METHOD_TO(committee::deleteCommitteeMember, "/{1}", Delete); // -> path: /api/v1/committee/{memberId}
    ADD_METHOD_TO(committee::updateCommitteeMember, "/{1}", Put); // -> path: /api/v1/committee/{memberId}

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    void createCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getCommitteeMembers(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,std::string committee);
    void getCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId);
    void deleteCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId);
    void updateCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId);
};
}
}
