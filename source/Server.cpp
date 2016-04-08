#include "Server.h"

string intToString(int number) {
    ostringstream convert;
    convert << number;
    return convert.str();
}
int getEventHandler(struct mg_connection* act_connection, enum mg_event event) {
    return 0;
}

Server::Server() {
    server = mg_create_server(NULL, getEventHandler);
    mg_set_option(server, "listening_port", intToString(PORT_NUMBER).c_str());

}

Server::~Server() {

}
