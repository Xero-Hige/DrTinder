#include <iostream>
#include "Client.h"

int main() {
	Client client;
	client.send("mi mensaje 1");
	while (! std::cin.eof()) { //TODO: mandar a otro thread
		client.run();
	}
    return 0;
}
