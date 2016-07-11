#include "RequestHandler.h"

#define MAX_LEN_TOKEN_BUFFER 100
#define BUFFER_SMALL_SIZE 100
#define BUFFER_MSG_SIZE 1000

RequestHandler::RequestHandler(http_message *pMessage, mg_connection *pConnection, std::string shared) :
    connection(pConnection), http_msg(pMessage) {
	server_databases_t *databases = ((server_databases_t *) connection->user_data);
	msgHandler = new MessageHandler(databases);
	msgHandler->setSharedLink(shared);
}

RequestHandler::~RequestHandler() {
	delete msgHandler;
}

void RequestHandler::sendHttpLine(int status_code) {
	sendHttpReply("","",status_code);
}

void RequestHandler::sendHttpReply(std::string reply, std::string content_type, int status) {
	LOGG(DEBUG) << "Sending reply: "<< status;
	mg_printf(connection, "HTTP/1.1 %d\r\n"
                      "Content-Type: %s\r\n"
                      "Content-Length: %d\r\n"
                      "\r\n"
                      "%s", status, content_type.c_str(),
              (int) reply.size(), reply.c_str());
	LOGG(DEBUG) << "Content type: " << content_type;
	LOGG(DEBUG) << "Content length: " << reply.size();
	if (reply.length() > 50){
		LOGG(DEBUG) << "Body: " << reply.substr(0,49) << "...";
	}else{
		LOGG(DEBUG) << "Body: " << reply;
	}

}

bool RequestHandler::validateToken() {
    char buffer[MAX_LEN_TOKEN_BUFFER];
    int parsed = mg_get_http_var(&http_msg->query_string, TOKEN_VARIABLE_NAME, buffer, sizeof(buffer));
    if (!parsed){
    	sendHttpLine(BAD_REQUEST);
    	return false;
    }
    std::string token(buffer);
    LOGG(DEBUG) << "Validating Token for connection: " << token;
    if (! msgHandler->validateToken(token)) {
        this->sendHttpReply("", "", AUTHENTICATION_ERROR);
        return false;
    }
    LOGG(DEBUG) << "Valid token";
    return true;
}

void RequestHandler::rejectConnection(int error_code) {
	LOGG(DEBUG) << "Rejected Connection " << error_code;
    sendHttpLine(error_code);
    connection->flags |= MG_F_SEND_AND_CLOSE;
}

bool RequestHandler::parseAuthorization(string &user, string &pass) {
	char user_[BUFFER_SMALL_SIZE], pass_[BUFFER_SMALL_SIZE];
	
    struct mg_str *hdr = mg_get_http_header(http_msg, AUTHORIZATION_HEADER);
    mg_http_parse_header(hdr, AUTHORIZATION_HEADER_USER, user_, sizeof(user_));
    mg_http_parse_header(hdr, AUTHORIZATION_HEADER_PASS, pass_, sizeof(pass_));


    if (! is_equal(&http_msg->uri, USERS_URI) && ! is_equal(&http_msg->uri, USER_URI)) {
    	LOGG(DEBUG) << "Cannot authenticae to this uri";
    	rejectConnection(NOT_IMPLEMENTED);
        return false;
    }
    user = std::string(user_);
    pass =  std::string(pass_);
    LOGG(DEBUG) << "Auth header parsed for " << user;
    return true;
}

bool RequestHandler::login() {
    	return validateToken();
}

void RequestHandler::listenUserPost() {
    LOGG(DEBUG) << POST_S;
    string user, pass;
    if (! parseAuthorization(user, pass)) {
        return;
    }
    if (! msgHandler->authenticate(user, pass) ) {
        rejectConnection(UNAUTHORIZED);
        return;
    }
    msgHandler->setUser(user);
    LOGG(DEBUG) << "Logged in: " << user;
    char localization[500];
    int parsed = mg_get_http_var(&http_msg->body, USER_LOCATION_TOKEN, localization, sizeof(localization));

    if (parsed <= 0 ){
        sendHttpLine(BAD_REQUEST);
        return;
    }
    bool updated = msgHandler->addLocalization(std::string(localization));
    if (! updated){
        sendHttpLine(BAD_REQUEST);
    }else{
        string token = msgHandler->getToken();
        sendHttpReply(token,CONTENT_TYPE_HEADER_PLAIN,STATUS_OK);
    }
}

void RequestHandler::listenUserGet() {
    LOGG(DEBUG) << GET_S;
    char username[BUFFER_SMALL_SIZE];
    int parsed = mg_get_http_var(&http_msg->query_string, QUERY_STRING_RESOURCE_ID, username, sizeof(username));

    if (parsed <= 0 ) {
        sendHttpLine(BAD_REQUEST);
        return;
    }
    std::string user_data;
    msgHandler->getUser(std::string(username), user_data);

    sendHttpReply(user_data, CONTENT_TYPE_HEADER_CSV, STATUS_OK);
}

void RequestHandler::listenUserRequest() {
	LOGG(DEBUG) << "Listening User Request";
    if (is_equal(&http_msg->method, POST_S)) {
    	listenUserPost();
        return;
    }

    if (! login()) {
    	return;
    }

    if (is_equal(&http_msg->method, GET_S)) {
    	listenUserGet();
        return;
    }

    sendHttpLine(NOT_IMPLEMENTED);
}

void RequestHandler::listenUsersPost() {
    LOGG(DEBUG) << POST_S;
    char user_data[1000];
    string user, pass;
    if (! parseAuthorization(user, pass)) {
        return;
    }
    int parsed = mg_get_http_var(&http_msg->body, BODY_USER, user_data, sizeof(user_data));
    if (parsed <= 0 ) {
        sendHttpLine(BAD_REQUEST);
        return;
    }
    try {
        msgHandler->setUser( user );
        bool created = msgHandler->createUser(std::string(user_data), std::string(pass));
        if (! created){
            LOGG(DEBUG) << "Cannot create user "  << user;
            rejectConnection(BAD_REQUEST);
        }else{
            sendHttpReply(msgHandler->getToken(), CONTENT_TYPE_HEADER_PLAIN, CREATED);
        }

    } catch (ExistentUserException existentUserException) {
        rejectConnection(UNAUTHORIZED);
    }
}

void RequestHandler::listenUsersGet() {
    LOGG(DEBUG) << GET_S;
    std::string users_data;
    msgHandler->getUsers(users_data);
    sendHttpReply(users_data, CONTENT_TYPE_HEADER_CSV, STATUS_OK);
}

void RequestHandler::listenUsersPut() {
    LOGG(DEBUG) << PUT_S;
    char user_data[1000];
    int parsed = mg_get_http_var(&http_msg->body, BODY_USER, user_data, sizeof(user_data));
    if (parsed <= 0 ) {
        LOGG(DEBUG) << "No 'User=' in request, recieved: " << http_msg->body.p;
        sendHttpLine(BAD_REQUEST);
        return;
    }
    bool updated = msgHandler->updateUser(string(user_data));
    int status = (updated) ? STATUS_OK: BAD_REQUEST;
    sendHttpLine(status);
}

void RequestHandler::listenUsersDelete() {
    LOGG(DEBUG) << DELETE_S;
    bool deleted = msgHandler->deleteUser();
    int status = (deleted) ? STATUS_OK: BAD_REQUEST;
    sendHttpLine(status);
}

void RequestHandler::listenUsersRequest() {
	LOGG(DEBUG) << "Listening UserS request";
    if (is_equal(&http_msg->method, POST_S)) {
        listenUsersPost();
    }
    if (! login()) {
    	return;
    }

    if (is_equal(&http_msg->method, GET_S)) {
    	listenUsersGet();
    } else if (is_equal(&http_msg->method, PUT_S)) {
        listenUsersPut();
    } else if (is_equal(&http_msg->method, DELETE_S)) {
        listenUsersDelete();
    } else {
    	rejectConnection(NOT_IMPLEMENTED);
    }

}

void RequestHandler::listenInterestRequest() {
	LOGG(DEBUG) << "Listening Interest request";
    if (! is_equal(&http_msg->method, GET_S)) {
    	sendHttpLine(NOT_IMPLEMENTED);
    }

    if (! login()) {
    	return;
    }

    LOGG(DEBUG) << GET_S;
    char id_interest[BUFFER_SMALL_SIZE];
    int parsed = mg_get_http_var(&http_msg->query_string, QUERY_STRING_RESOURCE_ID, id_interest, sizeof(id_interest));
	if (parsed <= 0 ) {
		sendHttpLine(BAD_REQUEST);
		return;
	}
    std::string interest_photo;
    LOGG(DEBUG) << "searching photo of " << id_interest;
    if (msgHandler->getInterestPhoto(interest_photo, std::string(id_interest))){
    	sendHttpReply(interest_photo, CONTENT_TYPE_HEADER_PLAIN, STATUS_OK);
    }else{
    	this->sendHttpLine(BAD_REQUEST);
    }
    LOGG(DEBUG) << "Searched for photo: "<< interest_photo;
}

void RequestHandler::listenChatRequest() {
	LOGG(DEBUG) << "Listening chat request";
	if (! login()) {
	    	return;
	}

	if ( is_equal(&http_msg->method, GET_S)) {
		this->listenChatGet();
    }else if (is_equal(&http_msg->method, POST_S)){
    	this->listenChatPost();
    }else{
    	sendHttpLine(NOT_IMPLEMENTED);
    }

}

void RequestHandler::listenPhotoGet() {
    LOGG(DEBUG) << GET_S;
    char username[BUFFER_SMALL_SIZE];
    int parsed = mg_get_http_var(&http_msg->query_string, QUERY_STRING_RESOURCE_ID, username, sizeof(username));
    if (parsed <= 0 ) {
    sendHttpLine(BAD_REQUEST);
    return;
    }
    std::string photo_64;
    if (!msgHandler->getPhoto(std::string(username), photo_64)){
    	sendHttpLine(BAD_REQUEST);
    }else{
    	sendHttpReply(photo_64, CONTENT_TYPE_HEADER_PLAIN, STATUS_OK);
    }

}

void RequestHandler::listenPhotoPost() {
    LOGG(DEBUG) << POST_S;
    std::string photo_64(http_msg->body.p, http_msg->body.len);
    bool posted = msgHandler->postPhoto(photo_64);
    int status = (posted) ? STATUS_OK: BAD_REQUEST;
    sendHttpLine(status);
}

void RequestHandler::listenPhotoRequest() {
	LOGG(DEBUG) << "Listening photo request";
    if (! login()) { return; }

    if (is_equal(&http_msg->method, GET_S)) {
        listenPhotoGet();
    } else if (is_equal(&http_msg->method, POST_S)) {
    	listenPhotoPost();
    } else {
        sendHttpLine(NOT_IMPLEMENTED);
    }
}

void RequestHandler::listenChatNewRequest(){
	LOGG(DEBUG) << "Listening new chat request";
	if (! login()) { return; }

	if (is_equal(&http_msg->method, GET_S)) {
		this->listenNewChatGet();
	}else{
		sendHttpLine(NOT_IMPLEMENTED);
	}

}

void RequestHandler::listenMatchesRequest(){
	LOGG(DEBUG)<< "Listening matches request";
	if (! login()) {return;}

	if (is_equal(&http_msg->method, GET_S)) {
		this->listenMatchesGet();
	} else if (is_equal(&http_msg->method, POST_S)) {
		this->listenMatchesPost();
	} else{
		sendHttpLine(NOT_IMPLEMENTED);
	}

}

void RequestHandler::listenChatGet(){
	LOGG(DEBUG)<< GET_S;

	char friend_name[BUFFER_SMALL_SIZE];
	std::string reply;

	if (mg_get_http_var(&http_msg->query_string, QUERY_STRING_RESOURCE_ID, friend_name, sizeof(friend_name)) >= 0) {
		string chatHistory;
		msgHandler->getChat(string(friend_name), chatHistory);
		LOGG(INFO) << "RETURNING CHAT HISTORY: " << chatHistory;
		reply = chatHistory;
	} else {
		string matchesUserData;
		string matchesUserNames;
		msgHandler->getMatches(matchesUserNames);

		istringstream f(matchesUserNames);
		string matchUserName;
		while (std::getline(f, matchUserName)) {
			string matchUserData;
			msgHandler->getUser(matchUserName, matchUserData);
			matchesUserData.append(matchUserData + "\n");
		}
		reply = matchesUserData;
	}
	sendHttpReply(reply, CONTENT_TYPE_HEADER_CSV, STATUS_OK);
}

void RequestHandler::listenChatPost(){
	LOGG(DEBUG)<< POST_S;

	char receiverUserName[BUFFER_SMALL_SIZE], message[BUFFER_MSG_SIZE];
	int parsed = mg_get_http_var(&http_msg->body, BODY_USER_ID, receiverUserName, sizeof(receiverUserName));
	int parsed_msg = mg_get_http_var(&http_msg->body, BODY_MSG, message, sizeof(message));

	if ( !parsed || !parsed_msg){
		LOGG(WARNING) << "BAD CHAT POST REQUEST. ";
		sendHttpLine(BAD_REQUEST);
		return;
	}
	bool posted = msgHandler->postChatMsg(receiverUserName, message);
	int status = (posted) ? STATUS_OK: BAD_REQUEST;
	sendHttpLine(status);
}

void RequestHandler::listenMatchesGet(){
	LOGG(DEBUG)<< GET_S;
	string newMatches;
	bool result_ok = msgHandler->getNewMatches(newMatches);
	if(result_ok){
		sendHttpReply(newMatches, CONTENT_TYPE_HEADER_CSV, STATUS_OK);
	}else{
		sendHttpLine(BAD_REQUEST);
	}
}

void RequestHandler::listenMatchesPost(){
	LOGG(DEBUG)<< POST_S;

	char friend_name[BUFFER_SMALL_SIZE];
	char reaction[10];

	int parsed = mg_get_http_var(&http_msg->body, BODY_USER_ID, friend_name, sizeof(friend_name));
	int parsed_like = mg_get_http_var(&http_msg->body, BODY_LIKE, reaction, sizeof(reaction));

	if ( !parsed || !parsed_like){
		sendHttpLine(BAD_REQUEST);
		return;
	}

	bool posted = msgHandler->postInteraction(friend_name, reaction);
    int status = (posted) ? STATUS_OK: BAD_REQUEST;
    sendHttpLine(status);
}

void RequestHandler::listenNewChatGet(){
	LOGG(DEBUG)<< GET_S;

	string newMessages;
	char friend_name[BUFFER_SMALL_SIZE];

	int parsed = mg_get_http_var(&http_msg->query_string, BODY_USER_ID, friend_name, sizeof(friend_name));

	if ( !parsed){
		LOGG(WARNING) << "BAD GET NEW MESSAGES REQUEST. ";
		sendHttpLine(BAD_REQUEST);
		return;
	}

	bool result_ok = msgHandler->getNewMessages(friend_name, newMessages);
	if(result_ok){
		LOGG(INFO) << "RETURNING NEW MESSAGES: " << newMessages;
		sendHttpReply(newMessages, CONTENT_TYPE_HEADER_CSV, STATUS_OK);
	}else{
		LOGG(WARNING) << "BAD GET NEW MESSAGES REQUEST. ";
		sendHttpLine(BAD_REQUEST);
	}
}
