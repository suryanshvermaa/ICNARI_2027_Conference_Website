#include"gallery.repository.hpp"
using std::string;

int GalleryRepository::createGallery(const std::string& title, const vector<std::string>& tags, const std::string& imageObjectKey){
    try
    {
        Gallery newGallery;
        string tagsString="";
        for(int i=0;i<tags.size();i++){
            tagsString+=tags[i];
            if(i!=tags.size()-1){
                tagsString+=",";
            }
        }
        newGallery.setTitle(title);
        newGallery.setTags(tagsString);
        newGallery.setImageObjectKey(imageObjectKey);

        Mapper<Gallery> mapper(Database::getClient());
        mapper.insert(newGallery);
        return newGallery.getValueOfId();
    }
    catch(const std::exception& e)
    {
        LOG_ERROR << e.what();
        return 0;
    }
}

bool GalleryRepository::deleteGallery(int id){
    try
    {
        Gallery galleryToDelete;
        Mapper<Gallery> mapper(Database::getClient());
        mapper.findByPrimaryKey(id);
        if(galleryToDelete.getValueOfId()==0){
            return false;
        }
        mapper.deleteByPrimaryKey(id);
        return true;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR << e.what();
        return false;
    }
}

galleryStruct GalleryRepository::getGalleryById(int id){
    try
    {
        Gallery gal;
        Mapper<Gallery> mapper(Database::getClient());
        mapper.findByPrimaryKey(id);
        gal = mapper.findByPrimaryKey(id);
        if(gal.getValueOfId()==0){
            return galleryStruct({0, "", {}, "", ""});
        }
        galleryStruct g;
        g.id=gal.getValueOfId();
        g.title=gal.getValueOfTitle();
        string tags=gal.getValueOfTags();
        size_t pos = 0;
        string token;
        while ((pos = tags.find(",")) != string::npos) {
            token = tags.substr(0, pos);
            g.tags.push_back(token);
            tags.erase(0, pos + 1);
        }
        g.tags.push_back(tags);
        g.imageObjectKey=gal.getValueOfImageObjectKey();
        g.createdAt=gal.getValueOfCreatedAt().toDbString();
        return g;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR << e.what();
        return galleryStruct({0, "", {}, "", ""});
    }
}

vector<galleryStruct> GalleryRepository::getAllGalleries(int limit, int page){
    try
    {
        Mapper<Gallery> mapper(Database::getClient());
        vector<Gallery> galleries = mapper.limit(limit).offset((page-1)*limit).findAll();
        vector<galleryStruct> result;
        for(auto &gal : galleries){
            galleryStruct g;
            g.id=gal.getValueOfId();
            g.title=gal.getValueOfTitle();
            string tags=gal.getValueOfTags();
            size_t pos = 0;
            string token;
            while ((pos = tags.find(",")) != string::npos) {
                token = tags.substr(0, pos);
                g.tags.push_back(token);
                tags.erase(0, pos + 1);
            }
            g.tags.push_back(tags);
            g.imageObjectKey=gal.getValueOfImageObjectKey();
            g.createdAt=gal.getValueOfCreatedAt().toDbString();
            result.push_back(g);
        }
        return result;
    }
    catch(const std::exception& e)
    {
        LOG_ERROR << e.what();
        return {};
    }
}
