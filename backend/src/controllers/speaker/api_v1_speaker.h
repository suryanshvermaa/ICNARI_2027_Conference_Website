#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
namespace v1
{
class speaker : public drogon::HttpController<speaker>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(speaker::getSpeakers, "", Get); // path -> /api/v1/speaker?page={1}&limit={2}
    METHOD_ADD(speaker::getSpeaker, "/{1}", Get); // path -> /api/v1/speaker/{speakerId}
    METHOD_ADD(speaker::deleteSpeaker, "/{1}", Delete); // path -> /api/v1/speaker/{speakerId}
    METHOD_ADD(speaker::updateSpeaker, "/{1}", Put); // path -> /api/v1/speaker/{speakerId}
    METHOD_ADD(speaker::createSpeaker, "", Post); // path -> /api/v1/speaker

    METHOD_LIST_END
    void createSpeaker(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getSpeakers(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getSpeaker(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int speakerId);
    void deleteSpeaker(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int speakerId);
    void updateSpeaker(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int speakerId);
};
}
}
