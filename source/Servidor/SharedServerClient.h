#ifndef SOURCE_SHAREDSERVERCLIENT_H
#define SOURCE_SHAREDSERVERCLIENT_H


#include <string>
#include <array>
#include "../libs/mongoose/mongoose.h"
#include "api_constants_shared.h"

class SharedServerClient {
    public:
        SharedServerClient();
        ~SharedServerClient();
        void getUsers(std::string* users);
        void getUser(std::string user_id, std::string* user_data);
        void postUsers(std::string user_id, std::string* user_data);
        void postUsersInterests(std::string *interest);
        void getUsersInterests(std::string *interests);
        void deleteUser(std::string user_id);
        void changeUserPhoto(std::string user_id, std::string photo);
        void changeUser(std::string user_id, std::string user_data);
        void run();
    protected:
        static void handleEvent(struct mg_connection* act_connection, int new_event, void* ev_data);
        std::string* connect(std::string url, std::string* post_data);
        std::string formUrl(std::string sub_url[], size_t sub_urls_size);
    private:
        struct mg_mgr manager_;
        struct mg_connection* connection_;
};


#endif //SOURCE_SHAREDSERVERCLIENT_H
