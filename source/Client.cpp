#include "Client.h"

#define DEFAULT_MILISECS_POLL 3000
#define SERVER_ADDRESS "tcp://127.0.0.1:8080"

using std::string;

Client::Client() {
	mg_mgr_init(&manager_, NULL);
	connection_ = mg_connect(&manager_, SERVER_ADDRESS, handleEvent);
}

Client::~Client() {
	mg_mgr_free(&manager_);
}

void Client::run() {
	mg_mgr_poll(&manager_, DEFAULT_MILISECS_POLL);
}


void Client::handleEvent(struct mg_connection *nc, int ev, void *ev_data) {	//TODO: adaptar todo
	struct mbuf *io = &nc->recv_mbuf;
	int connect_status;

	switch (ev) {
	case MG_EV_CONNECT:
		connect_status = *(int *) ev_data;
		if (connect_status != 0) {
			printf("Error connecting to %s: %s\n", SERVER_ADDRESS, strerror(connect_status)); //TODO: reemplazar por algo mejor
		}
		break;
	case MG_EV_RECV:
		printf("Got reply:\n%.*s\n", (int) io->len, io->buf);
		break;
	default:
		break;
	}
}

void Client::send(string message) {
	mg_send(connection_, message.c_str(), message.size());
}
