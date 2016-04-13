#include <iostream>
#include "Server.h"
#include "MessageHandler.h"
#include "UsersDatabase.h"

int main() {
	Server server;
	
	UsersDatabase usersDB;
	usersDB.addUser("deb", "123");
	
	MessageHandler msgHandler(&usersDB);
	server.setMessageHandler(&msgHandler);
	while (! std::cin.eof()) {	//TODO: mandar a otro thread
		server.run();
	}
    return 0;
}
