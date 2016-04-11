#include "Server.h"

#define DEFAULT_MILISECS_POLL 3000
#define REPLY_LEN 100

void intToString(int number, std::string& result) {
    std::ostringstream convert;
    convert << number;
    result = convert.str();
}

Server::Server() {
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
	mg_mgr_poll(&manager_, DEFAULT_MILISECS_POLL);
}

void Server::handleEvent(struct mg_connection* act_connection, int new_event, void* data) {
	if (new_event == MG_EV_RECV) {
		struct mbuf *io = &act_connection->recv_mbuf;
		
		char reply[REPLY_LEN];
		
		strcpy(reply, io->buf);

		mg_send(act_connection, reply, REPLY_LEN);
		mbuf_remove(io, io->len);
  }
}
