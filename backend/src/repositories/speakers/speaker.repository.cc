#include"speaker.repository.hpp"

int SpeakerRepository::createSpeaker(const speakerStruct& s){
    Speakers speaker;
    speaker.setName(s.name);
    speaker.setSpecialization(s.specialization);
    speaker.setProfilePictureObjectKey(s.profile_picture_object_key);
    speaker.setDescription(s.description);
    speaker.setPriority(s.priority?s.priority:0);
    Mapper<Speakers> mapper(Database::getClient());
    mapper.insert(speaker);
    return speaker.getValueOfId();
}

void SpeakerRepository::updateSpeaker(int id, const speakerStruct& s){
    Mapper<Speakers> mapper(Database::getClient());
    Speakers speaker = mapper.findByPrimaryKey(id);
    if(s.name != "") speaker.setName(s.name);
    if(s.specialization != "") speaker.setSpecialization(s.specialization);
    if(s.profile_picture_object_key != "") speaker.setProfilePictureObjectKey(s.profile_picture_object_key);
    if(s.description != "") speaker.setDescription(s.description);
    speaker.setPriority(s.priority);
    mapper.update(speaker);
}

vector<speakerStruct> SpeakerRepository::getAllSpeakers(int page=1, int limit=10){
    Mapper<Speakers> mapper(Database::getClient());
    vector<Speakers> speakers = mapper.paginate(page, limit).findAll();
    vector<speakerStruct> result;
    for(const auto& speaker: speakers){
        speakerStruct s;
        s.id = speaker.getValueOfId();
        s.name = speaker.getValueOfName();
        s.specialization = speaker.getValueOfSpecialization();
        s.profile_picture_object_key = speaker.getValueOfProfilePictureObjectKey();
        s.description = speaker.getValueOfDescription();
        s.priority = speaker.getValueOfPriority();
        s.created_at = speaker.getValueOfCreatedAt().toDbString();
        result.push_back(s);
    }
    return result;
}

speakerStruct SpeakerRepository::getSpeakerById(int id){
    Mapper<Speakers> mapper(Database::getClient());
    Speakers speaker = mapper.findByPrimaryKey(id);
    speakerStruct s;
    s.id = speaker.getValueOfId();
    s.name = speaker.getValueOfName();
    s.specialization = speaker.getValueOfSpecialization();
    s.profile_picture_object_key = speaker.getValueOfProfilePictureObjectKey();
    s.description = speaker.getValueOfDescription();
    s.priority = speaker.getValueOfPriority();
    s.created_at = speaker.getValueOfCreatedAt().toDbString();
    return s;
}

bool SpeakerRepository::deleteSpeaker(int id){
    Mapper<Speakers> mapper(Database::getClient());
    try{
        mapper.deleteByPrimaryKey(id);
        return true;
    }catch(...){
        return false;
    }
}