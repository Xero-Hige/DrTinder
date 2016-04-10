#include "Client.h"

#define DEFAULT_MILISECS_POLL 3000

static const char *s_url =
    "http://ajax.googleapis.com/ajax/services/search/web?v=1.0&q=cesanta";	//TODO: sacar cuando este la conexion


Client::Client() {
	mg_mgr_init(&manager_, NULL);
	connection_ = mg_connect_http(&manager_, handleEvent, s_url, NULL, NULL);
	mg_set_protocol_http_websocket(connection_);
}

Client::~Client() {
	mg_mgr_free(&manager_);
}

void Client::connect() {
	mg_mgr_poll(&manager_, DEFAULT_MILISECS_POLL);
}


void Client::handleEvent(struct mg_connection *nc, int ev, void *ev_data) {	//TODO: adaptar todo
	struct http_message *hm = (struct http_message *) ev_data;
	int connect_status;

	switch (ev) {
	case MG_EV_CONNECT:
		connect_status = *(int *) ev_data;
		if (connect_status != 0) {
			printf("Error connecting to %s: %s\n", s_url, strerror(connect_status));
		}
		break;
	case MG_EV_HTTP_REPLY:
		printf("Got reply:\n%.*s\n", (int) hm->body.len, hm->body.p);
		nc->flags |= MG_F_SEND_AND_CLOSE;
		break;
	default:
		break;
	}
}
