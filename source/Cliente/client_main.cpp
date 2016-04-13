#include <iostream>
#include "Client.h"

int main() {
	Client client;
	
	client.send("auth;deb;123"); //un mensaje correcto de login con un usuario existente
	
	while (! std::cin.eof()) { //TODO: mandar a otro thread
		client.run();
	}
    return 0;
}
