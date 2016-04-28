#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "UsersDatabase.h"
#include <iostream>

/* Handler for incomming requests. */
class MessageHandler {
	public:
		/* Create new handler with NULL usersDB. */
		MessageHandler();
		/* Create new handler with pDatabase as usersDB. */
		MessageHandler(UsersDatabase *pDatabase);
		/* Destroy handler. */
		~MessageHandler();
		/* Parse message and handle request. Get INVALID_REQUEST in resultMsg 
		or valid answer. */
		bool parse(std::string message, std::string& resultMsg);
		/* Set initialized usersDB. */
		void setUsersDB(UsersDatabase *usersDB);

	protected:
		/* Authenticate user and password in message. Saves INCORRECT_LOGIN
		or CORRECT_LOGIN in resultMsg. */
		bool authenticate(std::string message, std::string& resultMsg);
		/* Returns first substring of message until SEPARATOR. */
		std::string divideMessage(std::string& message);
	private:
		UsersDatabase* usersDB;

};

#endif // PARSER_H
