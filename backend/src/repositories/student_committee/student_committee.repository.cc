#include"student_committee.repository.hpp"

int StudentCommitteeRepository::createCommitteeMember(const studentCommitteeMemberStruct& member) {
    StudentCommittee committeeMember;
    committeeMember.setName(member.name);
    committeeMember.setProfilePictureObjectKey(member.profile_picture_object_key);
    committeeMember.setBranch(member.branch);
    committeeMember.setPriority(member.priority);
    Mapper<StudentCommittee> mapper(Database::getClient());
    mapper.insert(committeeMember);
    return committeeMember.getValueOfId();
}

void StudentCommitteeRepository::updateCommitteeMember(int id,const studentCommitteeMemberStruct& member) {
    Mapper<StudentCommittee> mapper(Database::getClient());
    auto committeeMember = mapper.findByPrimaryKey(member.id);
    if(member.name!="") committeeMember.setName(member.name);
    if(member.branch!="") committeeMember.setBranch(member.branch);
    if(member.profile_picture_object_key!="") committeeMember.setProfilePictureObjectKey(member.profile_picture_object_key);
    if(member.priority!=0) committeeMember.setPriority(member.priority);
    mapper.update(committeeMember);
}

vector<studentCommitteeMemberStruct> StudentCommitteeRepository::getCommitteeMembers(int page=1,int limit=10) {
    Mapper<StudentCommittee> mapper(Database::getClient());
    auto members = mapper.offset((page-1)*limit).limit(limit).orderBy(StudentCommittee::Cols::_priority, SortOrder::ASC).findAll();
    vector<studentCommitteeMemberStruct> result;
    for (const auto& member : members) {
        studentCommitteeMemberStruct memberStruct;
        memberStruct.id = member.getValueOfId();
        memberStruct.name = member.getValueOfName();
        memberStruct.profile_picture_object_key = member.getValueOfProfilePictureObjectKey();
        memberStruct.priority = member.getValueOfPriority();
        result.push_back(memberStruct);
    }
    return result;
}

studentCommitteeMemberStruct StudentCommitteeRepository::getCommitteeMemberById(int id) {
    Mapper<StudentCommittee> mapper(Database::getClient());
    auto member = mapper.findByPrimaryKey(id);
    studentCommitteeMemberStruct memberStruct;
    memberStruct.id = member.getValueOfId();
    memberStruct.name = member.getValueOfName();
    memberStruct.branch = member.getValueOfBranch();
    memberStruct.profile_picture_object_key = member.getValueOfProfilePictureObjectKey();
    memberStruct.priority = member.getValueOfPriority();
    return memberStruct;
}

bool StudentCommitteeRepository::deleteCommitteeMember(int id) {
    Mapper<StudentCommittee> mapper(Database::getClient());
    auto member = mapper.findByPrimaryKey(id);
    return mapper.deleteByPrimaryKey(id) > 0;
}