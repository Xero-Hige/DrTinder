#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "DatabaseManager.h"
#include <iostream>
#include "api_constants_client.h"
#include "SharedServerClient.h"
#include "api_server_constants.h"
#include <stdexcept>
#include <vector>
#include "Tokenizer.h"
#include "Parsers/UserParser.h"
#include "Parsers/CsvParser.h"
#include "Parsers/JsonParser.h"
#include "../libs/loger/easylogging++.h"
#include "./matches/UserMatcher.h"
#include "ChatDatabaseManager.h"
#include "ServerUtils.h"

#include <cstdbool>
#include <list>

#include "../libs/jsoncpp/dist/json/json.h"
#include "User.h"
#include "./matches/UserMatcher.h"
#include "LikesDatabaseManager.h"

class ExistentUserException : public exception {
};

typedef  struct _server_databases_t {
	rocksdb::DB *usersDB;
	rocksdb::DB *chatDB;
	rocksdb::DB *likesDB;
} server_databases_t;

/* Handler for incomming requests. */
class MessageHandler {
	public:
		/* Create new handler with pDatabase as usersDB. */
		MessageHandler(server_databases_t* databases);
		/* Destroy handler. */
		~MessageHandler();
		bool isUserSet();
		void setUser(string name);
		/* Set initialized usersDB. */
		void setUsersDB(DatabaseManager* usersDB);
		bool validateToken(std::string user_token);

		bool authenticate(std::string username, std::string password);

		bool createUser(std::string user_data, std::string pass);

		bool getUsers(std::string& resultMsg);

		bool updateUser(std::string user_data);

		bool deleteUser();

		void getMatches(std::string& matches);

		bool getInterestPhoto(std::string& photo_64, std::string id_interest);

		bool getChat(std::string other_username, std::string& chat_history);

		bool getPhoto(string username, string &photo_64);

		bool postPhoto(std::string photo_64);

		std::string getToken();

		bool addLocalization(std::string localization);

		bool getUser(std::string username, std::string &user_data);

		void setSharedLink(std::string linkToShared);

		void createInterests(Interests* interests);
protected:
		/* Authenticate user and password in message. Saves INCORRECT_LOGIN
		or CORRECT_LOGIN in resultMsg. */
		/* Returns first substring of message until SEPARATOR. */
		std::string divideMessage(std::string& message);

		bool postLogin(std::string user_name, std::string token, std::string& resultMsg);
		bool postSignUp(std::string data, std::string& resultMsg);


		std::string getId();
	private:
		DatabaseManager * usersDB;
		ChatDatabaseManager * chatDB;
		LikesDatabaseManager * likesDB;
		SharedServerClient * ssClient;
		Tokenizer* tokenizer;
		std::string username;

	bool match(string &users, string &candidate_data);
};

#endif // PARSER_H
