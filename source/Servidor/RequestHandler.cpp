#include "RequestHandler.h"

#define MAX_LEN_TOKEN_BUFFER 100
#define BUFFER_SMALL_SIZE 20


RequestHandler::RequestHandler(http_message *pMessage, mg_connection *pConnection) :
    connection(pConnection), http_msg(pMessage) {
}

RequestHandler::~RequestHandler() {
}

void RequestHandler::sendHttpLine(int status_code) {
    mg_send_response_line(connection, status_code, NULL);
}

void RequestHandler::sendHttpReply(std::string reply, std::string content_type) {
    mg_printf(connection, "HTTP/1.1 %d\r\n"
                      "Content-Type: %s\r\n"
                      "Content-Length: %d\r\n"
                      "\r\n"
                      "%s", STATUS_OK, content_type.c_str(),
              (int) reply.length(), reply.c_str());
}

bool RequestHandler::validateToken() {
    char buffer[MAX_LEN_TOKEN_BUFFER];
    mg_get_http_var(&http_msg->query_string, TOKEN_VARIABLE_NAME, buffer, sizeof(buffer));
    std::string token(buffer);
    if (! msgHandler->validateToken(token)) {
        sendHttpLine(INVALID_TOKEN);
        LOGG(INFO) << "Token expired";
        return false;
    }
    return true;
}

void RequestHandler::rejectConnection(int error_code) {
    sendHttpLine(error_code);
    connection->flags |= MG_F_SEND_AND_CLOSE;
}


bool RequestHandler::parseAuthorization(char* user, char* pass) {
    struct mg_str *hdr = mg_get_http_header(http_msg, AUTHORIZATION_HEADER);
    mg_http_parse_header(hdr, AUTHORIZATION_HEADER_USER, user, sizeof(user));
    mg_http_parse_header(hdr, AUTHORIZATION_HEADER_PASS, pass, sizeof(pass));

    server_databases_t *databases = ((server_databases_t *) connection->user_data);
    msgHandler = new MessageHandler(databases, std::string(user));
    connection->user_data = msgHandler;

    if (! is_equal(&http_msg->uri, USERS_URI) && ! is_equal(&http_msg->uri, USER_URI)) {
        rejectConnection(NOT_IMPLEMENTED);
        delete msgHandler;
        return false;
    }
    LOGG(INFO) << "Nueva conexion exitosa";
    return true;
}

bool RequestHandler::login() {
    msgHandler = (MessageHandler *) connection->user_data;
    return validateToken();
}

void RequestHandler::listenUserRequest() {
    if (is_equal(&http_msg->method, POST_S)) {
        char user[BUFFER_SMALL_SIZE], pass[BUFFER_SMALL_SIZE];
        if (! parseAuthorization(user, pass)) {
            return;
        }
        if (! msgHandler->authenticate(std::string(user), std::string(pass))) {
            rejectConnection(UNAUTHORIZED);
            delete msgHandler;
        }
        char localization[500];
        mg_get_http_var(&http_msg->body, USER_LOCATION_TOKEN, localization, sizeof(localization));
        msgHandler->addLocalization(std::string(localization));
        return;
    }
    if (! login()) {
    	rejectConnection(UNAUTHORIZED);
    	return; }

    if (is_equal(&http_msg->method, GET_S)) {
        char username[BUFFER_SMALL_SIZE];
        mg_get_http_var(&http_msg->query_string, QUERY_STRING_USER, username, sizeof(username));
        std::string user_data;
        msgHandler->getUser(std::string(username), user_data);
        sendHttpReply(user_data, CONTENT_TYPE_HEADER_CSV);
        return;
    }

    sendHttpLine(NOT_IMPLEMENTED);
}

void RequestHandler::listenUsersRequest() {
    if (is_equal(&http_msg->method, POST_S)) {
        char user_data[1000], user[BUFFER_SMALL_SIZE], pass[BUFFER_SMALL_SIZE];
        if (! parseAuthorization(user, pass)) {
            return;
        }
        mg_get_http_var(&http_msg->body, BODY_USER, user_data, sizeof(user_data));
        try {
            msgHandler->createUser(std::string(user_data), std::string(pass));
            sendHttpReply(user_data, CONTENT_TYPE_HEADER_CSV);
        } catch (ExistentUserException existentUserException) {
            rejectConnection(UNAUTHORIZED);
            delete msgHandler;
        }
        return;
    }
    if (! login()) {
    	rejectConnection(UNAUTHORIZED);
    	return; }

    if (is_equal(&http_msg->method, GET_S)) {

        std::string users_data;
        msgHandler->getUsers(users_data);
        sendHttpReply(users_data, CONTENT_TYPE_HEADER_CSV);

    } else if (is_equal(&http_msg->method, PUT_S)) {

        char user_data[1000];
        mg_get_http_var(&http_msg->body, BODY_USER, user_data, sizeof(user_data));
        bool updated = msgHandler->updateUser(string(user_data));
        int status = (updated) ? STATUS_OK: BAD_REQUEST;
        sendHttpLine(status);

    } else if (is_equal(&http_msg->method, DELETE_S)) {

        bool deleted = msgHandler->deleteUser();
        int status = (deleted) ? STATUS_OK: BAD_REQUEST;
        sendHttpLine(status);

    } else {
        sendHttpLine(NOT_IMPLEMENTED);
    }

}

void RequestHandler::listenIdRequest() {
    if (! is_equal(&http_msg->method, GET_S)) {
        sendHttpLine(NOT_IMPLEMENTED);
        return;
    }
    msgHandler = (MessageHandler *) connection->user_data;
    sendHttpReply(msgHandler->getToken(), TOKEN_VARIABLE_NAME);
}

void RequestHandler::listenInterestRequest() {
    if (! is_equal(&http_msg->method, GET_S)) {
        sendHttpLine(NOT_IMPLEMENTED);
    }
    if (! login()) {
    	sendHttpLine(UNAUTHORIZED);
    	return; }

    char id_interest[BUFFER_SMALL_SIZE];
    mg_get_http_var(&http_msg->query_string, QUERY_STRING_RESOURCE_ID, id_interest, sizeof(id_interest));

    std::string interest_photo;
    msgHandler->getInterestPhoto(interest_photo, std::string(id_interest));
    sendHttpReply(interest_photo, CONTENT_TYPE_HEADER_IMAGE);
}

void RequestHandler::listenChatRequest() {
    if (! is_equal(&http_msg->method, GET_S)) {
        sendHttpLine(NOT_IMPLEMENTED);
    }
    char friend_name[BUFFER_SMALL_SIZE];
    std::string reply;
    if (! login()) {
    	sendHttpLine(UNAUTHORIZED);
    	return; }

    if (mg_get_http_var(&http_msg->query_string, QUERY_STRING_RESOURCE_ID, friend_name, sizeof(friend_name)) == 0) {
        msgHandler->getChat(string(friend_name), reply);
    } else {
        msgHandler->getMatches(reply);
    }
    sendHttpReply(reply, CONTENT_TYPE_HEADER_CSV);
}

void RequestHandler::listenPhotoRequest() {
    if (! login()) { return; }

    if (is_equal(&http_msg->method, GET_S)) {
        char username[BUFFER_SMALL_SIZE];
        mg_get_http_var(&http_msg->query_string, QUERY_STRING_USER, username, sizeof(username));

        std::string photo_64;
        msgHandler->getPhoto(std::string(username), photo_64);
        sendHttpReply(photo_64, CONTENT_TYPE_HEADER_IMAGE);
    } else if (is_equal(&http_msg->method, POST_S)) {

    	std::string photo_64(http_msg->body.p, http_msg->body.len);
        bool posted = msgHandler->postPhoto(photo_64);
        int status = (posted) ? STATUS_OK: BAD_REQUEST;
        sendHttpLine(status);

    } else {
        sendHttpLine(NOT_IMPLEMENTED);
    }
}
