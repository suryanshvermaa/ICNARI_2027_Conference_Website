#include"contact.repository.hpp"

int ContactRepository::createContact(const contactStruct& contact) {
    Contacts newContact;
    newContact.setName(contact.name);
    newContact.setEmail(contact.email);
    newContact.setPhone(contact.phone);
    newContact.setSubject(contact.subject);
    newContact.setMessage(contact.message);
    Mapper<Contacts> mapper(Database::getClient());
    mapper.insert(newContact);
    return newContact.getValueOfId();
}

vector<contactStruct> ContactRepository::getContacts(int page=1, int limit=10) {
    Mapper<Contacts> mapper(Database::getClient());
    std::vector<Contacts> contacts = mapper.offset((page - 1) * limit).limit(limit).findAll();
    std::vector<contactStruct> result;
    for (const auto& contact : contacts) {
        contactStruct c;
        c.id = contact.getValueOfId();
        c.name = contact.getValueOfName();
        c.email = contact.getValueOfEmail();
        c.phone = contact.getValueOfPhone();
        c.subject = contact.getValueOfSubject();
        c.message = contact.getValueOfMessage();
        c.created_at = contact.getValueOfCreatedAt().toDbString();
        result.push_back(c);
    }
    return result;
}

contactStruct ContactRepository::getContactById(int id) {
    Mapper<Contacts> mapper(Database::getClient());
    auto contact = mapper.findByPrimaryKey(id);
    contactStruct c;
    c.id = contact.getValueOfId();
    c.name = contact.getValueOfName();
    c.email = contact.getValueOfEmail();
    c.phone = contact.getValueOfPhone();
    c.subject = contact.getValueOfSubject();
    c.message = contact.getValueOfMessage();
    c.created_at = contact.getValueOfCreatedAt().toDbString();
    return c;
}

bool ContactRepository::deleteContact(int id) {
    Mapper<Contacts> mapper(Database::getClient());
    auto contact = mapper.findByPrimaryKey(id); // Check if the contact exists
    if (!contact.getId()) {
        return false; // Contact does not exist or has already been deleted
    }
    mapper.deleteByPrimaryKey(id);
    return true;
}