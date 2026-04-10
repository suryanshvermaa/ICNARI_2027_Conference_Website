#include"committee.repository.hpp"

int CommitteeRepository::createCommitteeMember(const committeeMemberStruct& member) {
    Committee committeeMember;
    committeeMember.setName(member.name);
    committeeMember.setSpecialization(member.specialization);
    committeeMember.setProfilePictureObjectKey(member.profile_picture_object_key);
    committeeMember.setCollege(member.college);
    committeeMember.setCommittee(member.committee);
    committeeMember.setPriority(member.priority);
    committeeMember.setDescription(member.description);
    Mapper<Committee> mapper(Database::getClient());
    mapper.insert(committeeMember);
    return committeeMember.getValueOfId();
}

void CommitteeRepository::updateCommitteeMember(int id,const committeeMemberStruct& member) {
    Mapper<Committee> mapper(Database::getClient());
    auto committeeMember = mapper.findByPrimaryKey(member.id);
    if(member.name!="") committeeMember.setName(member.name);
    if(member.specialization!="") committeeMember.setSpecialization(member.specialization);
    if(member.profile_picture_object_key!="") committeeMember.setProfilePictureObjectKey(member.profile_picture_object_key);
    if(member.college!="") committeeMember.setCollege(member.college);
    if(member.committee!="") committeeMember.setCommittee(member.committee);
    if(member.priority!=0) committeeMember.setPriority(member.priority);
    if(member.description!="") committeeMember.setDescription(member.description);
    mapper.update(committeeMember);
}

vector<committeeMemberStruct> CommitteeRepository::getCommitteeMembers(const std::string& committee,int page=1,int limit=10) {
    Mapper<Committee> mapper(Database::getClient());
    auto members = mapper.offset((page-1)*limit).limit(limit).orderBy(Committee::Cols::_priority, SortOrder::ASC).findBy(Criteria(Committee::Cols::_committee, committee));
    vector<committeeMemberStruct> result;
    for (const auto& member : members) {
        committeeMemberStruct memberStruct;
        memberStruct.id = member.getValueOfId();
        memberStruct.name = member.getValueOfName();
        memberStruct.specialization = member.getValueOfSpecialization();
        memberStruct.profile_picture_object_key = member.getValueOfProfilePictureObjectKey();
        memberStruct.college = member.getValueOfCollege();
        memberStruct.committee = member.getValueOfCommittee();
        memberStruct.priority = member.getValueOfPriority();
        memberStruct.description = member.getValueOfDescription();
        result.push_back(memberStruct);
    }
    return result;
}

committeeMemberStruct CommitteeRepository::getCommitteeMemberById(int id) {
    Mapper<Committee> mapper(Database::getClient());
    auto member = mapper.findByPrimaryKey(id);
    committeeMemberStruct memberStruct;
    memberStruct.id = member.getValueOfId();
    memberStruct.name = member.getValueOfName();
    memberStruct.specialization = member.getValueOfSpecialization();
    memberStruct.profile_picture_object_key = member.getValueOfProfilePictureObjectKey();
    memberStruct.college = member.getValueOfCollege();
    memberStruct.committee = member.getValueOfCommittee();
    memberStruct.priority = member.getValueOfPriority();
    memberStruct.description = member.getValueOfDescription();
    return memberStruct;
}

bool CommitteeRepository::deleteCommitteeMember(int id) {
    Mapper<Committee> mapper(Database::getClient());
    auto member = mapper.findByPrimaryKey(id);
    return mapper.deleteByPrimaryKey(id) > 0;
}

bool CommitteeRepository::isValidCommittee(const std::string& committee) {
    return std::find(allowedCommittees.begin(), allowedCommittees.end(), committee) != allowedCommittees.end();
}