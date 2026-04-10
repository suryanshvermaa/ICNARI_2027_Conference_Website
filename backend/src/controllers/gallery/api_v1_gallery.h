#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
namespace v1
{
class gallery : public drogon::HttpController<gallery>
{
  public:
    METHOD_LIST_BEGIN

    METHOD_ADD(gallery::getImages, "/images", Get); // path -> /api/v1/gallery/images
    METHOD_ADD(gallery::getImage, "/image/{1}", Get); // path -> /api/v1/gallery/image/{arg1}
    METHOD_ADD(gallery::deleteImage, "/image/{1}", Delete,"AuthFilter"); // path -> /api/v1/gallery/image/{arg1}
    METHOD_ADD(gallery::uploadImage, "/image", Post,"AuthFilter"); // path -> /api/v1/gallery/image

    METHOD_LIST_END

    void getImages(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void getImage(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int imageId);
    void deleteImage(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int imageId);
    void uploadImage(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};
}
}
