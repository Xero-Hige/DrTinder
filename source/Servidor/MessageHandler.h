#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "UsersDatabase.h"

class MessageHandler {
	public:
		MessageHandler(UsersDatabase* usersDB);
		~MessageHandler();
		bool parse(std::string message, std::string& resultMsg);
	protected:
		bool authenticate(std::string message, std::string& resultMsg);
		std::string divideMessage(std::string& message);
	private:
		UsersDatabase* usersDB;
};

#endif // PARSER_H
