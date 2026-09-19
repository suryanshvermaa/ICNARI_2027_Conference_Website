#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
namespace v1
{
class student_committee : public drogon::HttpController<student_committee>
{
  public:
    METHOD_LIST_BEGIN
    
    METHOD_ADD(student_committee::createCommitteeMember, "", Post,"AuthFilter"); // -> path: /api/v1/student_committee
    METHOD_ADD(student_committee::getCommitteeMembers, "", Get); // -> path: /api/v1/student_committee?page={1}&limit={2}
    METHOD_ADD(student_committee::getCommitteeMember, "/{1}", Get); // -> path: /api/v1/student_committee/{memberId}
    METHOD_ADD(student_committee::deleteCommitteeMember, "/{1}", Delete,"AuthFilter"); // -> path: /api/v1/student_committee/{memberId}
    METHOD_ADD(student_committee::updateCommitteeMember, "/{1}", Put,"AuthFilter"); // -> path: /api/v1/student_committee/{memberId}

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    void createCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getCommitteeMembers(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId);
    void deleteCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId);
    void updateCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId);
};
}
}
