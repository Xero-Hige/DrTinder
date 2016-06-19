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
#include "HttpResponse.h"
#include "Handlers/Handler.h"
#include "Handlers/userHandler.h"
#include "Handlers/usersHandler.h"
#include "Handlers/chatHandler.h"
#include "Handlers/friendsHandler.h"
#include "Handlers/interestHandler.h"

/* Handler for incomming requests. */
class MessageHandler {
	public:
		/* Create new handler with pDatabase as usersDB. */
		MessageHandler(DatabaseManager *pDatabase, std::string mail, std::string pass);
		/* Destroy handler. */
		~MessageHandler();
		/* Parse message and handle request. Return HttpResponse  */
		HttpResponse parse(std::string message);
		/* Set initialized usersDB. */
		void setUsersDB(DatabaseManager* usersDB);
		bool validateToken(std::string user_token);

		bool authenticate(std::string username, std::string password);

		void createUser(struct mg_str* user_data);

		bool getUsers(std::string& resultMsg);

		void updateUser(struct mg_str* user_data);

		void deleteUser();

		void getMatches(std::string id);

	void getInterest(std::string photo_64, std::string id_interest);

	void getChat(std::string chat_history);

	void getPhoto(std::string photo_64);

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
		std::vector<Handler *> handlers;
		std::string token;
		std::string username;
};

#endif // PARSER_H
