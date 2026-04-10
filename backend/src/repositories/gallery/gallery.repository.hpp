#pragma once

#include"../../config/database.h"
#include"../../models/Gallery.h"
#include"../../services/S3Service.h"
#include<vector>

using namespace drogon_model::ieee_conference_db;
using namespace drogon::orm;
using std::vector;

struct galleryStruct{
    int id;
    std::string title;
    vector<std::string> tags;
    std::string imageUrl;
    std::string createdAt;
};

namespace GalleryRepository
{
    vector<galleryStruct> getAllGalleries(int limit, int page);
    galleryStruct getGalleryById(int id);
    bool deleteGallery(int id);
    int createGallery(const std::string& title, const vector<std::string>& tags, const std::string& imageObjectKey);
}
