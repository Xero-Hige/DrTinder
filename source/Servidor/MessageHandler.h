#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "DatabaseManager.h"
#include <iostream>
#include "api_constants_client.h"
#include "SharedServerClient.h"
#include "Parser.h"
#include "api_server_constants.h"
#include <stdexcept>
#include <vector>
#include "Tokenizer.h"
#include "Parsers/CsvParser.h"
#include "Parsers/JsonParser.h"


/* Handler for incomming requests. */
class MessageHandler {
	public:
		/* Create new handler with pDatabase as usersDB. */
		MessageHandler(DatabaseManager *pDatabase, std::string mail, std::string pass);
		/* Destroy handler. */
		~MessageHandler();
		/* Set initialized usersDB. */
		void setUsersDB(DatabaseManager* usersDB);
		bool validateToken(std::string user_token);

		bool authenticate(std::string username, std::string password);

		void createUser(std::string user_data);

		bool getUsers(std::string& resultMsg);

		void updateUser(std::string user_data);

		void deleteUser();

		void getMatches(std::string id);

		void getInterestPhoto(std::string& photo_64, std::string id_interest);

		void getChat(std::string& chat_history);

		void getPhoto(std::string& photo_64);

		void postPhoto(std::string photo_64);

		std::string getToken();

	protected:
		/* Authenticate user and password in message. Saves INCORRECT_LOGIN
		or CORRECT_LOGIN in resultMsg. */
		/* Returns first substring of message until SEPARATOR. */
		std::string divideMessage(std::string& message);

		bool postRechazado(int id_origen, int id_rechazado, std::string& resultMsg);
		bool postAceptado(int id_origen, int id_rechazado, std::string& resultMsg);
		/*TODO: pensar formato de la data traida*/

		bool postLogin(std::string user_name, std::string token, std::string& resultMsg);
		bool postSignUp(std::string data, std::string& resultMsg);
		/*TODO: POST chat /chat*/
		/*TODO: PUT /users */

	private:
		DatabaseManager * usersDB;
		SharedServerClient ssClient;
		std::string token;
		std::string username;
};

#endif // PARSER_H
