#pragma once

#include"../../config/database.h"
#include"../../models/StudentCommittee.h"
#include"../../services/S3Service.h"
#include<vector>

using namespace drogon_model::icnari_conference_db;
using namespace drogon::orm;
using std::vector;

struct studentCommitteeMemberStruct{
    int id;
    std::string name;
    std::string branch;
    std::string profile_picture_object_key;
    int priority;
};

namespace StudentCommitteeRepository {
    vector<studentCommitteeMemberStruct> getCommitteeMembers(int page=1,int limit=10);
    int createCommitteeMember(const studentCommitteeMemberStruct& member);
    void updateCommitteeMember(int id,const studentCommitteeMemberStruct& member);
    bool deleteCommitteeMember(int id);
    studentCommitteeMemberStruct getCommitteeMemberById(int id);
}