#include "api_v1_speaker.h"
#include <json/json.h>
#include "../../utils/response.hpp"
#include "../../utils/AppError.hpp"
#include "../../repositories/speakers/speaker.repository.hpp"
#include "../../services/S3Service.h"

using namespace api::v1;

void speaker::createSpeaker(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    try
    {
        speakerStruct newSpeaker;
        newSpeaker.priority = 0;

        // Parse multipart first (browser uploads)
        MultiPartParser fileParser;
        fileParser.parse(req);

        // Extract metadata from JSON if available; otherwise from multipart fields
        const auto speakerData = req->getJsonObject();
        if(speakerData && speakerData->isMember("name"))
        {
            newSpeaker.name = (*speakerData)["name"].asString();
            if(speakerData->isMember("specialization")) newSpeaker.specialization = (*speakerData)["specialization"].asString();
            if(speakerData->isMember("priority")) newSpeaker.priority = (*speakerData)["priority"].asInt();
            if(speakerData->isMember("description")) newSpeaker.description = (*speakerData)["description"].asString();
        }
        else
        {
            const auto params = fileParser.getParameters();
            const auto nameIt = params.find("name");
            if(nameIt == params.end() || nameIt->second.empty())
                throw AppError("Missing required fields: name", k400BadRequest);
            newSpeaker.name = nameIt->second;

            const auto specIt = params.find("specialization");
            if(specIt != params.end()) newSpeaker.specialization = specIt->second;

            const auto descIt = params.find("description");
            if(descIt != params.end()) newSpeaker.description = descIt->second;

            const auto prIt = params.find("priority");
            if(prIt != params.end() && !prIt->second.empty())
            {
                try { newSpeaker.priority = std::stoi(prIt->second); }
                catch(...) { throw AppError("Invalid priority", k400BadRequest); }
            }
        }
        
        // file handling
        if(fileParser.getFiles().empty())
            throw AppError("No file uploaded", k400BadRequest);
        size_t num_of_files = fileParser.getFiles().size();
        if(num_of_files > 1)
            throw AppError("Multiple files uploaded. Only one file is allowed.", k400BadRequest);
        auto file = fileParser.getFiles()[0];
        const std::string fileName=std::to_string(std::time(nullptr))+"_"+ file.getFileName();
        newSpeaker.profile_picture_object_key=putObject(fileName,file);
        int newSpeakerId = SpeakerRepository::createSpeaker(newSpeaker);
        Json::Value responseData;
        responseData["id"] = newSpeakerId;
        callback(Response::success(k200OK, "Speaker created successfully", responseData));
    }
    catch(const std::exception& e)
    {
        LOG_INFO << e.what();
        callback(Response::error(k500InternalServerError, "Internal Server Error"));
    }
}

void speaker::getSpeakers(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    try
    {
        auto queryParams = req->getParameters();
        int page = 1;
        int limit = 10;
        if(queryParams.find("page") != queryParams.end()){
            page = std::stoi(queryParams["page"]);
        }
        if(queryParams.find("limit") != queryParams.end()){
            limit = std::stoi(queryParams["limit"]);
        }
        auto speakers = SpeakerRepository::getAllSpeakers(page, limit);
        Json::Value responseData(Json::arrayValue);
        for(const auto& speaker : speakers){
            Json::Value speakerJson;
            speakerJson["id"] = speaker.id;
            speakerJson["name"] = speaker.name;
            speakerJson["specialization"] = speaker.specialization;
            speakerJson["description"] = speaker.description;
            speakerJson["priority"] = speaker.priority;
            if(!speaker.profile_picture_object_key.empty()){
                speakerJson["profile_picture_url"] = getSignedUrl(speaker.profile_picture_object_key);
            }
            responseData.append(speakerJson);
        }
        callback(Response::success(k200OK, "Speakers retrieved successfully", responseData));
    }
    catch(const std::exception& e)
    {
        LOG_INFO << e.what();
        callback(Response::error(k500InternalServerError, "Internal Server Error"));
    }
}

void speaker::getSpeaker(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int speakerId)
{
    try
    {
        auto speaker = SpeakerRepository::getSpeakerById(speakerId);
        if(speaker.id == 0){
            throw AppError("Speaker not found", k404NotFound);
        }
        Json::Value speakerJson;
        speakerJson["id"] = speaker.id;
        speakerJson["name"] = speaker.name;
        speakerJson["specialization"] = speaker.specialization;
        speakerJson["description"] = speaker.description;
        speakerJson["priority"] = speaker.priority;
        if(!speaker.profile_picture_object_key.empty()){
            speakerJson["profile_picture_url"] = getSignedUrl(speaker.profile_picture_object_key);
        }
        callback(Response::success(k200OK, "Speaker retrieved successfully", speakerJson));
    }
    catch(const std::exception& e)
    {
        LOG_INFO << e.what();
        callback(Response::error(k500InternalServerError, "Internal Server Error"));
    }
}

void speaker::deleteSpeaker(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int speakerId)
{
    try
    {
        // check if speaker exists
        auto speaker = SpeakerRepository::getSpeakerById(speakerId);
        if(speaker.id == 0){
            throw AppError("Speaker not found", k404NotFound);
        }
        deleteObject(speaker.profile_picture_object_key); 
        bool deleted = SpeakerRepository::deleteSpeaker(speakerId);
        if(!deleted){
            throw AppError("Speaker not found", k404NotFound);
        }
        callback(Response::success(k200OK, "Speaker deleted successfully"));
    }
    catch(const std::exception& e)
    {
        LOG_INFO << e.what();
        callback(Response::error(k500InternalServerError, "Internal Server Error"));
    }
}

void speaker::updateSpeaker(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int speakerId)
{
    try
    {
        // Start from existing values to avoid uninitialized fields (priority)
        const auto existingSpeaker = SpeakerRepository::getSpeakerById(speakerId);
        if(existingSpeaker.id == 0) throw AppError("Speaker not found", k404NotFound);

        speakerStruct updatedSpeaker;
        updatedSpeaker.name = "";
        updatedSpeaker.specialization = "";
        updatedSpeaker.profile_picture_object_key = "";
        updatedSpeaker.description = "";
        updatedSpeaker.priority = existingSpeaker.priority;

        const auto speakerData = req->getJsonObject();
        if(speakerData)
        {
            if(speakerData->isMember("name")) updatedSpeaker.name = (*speakerData)["name"].asString();
            if(speakerData->isMember("specialization")) updatedSpeaker.specialization = (*speakerData)["specialization"].asString();
            if(speakerData->isMember("description")) updatedSpeaker.description = (*speakerData)["description"].asString();
            if(speakerData->isMember("priority")) updatedSpeaker.priority = (*speakerData)["priority"].asInt();
        }
        
        // file handling
        MultiPartParser fileParser;
        fileParser.parse(req);

        // If this is a multipart request, allow metadata via form fields too
        const auto params = fileParser.getParameters();
        const auto nameIt = params.find("name");
        if(nameIt != params.end()) updatedSpeaker.name = nameIt->second;
        const auto specIt = params.find("specialization");
        if(specIt != params.end()) updatedSpeaker.specialization = specIt->second;
        const auto descIt = params.find("description");
        if(descIt != params.end()) updatedSpeaker.description = descIt->second;
        const auto prIt = params.find("priority");
        if(prIt != params.end() && !prIt->second.empty())
        {
            try { updatedSpeaker.priority = std::stoi(prIt->second); }
            catch(...) { throw AppError("Invalid priority", k400BadRequest); }
        }

        if(!fileParser.getFiles().empty()){
            size_t num_of_files = fileParser.getFiles().size();
            if(num_of_files > 1)
                throw AppError("Multiple files uploaded. Only one file is allowed.", k400BadRequest);
            auto file = fileParser.getFiles()[0];
            const std::string fileName=std::to_string(std::time(nullptr))+"_"+ file.getFileName();
            updatedSpeaker.profile_picture_object_key=putObject(fileName,file);
        }
        
        SpeakerRepository::updateSpeaker(speakerId, updatedSpeaker);
        callback(Response::success(k200OK, "Speaker updated successfully"));
    }
    catch(const std::exception& e)
    {
        LOG_INFO << e.what();
        callback(Response::error(k500InternalServerError, "Internal Server Error"));
    }
}