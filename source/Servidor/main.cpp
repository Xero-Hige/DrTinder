#include <iostream>
#include <thread>
#include <mutex>
#include "Server.h"
#include "DatabaseManager.h"

#define QUIT_LINE "quit"

void hasToQuit(bool& result,  std::mutex& result_mutex) {
	std::string line;
	while (line.compare(QUIT_LINE) != 0) {
		std::cin >> line;
	}
	result_mutex.lock();
	result = true;
	result_mutex.unlock();
}

int main() {
	Server server;

	rocksdb::DB* usersDB;
	DatabaseManager usersDBM(usersDB);
	usersDBM.addEntry("deb", "123");

	server.setUsersDB(&usersDBM);

	bool quit = false;
	std::mutex quit_mutex;
	std::thread quit_control_thread(hasToQuit, std::ref(quit), std::ref(quit_mutex));

	quit_mutex.lock();
	while (! quit) {
		quit_mutex.unlock();
		server.run();
		quit_mutex.lock();
	}
	quit_mutex.unlock();

	quit_control_thread.join();
    return 0;
}
