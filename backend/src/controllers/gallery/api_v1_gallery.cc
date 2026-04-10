#include "api_v1_gallery.h"
#include <json/json.h>
#include "../../utils/response.hpp"
#include "../../utils/AppError.hpp"
#include "../../repositories/gallery/gallery.repository.hpp"
#include "../../services/S3Service.h"

using namespace api::v1;

vector<std::string> stringSplit(const std::string& str, char delimiter)
{
    vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}


void gallery::uploadImage(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    try
    {
        auto body=req->getJsonObject();
        if(!body) throw AppError("Invalid JSON body", k400BadRequest);
        std::string title=(*body)["title"].asString();
        std::string tags=(*body)["tags"].asString();
        std::string imageObjectUrl=(*body)["imageObjectKey"].asString();
        std::string imageObjectKey=urlToKey(imageObjectUrl);
        int id=GalleryRepository::createGallery(title, stringSplit(tags, ','), imageObjectKey);
        if(id==0) throw AppError("Failed to create gallery", k500InternalServerError);
        Json::Value res;
        res["id"]=id;
        callback(Response::success(k200OK,"Gallery created successfully",res));
    }
    catch(const AppError& e)
    {
        std::cerr << e.what() << '\n';
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        callback(Response::error(k400BadRequest, "Failed to upload image"));
    }
}

void gallery::deleteImage(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int id)
{
    try
    {
        bool deleted=GalleryRepository::deleteGallery(id);
        if(!deleted) throw AppError("Gallery not found", k404NotFound);
        callback(Response::success(k200OK,"Gallery deleted successfully"));
    }
    catch(const AppError& e)
    {
        std::cerr << e.what() << '\n';
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        callback(Response::error(k400BadRequest, "Failed to delete image"));
    }
}

void gallery::getImages(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    try
    {
        std::string page=req->getParameter("page");
        std::string limit=req->getParameter("limit");
        if(page.empty()) page="1";
        if(limit.empty()) limit="10";
        auto images=GalleryRepository::getAllGalleries(stoi(limit), stoi(page));
        Json::Value res;
        for(const auto& img:images)
        {
            Json::Value imgJson;
            imgJson["id"]=img.id;
            imgJson["title"]=img.title;
            imgJson["tags"]=Json::arrayValue;
            for(const auto& tag:img.tags) imgJson["tags"].append(tag);
            imgJson["imageUrl"]=img.imageUrl;
            res.append(imgJson);
        }
        callback(Response::success(k200OK,"Galleries fetched successfully",res));
    }
    catch(const AppError& e)
    {
        std::cerr << e.what() << '\n';
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        callback(Response::error(k400BadRequest, "Failed to fetch images"));
    }
}

void gallery::getImage(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int id)
{
    try
    {
        auto image=GalleryRepository::getGalleryById(id);
        if(image.id==0) throw AppError("Gallery not found", k404NotFound);
        Json::Value res;
        res["id"]=image.id;
        res["title"]=image.title;
        res["tags"]=Json::arrayValue;
        for(const auto& tag:image.tags) res["tags"].append(tag);
        res["imageUrl"]=image.imageUrl;
        res["createdAt"]=image.createdAt;
        callback(Response::success(k200OK,"Gallery fetched successfully",res));
    }
    catch(const AppError& e)
    {
        std::cerr << e.what() << '\n';
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        callback(Response::error(k400BadRequest, "Failed to fetch image"));
    }
}