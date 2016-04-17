#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "UsersDatabase.h"
#include <iostream>

class MessageHandler {
	public:
		MessageHandler();

	MessageHandler(UsersDatabase *pDatabase);

	~MessageHandler();
		bool parse(std::string message, std::string& resultMsg);
		void setUsersDB(UsersDatabase *usersDB);

	protected:
		bool authenticate(std::string message, std::string& resultMsg);
		std::string divideMessage(std::string& message);
	private:
		UsersDatabase* usersDB;

};

#endif // PARSER_H
