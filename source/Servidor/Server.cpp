#include "Server.h"

#define DEFAULT_MILISECS_POLL 3000

void intToString(int number, std::string& result) {
    std::ostringstream convert;
    convert << number;
    result = convert.str();
}

Server::Server() : usersDB(NULL) {
    std::string port;
	intToString(DEFAULT_PORT_NUMBER, port);
	
    mg_mgr_init(&manager_, NULL);
    connection_ = mg_bind(&manager_, port.c_str(), handleEvent);
    mg_enable_multithreading(connection_);
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

void Server::handleEvent(struct mg_connection* act_connection, int new_event, void* data) {
	UsersDatabase* usersDB = (UsersDatabase*) act_connection->user_data;

	if (new_event == MG_EV_RECV) {
		struct mbuf *io = &act_connection->recv_mbuf;
		
		std::string reply;
		MessageHandler msgHandler(usersDB);
		msgHandler.parse(io->buf, reply);

		mg_send(act_connection, reply.c_str(), reply.size());
		mbuf_remove(io, io->len);
	}
}

void Server::setUsersDB(UsersDatabase* database) {
	this->usersDB = database;
	connection_->user_data = database;
}
