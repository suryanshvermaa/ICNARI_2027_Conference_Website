#pragma once

#include"../../config/database.h"
#include"../../models/Speakers.h"
#include"../../services/S3Service.h"
#include<vector>

using namespace drogon_model::icnari_conference_db;
using namespace drogon::orm;
using std::vector;

struct speakerStruct{
    int id;
    std::string name;
    std::string specialization;
    std::string profile_picture_object_key;
    std::string description;
    int priority;
    std::string created_at;
};

namespace SpeakerRepository{
    int createSpeaker(const speakerStruct& speaker);
    vector<speakerStruct> getAllSpeakers(int page, int limit);
    speakerStruct getSpeakerById(int id);
    void updateSpeaker(int id, const speakerStruct& speaker);
    bool deleteSpeaker(int id);
}