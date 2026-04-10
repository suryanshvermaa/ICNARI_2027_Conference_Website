#pragma once

#include"../../config/database.h"
#include"../../models/Committee.h"
#include"../../services/S3Service.h"
#include<vector>

using namespace drogon_model::icnari_conference_db;
using namespace drogon::orm;
using std::vector;

namespace CommitteeNames {
    const std::string organizingCommittee = "organizing";
    const std::string technicalCommittee = "technical";
    const std::string programmeCommittee = "programme";
    const std::string internationalCommittee = "international";
    const std::string industryCommittee = "industry";
};
// allowed committees
const vector<std::string> allowedCommittees = {
    CommitteeNames::organizingCommittee,
    CommitteeNames::technicalCommittee,
    CommitteeNames::programmeCommittee,
    CommitteeNames::internationalCommittee,
    CommitteeNames::industryCommittee
};

struct committeeMemberStruct{
    int id;
    std::string name;
    std::string specialization;
    std::string profile_picture_object_key;
    std::string college;
    std::string committee;
    int priority;
    std::string description;
};

namespace CommitteeRepository {
    vector<committeeMemberStruct> getCommitteeMembers(const std::string& committee,int page,int limit);
    int createCommitteeMember(const committeeMemberStruct& member);
    void updateCommitteeMember(int id,const committeeMemberStruct& member);
    bool deleteCommitteeMember(int id);
    committeeMemberStruct getCommitteeMemberById(int id);
    bool isValidCommittee(const std::string& committee);
        
}