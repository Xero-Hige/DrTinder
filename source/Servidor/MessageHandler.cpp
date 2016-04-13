#include "MessageHandler.h"

#define AUTHENTICATION_TOKEN "auth"
#define SEPARATOR ";"
#define RESULT_TOKEN "resu"
#define INVALID_REQUEST "invalid"
#define INCORRECT_LOGIN "incorrect_login"
#define CORRECT_LOGIN "correct_login"

using std::string;

MessageHandler::MessageHandler(UsersDatabase* usersDB) :
	usersDB(usersDB) {
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
	string token = divideMessage(message);
	resultMsg += RESULT_TOKEN;
	resultMsg += SEPARATOR;
	
	if (token.compare(AUTHENTICATION_TOKEN) == 0) {
		return authenticate(message, resultMsg);
	}
	resultMsg += INVALID_REQUEST;
	return false;
}

bool MessageHandler::authenticate(string message, string& resultMsg) {
	string name = divideMessage(message);
	string pass = divideMessage(message);
	if (! usersDB->correctCredentials(name, pass)) {
		resultMsg += INCORRECT_LOGIN;
		return false;
	}
	resultMsg += CORRECT_LOGIN;
	return true;
}