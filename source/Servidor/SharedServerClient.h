#ifndef SOURCE_SHAREDSERVERCLIENT_H
#define SOURCE_SHAREDSERVERCLIENT_H

#include <iostream>
#include <string>
#include <array>
#include "api_constants_shared.h"
#include "../libs/restclient-cpp/include/restclient-cpp/connection.h"
#include "../libs/restclient-cpp/include/restclient-cpp/restclient.h"
#include "../libs/loger/easylogging++.h"
#include "../libs/mongoose/mongoose.h"
#include "Parsers/JsonParser.h"


/*
 * Llamar RestClient::init(); en main para empezar conexiones
 * Llamar RestClient::disable(); en main para terminar conexiones
 * */
#define BAD_REQUEST_STR ""

class SharedServerClient {
    public:
        SharedServerClient(std::string url_to_connect);
        ~SharedServerClient();
        bool getUsers(std::string* users);
        bool getUser(std::string user_id, std::string* user_data);
        bool postUsers(std::string &response, std::string* user_data);
        bool postUsersInterests(std::string *interest);
        bool getUsersInterests(std::string *interests);
        bool deleteUser(std::string user_id, std::string *response);
        bool changeUserPhoto(std::string user_id, std::string photo);
        bool changeUser(std::string user_id, std::string user_data);

    bool getUserPhoto(std::string user_id, std::string &photo_64);

    bool getInterestPhoto(std::string interest_id, std::string &photo_64);

protected:
        bool valid(RestClient::Response *r);
        bool setBody(std::string * body, RestClient::Response *r);
        std::string formUrl(std::string sub_url[], size_t sub_urls_size);
    private:
        RestClient::Connection *conn;

};


#endif //SOURCE_SHAREDSERVERCLIENT_H
