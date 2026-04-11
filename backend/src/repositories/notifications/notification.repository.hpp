#include <iostream>
#include <vector>
using std::string;
using std::vector;

static string validTypes[]={"recent_update","highlight"};

struct notification
{
    int id;
    string title;
    string description;
    string type;
    string link;
    int priority;
    string createdAt;
};

namespace NotificationRepository{
    int createNotification(const notification &n);
    bool updateNotification(int id,const notification &n);
    bool deleteNotification(int id);
    vector<notification> getNotifications(int page,int limit,const string &type);
    notification getNotification(int id);
}