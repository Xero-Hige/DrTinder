#include <iostream>
#include "Client.h"

int main() {
	Client client;
	while (! std::cin.eof()) {
		client.connect();
	}
    return 0;
}
