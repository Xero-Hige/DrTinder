#ifndef SOURCE_REQUESTHANDLER_H
#define SOURCE_REQUESTHANDLER_H


#include "MessageHandler.h"
#include <cstring>
#include "ServerUtils.h"


class RequestHandler {
public:
    RequestHandler(http_message *pMessage, mg_connection *pConnection);
    RequestHandler(http_message *pMessage, mg_connection *pConnection, std::string sharedLink);
    ~RequestHandler();

    bool validateToken();

    void listenUsersRequest();

    void listenIdRequest();

    void listenInterestRequest();

    void listenChatRequest();

    void listenPhotoRequest();

    void sendHttpLine(int status_code);

    void sendHttpReply(std::string reply, std::string content_type,int code);

    void rejectConnection(int error_code);

    void listenUserRequest();

private:
    struct mg_connection* connection;
    MessageHandler* msgHandler;
    struct http_message* http_msg;
    bool parseAuthorization(string &user, string &pass);

    bool login();

    void listenUserPost();

    void listenUserGet();

    void listenUsersPost();

    void listenUsersGet();

    void listenUsersPut();

    void listenUsersDelete();

    void listenPhotoGet();

    void listenPhotoPost();
};


#endif //SOURCE_REQUESTHANDLER_H
