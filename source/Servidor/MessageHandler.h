#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "DatabaseManager.h"
#include <iostream>

class MessageHandler {
	public:
		MessageHandler();

	MessageHandler(DatabaseManager *pDatabase);

	~MessageHandler();
		bool parse(std::string message, std::string& resultMsg);
		void setUsersDB(DatabaseManager *usersDB);

	protected:
		bool authenticate(std::string message, std::string& resultMsg);
		std::string divideMessage(std::string& message);
	private:
		DatabaseManager * usersDB;

};

#endif // PARSER_H
