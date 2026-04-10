#include "api_v1_committee.h"
#include <json/json.h>
#include "../../utils/response.hpp"
#include "../../utils/AppError.hpp"
#include "../../repositories/committee/committee.repository.hpp"
#include "../../services/S3Service.h"

using namespace api::v1;

void committee::createCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    try
    {
        auto reqBody=req->getJsonObject();
        if(reqBody==nullptr)
            throw AppError("Invalid request body", k400BadRequest);
        if(!reqBody->isMember("name") || !reqBody->isMember("committee")|| !reqBody->isMember("college"))
            throw AppError("Missing required fields: name, college, committee", k400BadRequest);

        committeeMemberStruct member;
        member.name=(*reqBody)["name"].asString();
        member.college=(*reqBody)["college"].asString();
        if(CommitteeRepository::isValidCommittee((*reqBody)["committee"].asString()))
            throw AppError("Invalid committee name", k400BadRequest);
        member.committee=(*reqBody)["committee"].asString();
        if(reqBody->isMember("specialization")) member.specialization=(*reqBody)["specialization"].asString();
        if(reqBody->isMember("description")) member.description=(*reqBody)["description"].asString();
        if(reqBody->isMember("priority")) member.priority=(*reqBody)["priority"].asInt();

        // file upload handling
        MultiPartParser fileParser;
        fileParser.parse(req);
        if(fileParser.getFiles().empty())
            throw AppError("No file uploaded", k400BadRequest);
        size_t num_of_files = fileParser.getFiles().size();
        if(num_of_files > 1)
            throw AppError("Multiple files uploaded. Only one file is allowed.", k400BadRequest);
        auto file=fileParser.getFiles()[0];
        const std::string fileName=std::to_string(std::time(nullptr))+"_"+file.getFileName()+"_"; // to avoid name collision
        member.profile_picture_object_key=putObject(fileName, file);
        int newId=CommitteeRepository::createCommitteeMember(member);
        Json::Value response;
        response["id"]=newId;
        callback(Response::success(k201Created,"Committee member created successfully",response));
    }
    catch(const AppError& e)
    {
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(...)
    {
        callback(Response::error(k500InternalServerError, "An unexpected error occurred"));
    }
}

void committee::getCommitteeMembers(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,std::string committee){
    try
    {
        if(!CommitteeRepository::isValidCommittee(committee))
            throw AppError("Invalid committee name", k400BadRequest);
        auto queryParams=req->getParameters();
        int page=1;
        int limit=10;
        if(queryParams.find("page")!=queryParams.end())
            page=std::stoi(queryParams["page"]);
        if(queryParams.find("limit")!=queryParams.end())
            limit=std::stoi(queryParams["limit"]);
        if(!CommitteeRepository::isValidCommittee(committee))
            throw AppError("Invalid committee name", k400BadRequest);
        auto members=CommitteeRepository::getCommitteeMembers(committee,page,limit);
        Json::Value response(Json::arrayValue);
        for(const auto& member:members){
            Json::Value memberJson;
            memberJson["id"]=member.id;
            memberJson["name"]=member.name;
            memberJson["specialization"]=member.specialization;
            memberJson["profile_picture_url"]=getSignedUrl(member.profile_picture_object_key);
            memberJson["college"]=member.college;
            memberJson["committee"]=member.committee;
            memberJson["priority"]=member.priority;
            memberJson["description"]=member.description;
            response.append(memberJson);
        }
        callback(Response::success(k200OK,"Committee members retrieved successfully",response));
    }
    catch(const AppError& e)
    {
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(...)
    {
        callback(Response::error(k500InternalServerError, "An unexpected error occurred"));
    }
}


void committee::getCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId){
    try
    {
        auto member=CommitteeRepository::getCommitteeMemberById(memberId);
        if(member.id==0)
            throw AppError("Committee member not found", k404NotFound);
        Json::Value memberJson;
        memberJson["id"]=member.id;
        memberJson["name"]=member.name;
        memberJson["specialization"]=member.specialization;
        memberJson["profile_picture_url"]=getSignedUrl(member.profile_picture_object_key);
        memberJson["college"]=member.college;
        memberJson["committee"]=member.committee;
        memberJson["priority"]=member.priority;
        memberJson["description"]=member.description;
        callback(Response::success(k200OK,"Committee member retrieved successfully",memberJson));
    }
    catch(const AppError& e)
    {
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(...)
    {
        callback(Response::error(k500InternalServerError, "An unexpected error occurred"));
    }
}

void committee::deleteCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId){
    try
    {
        committeeMemberStruct member=CommitteeRepository::getCommitteeMemberById(memberId);
        if(member.id==0)            throw AppError("Committee member not found", k404NotFound);
        deleteObject(member.profile_picture_object_key); // delete profile picture from S3
        if(!CommitteeRepository::deleteCommitteeMember(memberId))
            throw AppError("Committee member not found", k404NotFound);
        callback(Response::success(k200OK,"Committee member deleted successfully"));
    }
    catch(const AppError& e)
    {
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(...)
    {
        callback(Response::error(k500InternalServerError, "An unexpected error occurred"));
    }
}

void committee::updateCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId){
    try
    {
        auto existingMember=CommitteeRepository::getCommitteeMemberById(memberId);
        if(existingMember.id==0)
            throw AppError("Committee member not found", k404NotFound);
        auto reqBody=req->getJsonObject();
        if(reqBody==nullptr)
            throw AppError("Invalid request body", k400BadRequest);
        committeeMemberStruct member=existingMember; // start with existing member data
        if(reqBody->isMember("name")) member.name=(*reqBody)["name"].asString();
        if(reqBody->isMember("college")) member.college=(*reqBody)["college"].asString();
        if(reqBody->isMember("committee")){
            if(CommitteeRepository::isValidCommittee((*reqBody)["committee"].asString()))
                throw AppError("Invalid committee name", k400BadRequest);
            member.committee=(*reqBody)["committee"].asString();
        }
        if(reqBody->isMember("specialization")) member.specialization=(*reqBody)["specialization"].asString();
        if(reqBody->isMember("description")) member.description=(*reqBody)["description"].asString();
        if(reqBody->isMember("priority")) member.priority=(*reqBody)["priority"].asInt();

        // file upload handling
        MultiPartParser fileParser;
        fileParser.parse(req);
        if(!fileParser.getFiles().empty()){
            size_t num_of_files = fileParser.getFiles().size();
            if(num_of_files > 1)
                throw AppError("Multiple files uploaded. Only one file is allowed.", k400BadRequest);
            auto file=fileParser.getFiles()[0];
            const std::string fileName=std::to_string(std::time(nullptr))+"_"+file.getFileName()+"_"; // to avoid name collision
            std::string newProfilePictureKey=putObject(fileName, file);
            deleteObject(existingMember.profile_picture_object_key); // delete old profile picture from S3
            member.profile_picture_object_key=newProfilePictureKey;
        }
        
        if(!CommitteeRepository::updateCommitteeMember(member))
            throw AppError("Failed to update committee member", k500InternalServerError);
        
        callback(Response::success(k200OK,"Committee member updated successfully"));
    }
    catch(const AppError& e){
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(...)
    {
        callback(Response::error(k500InternalServerError, "An unexpected error occurred"));
    }
}