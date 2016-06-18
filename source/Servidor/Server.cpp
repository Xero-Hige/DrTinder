#include "Server.h"

#define DEFAULT_MILISECS_POLL 3000
#define MAX_LEN_TOKEN_BUFFER 100

void intToString(int number, std::string& result) {
    std::ostringstream convert;
    convert << number;
    result = convert.str();
}

static int is_equal(const struct mg_str *s1, const struct mg_str *s2) {
	return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}

Server::Server() : usersDB(NULL) {
    std::string port;
	intToString(DEFAULT_PORT_NUMBER, port);
	
    mg_mgr_init(&manager_, NULL);
    connection_ = mg_bind(&manager_, port.c_str(), handleEvent);
    mg_set_protocol_http_websocket(connection_);
}

Server::~Server() {
    mg_mgr_free(&manager_);
}

void Server::run() {
	if (! usersDB) {
		//TODO: error
	}
	mg_mgr_poll(&manager_, DEFAULT_MILISECS_POLL);
}

void Server::sendHttpLine(struct mg_connection* connection, int status_code) {
	mg_send_response_line(connection, status_code, NULL);
}

void Server::sendHttpReply(struct mg_connection* connection, std::string reply, std::string content_type) {
	mg_printf(connection, "HTTP/1.1 200 OK\r\n"
					  "Content-Type: %s\r\n"
					  "Content-Length: %d\r\n"
					  "\r\n"
					  "%s", content_type.c_str(),
			  (int) reply.length(), reply.c_str());
}

bool Server::validateToken(MessageHandler* msgHandler, struct http_message* http_msg) {
	char buffer[MAX_LEN_TOKEN_BUFFER];
	mg_get_http_var(&http_msg->body, TOKEN_VARIABLE_NAME, buffer, sizeof(buffer));
	std::string token(buffer);
	return msgHandler->validateToken(token);
}

MessageHandler* Server::acceptConnection(struct http_message* http_msg, struct mg_connection* connection) {
	char user[20], pass[20];
	struct mg_str *hdr = mg_get_http_header(http_msg, "Authorization");
	mg_http_parse_header(hdr, "username", user, sizeof(user));
	mg_http_parse_header(hdr, "pass", pass, sizeof(pass));

	rocksdb::DB *usersDB = (rocksdb::DB *) connection->user_data;
	DatabaseManager *usersDBM = new DatabaseManager(usersDB);
	MessageHandler* msgHandler = new MessageHandler(usersDBM, std::string(user), std::string(pass));
	if (! msgHandler->authenticate(std::string(user), std::string(pass))) {
		delete msgHandler;
		sendHttpLine(connection, AUTHENTICATION_ERROR);
		connection->flags |= MG_F_SEND_AND_CLOSE;
		return NULL;
	}

	connection->user_data = msgHandler;
	return msgHandler;
}

void Server::listenUsersRequest(struct http_message* http_msg, struct mg_connection* connection) {
	const struct mg_str s_get_method = MG_MK_STR(GET_S);
	const struct mg_str s_post_method = MG_MK_STR(POST_S);
	const struct mg_str s_put_method = MG_MK_STR(PUT_S);
	const struct mg_str s_delete_method = MG_MK_STR(DELETE_S);

	MessageHandler* msgHandler = (MessageHandler *) connection->user_data;

	if (is_equal(&http_msg->method, &s_post_method)) {
		std::string user_data(http_msg->body.p, http_msg->body.len);
		msgHandler->createUser(user_data);
		return;
	}

	if (! validateToken(msgHandler, http_msg)) {
		sendHttpLine(connection, AUTHENTICATION_ERROR);
	}

	if (is_equal(&http_msg->method, &s_get_method)) {
		std::string users_data;
		msgHandler->getUsers(users_data);
		sendHttpReply(connection, users_data, "text");
	} else if (is_equal(&http_msg->method, &s_put_method)) {
		std::string user_data(http_msg->body.p, http_msg->body.len);
		msgHandler->updateUser(user_data);
	} else if (is_equal(&http_msg->method, &s_delete_method)) {
		msgHandler->deleteUser();
	} else {
		sendHttpLine(connection, BAD_REQUEST);
	}

}

void Server::listenIdRequest(struct http_message* http_msg, struct mg_connection* connection) {
	const struct mg_str s_get_method = MG_MK_STR(GET_S);
	if (! is_equal(&http_msg->method, &s_get_method)) {
		sendHttpLine(connection, BAD_REQUEST);
		return;
	}
	MessageHandler* msgHandler = (MessageHandler *) connection->user_data;
	sendHttpReply(connection, msgHandler->getToken(), TOKEN_VARIABLE_NAME);
}

void Server::listenFriendsRequest(struct http_message* http_msg, struct mg_connection* connection) {
	const struct mg_str s_get_method = MG_MK_STR(GET_S);
	if (! is_equal(&http_msg->method, &s_get_method)) {
		sendHttpLine(connection, BAD_REQUEST);
	}
	std::string matches;
	MessageHandler* msgHandler = (MessageHandler *) connection->user_data;
	msgHandler->getMatches(matches);
	sendHttpReply(connection, matches, "text");
}

void Server::listenInterestRequest(struct http_message* http_msg, struct mg_connection* connection) {
	const struct mg_str s_get_method = MG_MK_STR(GET_S);
	if (! is_equal(&http_msg->method, &s_get_method)) {
		sendHttpLine(connection, BAD_REQUEST);
	}
	std::string interest_photo;
	MessageHandler* msgHandler = (MessageHandler *) connection->user_data;
	msgHandler->getInterest(interest_photo);
	sendHttpReply(connection, interest_photo, "image");
}

void Server::listenChatRequest(struct http_message* http_msg, struct mg_connection* connection) {
	char friend_name[20];
	mg_get_http_var(&http_msg->query_string, "friend", friend_name, sizeof(friend_name));
	std::string chat_history;
	MessageHandler* msgHandler = (MessageHandler *) connection->user_data;
	msgHandler->getChat(chat_history);
	//TODO: enviar historial
}

void Server::listenPhotoRequest(struct http_message* http_msg, struct mg_connection* connection) {
	const struct mg_str s_get_method = MG_MK_STR(GET_S);
	const struct mg_str s_post_method = MG_MK_STR(POST_S);

	MessageHandler* msgHandler = (MessageHandler *) connection->user_data;

	if (is_equal(&http_msg->method, &s_get_method)) {
		std::string photo_64;
		msgHandler->getPhoto(photo_64);
		sendHttpReply(connection, photo_64, "image");
	} else if (is_equal(&http_msg->method, &s_post_method)) {
		std::string photo_64(http_msg->body.p, http_msg->body.len);
		msgHandler->postPhoto(photo_64);
	} else {
		sendHttpLine(connection, BAD_REQUEST);
	}
}

void Server::handleEvent(struct mg_connection* act_connection, int new_event, void* ev_data) {
	MessageHandler* msgHandler;
	struct http_message *http_msg = (struct http_message *) ev_data;

	switch (new_event) {
		case MG_EV_ACCEPT:
			msgHandler = acceptConnection(http_msg, act_connection);
			break;
		case MG_EV_HTTP_REQUEST:
			if (mg_vcmp(&http_msg->uri, USER_ID_URI) == 0) {
				listenIdRequest(http_msg, act_connection);
			} else if (mg_vcmp(&http_msg->uri, USERS_URI) == 0) {
				listenUsersRequest(http_msg, act_connection);
			}
			msgHandler = (MessageHandler *) act_connection->user_data;
			if (! validateToken(msgHandler, http_msg)) {
				sendHttpLine(act_connection, AUTHENTICATION_ERROR);
			}

			if (mg_vcmp(&http_msg->uri, FRIENDS_URI) == 0) {
				listenFriendsRequest(http_msg, act_connection);
			} else if (mg_vcmp(&http_msg->uri, INTEREST_URI) == 0) {
				listenInterestRequest(http_msg, act_connection);
			} else if (mg_vcmp(&http_msg->uri, CHAT_URI) == 0) {
				listenChatRequest(http_msg, act_connection);
			} else if (mg_vcmp(&http_msg->uri, USERS_PHOTO_URI) == 0) {
				listenPhotoRequest(http_msg, act_connection);
			}
			break;
		default:
			break;
	}
}

void Server::setUsersDB(rocksdb::DB *database) {
	this->usersDB = database;
	connection_->user_data = database;
}


