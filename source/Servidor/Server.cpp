#include "Server.h"

#define DEFAULT_MILISECS_POLL 3000

std::string Server::linkToShared = DEFAULT_SHARED;

Server::Server() : databases(new server_databases_t()) {
    std::string port;
	intToString(DEFAULT_PORT_NUMBER, port);
    mg_mgr_init(&manager_, NULL);
    connection_ = mg_bind(&manager_, port.c_str(), handleEvent);
    mg_set_protocol_http_websocket(connection_);
    mg_enable_multithreading(connection_);
	connection_->user_data = databases;
}

Server::Server(std::string port,std::string sharedlink) : databases(new server_databases_t()) {
	Server::linkToShared = sharedlink;
	mg_mgr_init(&manager_, NULL);
    connection_ = mg_bind(&manager_, port.c_str(), handleEvent);
    mg_set_protocol_http_websocket(connection_);
    mg_enable_multithreading(connection_);
	connection_->user_data = databases;
}

Server::~Server() {
    mg_mgr_free(&manager_);
}

void Server::run() {
	if (! databases->usersDB) {
		//TODO: error
	}
	mg_mgr_poll(&manager_, DEFAULT_MILISECS_POLL);
}

void Server::handleEvent(struct mg_connection* act_connection, int new_event, void* ev_data) {
	struct http_message *http_msg = (struct http_message *) ev_data;
	RequestHandler requestHandler(http_msg, act_connection, linkToShared);

	switch (new_event) {
		case MG_EV_HTTP_REQUEST:
			LOGG(DEBUG) << "Recieved http msg";
			if (is_equal(&http_msg->uri, USER_ID_URI)) {
				requestHandler.listenIdRequest();
			} else if (is_equal(&http_msg->uri, USERS_URI)) {
				requestHandler.listenUsersRequest();
			} else if (is_equal(&http_msg->uri, INTEREST_URI)) {
				requestHandler.listenInterestRequest();
			} else if (is_equal(&http_msg->uri, CHAT_URI)) {
				requestHandler.listenChatRequest();
			} else if (is_equal(&http_msg->uri, USERS_PHOTO_URI)) {
				requestHandler.listenPhotoRequest();
			} else if (is_equal(&http_msg->uri, USER_URI)) {
				requestHandler.listenUserRequest();
			}else{
				requestHandler.sendHttpLine(NOT_IMPLEMENTED);
			}
			break;
		default:
			break;
	}
}

void Server::setUsersDB(rocksdb::DB *database) {
	databases->usersDB = database;
}

void Server::setChatDB(rocksdb::DB *db) {
	databases->chatDB = db;
}

void Server::setLikesDB(rocksdb::DB *database) {
	databases->likesDB = database;
}




