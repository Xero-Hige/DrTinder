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

bool MessageHandler::parse(string message, string& resultMsg) {
	std::cout << message << "\n";
	string token = divideMessage(message);
	
	if (token.compare(AUTHENTICATION_TOKEN) == 0) {
		return authenticate(message, resultMsg);
	} if (token.compare(GET_USERS_TOKEN) == 0) {
		return getUsers(resultMsg);
	}
	resultMsg += INVALID_REQUEST;
	return false;
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