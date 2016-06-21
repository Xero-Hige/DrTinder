#ifndef SOURCE_REQUESTHANDLER_H
#define SOURCE_REQUESTHANDLER_H


#include "MessageHandler.h"
#include <cstring>
#include "RequestHandler.h"
#include "ServerUtils.h"


class RequestHandler {
public:
    RequestHandler(http_message *pMessage, mg_connection *pConnection);
    ~RequestHandler();

    bool validateToken();

    void listenUsersRequest();

    void listenIdRequest();

    void listenInterestRequest();

    void listenChatRequest();

    void listenPhotoRequest();

    void sendHttpLine(int status_code);

    void sendHttpReply(std::string reply, std::string content_type);

    void rejectConnection(int error_code);

    void listenUserRequest();

private:
    struct mg_connection* connection;
    MessageHandler* msgHandler;
    struct http_message* http_msg;

    bool parseAuthorization(char *user, char *pass);

    bool login();
};


#endif //SOURCE_REQUESTHANDLER_H
