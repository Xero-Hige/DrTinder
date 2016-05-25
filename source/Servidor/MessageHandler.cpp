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

int MessageHandler::parse(string message, string& resultMsg) {
	Parser parser;
	struct http_message hm;
	int bad = 400;
	int good = 200;

	int err = mg_parse_http( message.c_str(), message.length(), &hm, 1);
	if (err <= 0){
		//TODO: error? no es un request o esta incompleto el mensajes
		resultMsg="NO HTTP REQUEST";
		return 400;
	}else{
		std::string uri_start = parser.getUriStart(&hm);
		std::string uri = parser.getUri(&hm);
		std::string method = parser.getMethod(&hm);
		std::string body = parser.getBody(&hm);
		
		std::cout << uri_start.c_str() << "\n";

		if ( strcmp( uri_start.c_str(),MATCH_URI ) == 0 ){
			resultMsg = matchHandler(method,body);
		}else if ( strcmp(uri_start.c_str(),USER_URI) == 0) {
			resultMsg = userHandler(method,body);
		}else {
			//TODO: bad request
			resultMsg="BAD REQUEST";
			return 400;
		}
		return 200;
	}
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

std::string MessageHandler::matchHandler(std::string method, std::string& body){
	std::cout << method.c_str() << body.c_str() <<"\n";
	return "Matches";
}

std::string MessageHandler::userHandler(std::string method, std::string& body){
	std::cout << method.c_str() << body.c_str() << "\n";	
	return "Users";
}