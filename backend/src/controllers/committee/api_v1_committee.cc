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
        committeeMemberStruct member;
        // file upload handling + form fields
        MultiPartParser fileParser;
        fileParser.parse(req);
        if(fileParser.getFiles().empty())
            throw AppError("No file uploaded", k400BadRequest);
        size_t num_of_files = fileParser.getFiles().size();
        if(num_of_files > 1)
            throw AppError("Multiple files uploaded. Only one file is allowed.", k400BadRequest);

        // Prefer JSON body when present, otherwise use multipart parameters
        const auto reqBody=req->getJsonObject();
        if(reqBody!=nullptr)
        {
            if(!reqBody->isMember("name") || !reqBody->isMember("committee")|| !reqBody->isMember("college"))
                throw AppError("Missing required fields: name, college, committee", k400BadRequest);
            member.name=(*reqBody)["name"].asString();
            member.college=(*reqBody)["college"].asString();
            if(CommitteeRepository::isValidCommittee((*reqBody)["committee"].asString()))
                throw AppError("Invalid committee name", k400BadRequest);
            member.committee=(*reqBody)["committee"].asString();
            if(reqBody->isMember("specialization")) member.specialization=(*reqBody)["specialization"].asString();
            if(reqBody->isMember("description")) member.description=(*reqBody)["description"].asString();
            if(reqBody->isMember("priority")) member.priority=(*reqBody)["priority"].asInt();
            if(member.committee==CommitteeNames::organizingCommittee){
                if(reqBody->isMember("position")) member.position=(*reqBody)["position"].asString();
                else
                    throw AppError("Missing required field for organizing committee member: position", k400BadRequest);
            }
        }
        else
        {
            const auto params = fileParser.getParameters();
            const auto nameIt = params.find("name");
            const auto collegeIt = params.find("college");
            const auto committeeIt = params.find("committee");
            if(nameIt==params.end() || nameIt->second.empty() || collegeIt==params.end() || collegeIt->second.empty() || committeeIt==params.end() || committeeIt->second.empty())
                throw AppError("Missing required fields: name, college, committee", k400BadRequest);
            member.name = nameIt->second;
            member.college = collegeIt->second;
            if(CommitteeRepository::isValidCommittee(committeeIt->second))
                throw AppError("Invalid committee name", k400BadRequest);
            member.committee = committeeIt->second;

            const auto specIt = params.find("specialization");
            if(specIt!=params.end()) member.specialization = specIt->second;
            const auto descIt = params.find("description");
            if(descIt!=params.end()) member.description = descIt->second;
            const auto prIt = params.find("priority");
            if(prIt!=params.end() && !prIt->second.empty())
            {
                try { member.priority = std::stoi(prIt->second); }
                catch(...) { throw AppError("Invalid priority", k400BadRequest); }
            }
            const auto posIt = params.find("position");
            if(member.committee==CommitteeNames::organizingCommittee){
                if(posIt!=params.end() && !posIt->second.empty()) member.position = posIt->second;
                else throw AppError("Missing required field for organizing committee member: position", k400BadRequest);
            }
        }

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

void committee::getCommitteeMembers(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    try
    {
        auto queryParams=req->getParameters();

        const auto committeeIt = queryParams.find("committee");
        if (committeeIt == queryParams.end() || committeeIt->second.empty())
            throw AppError("Missing required query parameter: committee", k400BadRequest);
        const std::string committee = committeeIt->second;

        if(!CommitteeRepository::isValidCommittee(committee))
            throw AppError("Invalid committee name", k400BadRequest);

        int page=1;
        int limit=10;
        if(queryParams.find("page")!=queryParams.end())
            page=std::stoi(queryParams["page"]);
        if(queryParams.find("limit")!=queryParams.end())
            limit=std::stoi(queryParams["limit"]);
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
            if(member.position!=""){
                memberJson["position"]=member.position;
            }
            response.append(memberJson);
        }
        callback(Response::success(k200OK,"Committee members retrieved successfully",response));
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
        if(member.position!=""){
            memberJson["position"]=member.position;
        }
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
        committeeMemberStruct member=existingMember; // start with existing member data

        // Update from JSON if provided
        auto reqBody=req->getJsonObject();
        if(reqBody!=nullptr)
        {
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
            if(reqBody->isMember("position")) member.position=(*reqBody)["position"].asString();
        }

        // file upload handling
        MultiPartParser fileParser;
        fileParser.parse(req);

        // Also allow multipart form fields (and file-only updates)
        const auto params = fileParser.getParameters();
        const auto nameIt = params.find("name");
        if(nameIt!=params.end()) member.name = nameIt->second;
        const auto collegeIt = params.find("college");
        if(collegeIt!=params.end()) member.college = collegeIt->second;
        const auto committeeIt = params.find("committee");
        if(committeeIt!=params.end() && !committeeIt->second.empty())
        {
            if(CommitteeRepository::isValidCommittee(committeeIt->second))
                throw AppError("Invalid committee name", k400BadRequest);
            member.committee = committeeIt->second;
        }
        const auto specIt = params.find("specialization");
        if(specIt!=params.end()) member.specialization = specIt->second;
        const auto descIt = params.find("description");
        if(descIt!=params.end()) member.description = descIt->second;
        const auto prIt = params.find("priority");
        if(prIt!=params.end() && !prIt->second.empty())
        {
            try { member.priority = std::stoi(prIt->second); }
            catch(...) { throw AppError("Invalid priority", k400BadRequest); }
        }
        const auto posIt = params.find("position");
        if(posIt!=params.end()) member.position = posIt->second;

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
        
        CommitteeRepository::updateCommitteeMember(memberId, member);
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