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
        c.name = contact.getName();
        c.email = contact.getEmail();
        c.phone = contact.getPhone();
        c.subject = contact.getSubject();
        c.message = contact.getMessage();
        c.created_at = contact.getCreatedAt();
        result.push_back(c);
    }
    return result;
}