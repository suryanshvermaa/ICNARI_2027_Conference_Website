#include "api_v1_student_committee.h"
#include <json/json.h>
#include "../../utils/response.hpp"
#include "../../utils/AppError.hpp"
#include "../../repositories/student_committee/student_committee.repository.hpp"
#include "../../services/S3Service.h"

using namespace api::v1;

void student_committee::createCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    try
    {
        studentCommitteeMemberStruct member;
        MultiPartParser fileParser;
        fileParser.parse(req);
        if(fileParser.getFiles().size() > 1)
            throw AppError("Multiple files uploaded. Only one file is allowed.", k400BadRequest);

        const auto reqBody=req->getJsonObject();
        if(reqBody!=nullptr)
        {
            if(!reqBody->isMember("name") || (*reqBody)["name"].asString().empty())
                throw AppError("Missing required field: name", k400BadRequest);
            member.name=(*reqBody)["name"].asString();
            if(reqBody->isMember("branch")) member.branch=(*reqBody)["branch"].asString();
            if(reqBody->isMember("priority")) member.priority=(*reqBody)["priority"].asInt();
        }
        else
        {
            const auto params = fileParser.getParameters();
            const auto nameIt = params.find("name");
            const auto branchIt = params.find("branch");
            if(nameIt==params.end() || nameIt->second.empty())
                throw AppError("Missing required field: name", k400BadRequest);
            member.name = nameIt->second;
            if(branchIt!=params.end()) member.branch = branchIt->second;

            const auto prIt = params.find("priority");
            if(prIt!=params.end() && !prIt->second.empty())
            {
                try { member.priority = std::stoi(prIt->second); }
                catch(...) { throw AppError("Invalid priority", k400BadRequest); }
            }
        }

        if(!fileParser.getFiles().empty())
        {
            auto file=fileParser.getFiles()[0];
            const std::string fileName=std::to_string(std::time(nullptr))+"_"+file.getFileName()+"_";
            member.profile_picture_object_key=putObject(fileName, file);
        }
        int newId=StudentCommitteeRepository::createCommitteeMember(member);
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

void student_committee::getCommitteeMembers(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    try
    {
        auto queryParams=req->getParameters();

        int page=1;
        int limit=10;
        if(queryParams.find("page")!=queryParams.end())
            page=std::stoi(queryParams["page"]);
        if(queryParams.find("limit")!=queryParams.end())
            limit=std::stoi(queryParams["limit"]);
        if(page < 1 || limit < 1)
            throw AppError("Invalid page/limit", k400BadRequest);
        auto members=StudentCommitteeRepository::getCommitteeMembers(page,limit);
        Json::Value response(Json::arrayValue);
        for(const auto& member:members){
            Json::Value memberJson;
            memberJson["id"]=member.id;
            memberJson["name"]=member.name;
            memberJson["branch"]=member.branch;
            if(!member.profile_picture_object_key.empty())
                memberJson["profile_picture_url"]=getSignedUrl(member.profile_picture_object_key);
            memberJson["priority"]=member.priority;
            response.append(memberJson);
        }
        callback(Response::success(k200OK,"Student committee members retrieved successfully",response));
    }
    catch (const std::invalid_argument&)
    {
        callback(Response::error(k400BadRequest, "Invalid page/limit"));
    }
    catch (const std::out_of_range&)
    {
        callback(Response::error(k400BadRequest, "Invalid page/limit"));
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


void student_committee::getCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId){
    try
    {
        auto member=StudentCommitteeRepository::getCommitteeMemberById(memberId);
        if(member.id==0)
            throw AppError("Committee member not found", k404NotFound);
        Json::Value memberJson;
        memberJson["id"]=member.id;
        memberJson["name"]=member.name;
        memberJson["branch"]=member.branch;
        if(!member.profile_picture_object_key.empty())
            memberJson["profile_picture_url"]=getSignedUrl(member.profile_picture_object_key);
        memberJson["priority"]=member.priority;
        callback(Response::success(k200OK,"Student committee member retrieved successfully",memberJson));
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

void student_committee::deleteCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId){
    try
    {
        studentCommitteeMemberStruct member=StudentCommitteeRepository::getCommitteeMemberById(memberId);
        if(member.id==0)            throw AppError("Committee member not found", k404NotFound);
        deleteObject(member.profile_picture_object_key); // delete profile picture from S3
        if(!StudentCommitteeRepository::deleteCommitteeMember(memberId))
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

void student_committee::updateCommitteeMember(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,int memberId){
    try
    {
        auto existingMember=StudentCommitteeRepository::getCommitteeMemberById(memberId);
        if(existingMember.id==0)
            throw AppError("Committee member not found", k404NotFound);
        studentCommitteeMemberStruct member=existingMember;

        // Update from JSON if provided
        auto reqBody=req->getJsonObject();
        if(reqBody!=nullptr)
        {
            if(reqBody->isMember("name")) member.name=(*reqBody)["name"].asString();
            if(reqBody->isMember("branch")) member.branch=(*reqBody)["branch"].asString();
            if(reqBody->isMember("priority")) member.priority=(*reqBody)["priority"].asInt();
        }

        MultiPartParser fileParser;
        fileParser.parse(req);

        const auto params = fileParser.getParameters();
        const auto nameIt = params.find("name");
        if(nameIt!=params.end()) member.name = nameIt->second;
        const auto branchIt = params.find("branch");
        if(branchIt!=params.end()) member.branch = branchIt->second;
        const auto prIt = params.find("priority");
        if(prIt!=params.end() && !prIt->second.empty())
        {
            try { member.priority = std::stoi(prIt->second); }
            catch(...) { throw AppError("Invalid priority", k400BadRequest); }
        }
        if(!fileParser.getFiles().empty()){
            size_t num_of_files = fileParser.getFiles().size();
            if(num_of_files > 1)
                throw AppError("Multiple files uploaded. Only one file is allowed.", k400BadRequest);
            auto file=fileParser.getFiles()[0];
            const std::string fileName=std::to_string(std::time(nullptr))+"_"+file.getFileName()+"_";
            std::string newProfilePictureKey=putObject(fileName, file);
            deleteObject(existingMember.profile_picture_object_key);
            member.profile_picture_object_key=newProfilePictureKey;
        }
        
        StudentCommitteeRepository::updateCommitteeMember(memberId, member);
        callback(Response::success(k200OK,"Student committee member updated successfully"));
    }
    catch(const AppError& e){
        callback(Response::error(e.statusCode, e.what()));
    }
    catch(...)
    {
        callback(Response::error(k500InternalServerError, "An unexpected error occurred"));
    }
}