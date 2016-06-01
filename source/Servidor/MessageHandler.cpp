#include "MessageHandler.h"

using std::string;

MessageHandler::MessageHandler() :
	usersDB(NULL) {
}

MessageHandler::MessageHandler(DatabaseManager *pDatabase) : usersDB(pDatabase){
}

MessageHandler::~MessageHandler() {
}

string MessageHandler::divideMessage(string& message) {
	size_t pos = message.find(SEPARATOR);
	if (pos == std::string::npos) {
		pos = message.size();
	}
	string subMsg = message.substr(0, pos);
	message.erase(0, pos + 1);
	return subMsg;
}

HttpResponse MessageHandler::parse(string message, string& resultMsg) {
	Parser parser;
	HttpResponse resp;
	struct http_message hm;
	int bad = 400;
	int good = 200;

	int err = mg_parse_http( message.c_str(), message.length(), &hm, 1);
	if (err <= 0){
		resp.turnToBadRequest("NO HTTP REQUEST");
	}else{
		std::string uri_start = parser.getUriStart(&hm);

		std::cout << uri_start.c_str() << "\n";

		if ( strcmp(uri_start.c_str(),USER_URI) == 0) {
			userHandler handler;
			resp = handler.httpRequest(&hm);
		}else {
			resp.turnToBadRequest("No existe handler");
		}
	}
	return resp;
/*
	string token = divideMessage(message);
	if (token.compare(AUTHENTICATION_TOKEN) == 0) {
		return authenticate(message, resultMsg);
	} if (token.compare(GET_USERS_TOKEN) == 0) {
		return getUsers(resultMsg);
	}
	resultMsg += INVALID_REQUEST;
	return false;*/
}

bool MessageHandler::authenticate(string message, string& resultMsg) {
	//TODO: token
	string name = divideMessage(message);
	string pass = divideMessage(message);
	if (!usersDB->correctEntry(name, pass)) {
		resultMsg += INCORRECT_LOGIN;
		return false;
	}
	resultMsg += CORRECT_LOGIN;
	return true;
}

void MessageHandler::setUsersDB(DatabaseManager * usersDB) {
	this->usersDB = usersDB;
}

bool MessageHandler::getUsers(std::string& resultMsg) {
	ssClient.getUsers(&resultMsg);

	//	TODO: servicio para filtrar usuarios
	return true;
}
