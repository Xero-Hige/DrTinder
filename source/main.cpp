#include <iostream>
#include "Server.h"

int main() {
	Server server;
	while (! std::cin.eof()) {	//TODO: mandar a otro thread
		server.run();
	}
    return 0;
}
