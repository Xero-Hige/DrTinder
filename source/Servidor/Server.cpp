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

void Server::handleEvent(struct mg_connection* act_connection, int new_event, void* data) {
		
	if (new_event == MG_EV_ACCEPT) {
	
		rocksdb::DB* usersDB = (rocksdb::DB *) act_connection->user_data;
		DatabaseManager* usersDBM = new DatabaseManager(usersDB);
		MessageHandler* msgHandler = new MessageHandler(usersDBM);

		act_connection->user_data = msgHandler;
	} if (new_event == MG_EV_RECV) {

		MessageHandler* msgHandler = (MessageHandler *) act_connection->user_data;
		struct mbuf *io = &act_connection->recv_mbuf;
		
		std::string reply, recv_str(io->buf, io->len);
		int status = msgHandler->parse(recv_str, reply);

		mbuf_remove(io, io->len);

		mg_printf(act_connection, "HTTP/1.1 %d\r\n"
						"Transfer-Encoding: chunked\r\n"
						"\r\n", status);
		mg_printf_http_chunk(act_connection, "%s", reply.c_str());
		mg_send_http_chunk(act_connection,"",0);
		
		
	}
}

void Server::setUsersDB(rocksdb::DB *database) {
	this->usersDB = database;
	connection_->user_data = database;
}
