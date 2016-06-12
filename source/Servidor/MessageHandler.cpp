#include "MessageHandler.h"

using std::string;

MessageHandler::MessageHandler() :
	usersDB(NULL) {
	handlers.push_back((Handler *)new userHandler());
	handlers.push_back((Handler *) new usersHandler());
	handlers.push_back((Handler *) new friendsHandler());
	handlers.push_back((Handler *) new interestHandler());
	handlers.push_back((Handler *) new chatHandler());
}

MessageHandler::MessageHandler(DatabaseManager *pDatabase) : usersDB(pDatabase){
	handlers.push_back((Handler *)new userHandler());
	handlers.push_back((Handler *) new usersHandler());
	handlers.push_back((Handler *) new friendsHandler());
	handlers.push_back((Handler *) new interestHandler());
	handlers.push_back((Handler *) new chatHandler());
}

MessageHandler::~MessageHandler() {
	std::vector<Handler*>::iterator it;
	for ( it = handlers.begin(); it != handlers.end(); ){
	      delete * it;
	      it = handlers.erase(it);
	}
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

HttpResponse MessageHandler::parse(string message) {
	Parser parser;
	HttpResponse resp;
	struct http_message hm;

	int err = mg_parse_http( message.c_str(), message.length(), &hm, 1);
	if (err <= 0){
		resp.turnToBadRequest("NO HTTP REQUEST");
	}else{
		std::string uri_start = parser.getUriStart(&hm);

		for (std::vector<Handler*>::iterator it = handlers.begin() ; it != handlers.end(); ++it){
			if ((*it)->manages(uri_start)){
				return (*it)->httpRequest(&hm);
			}
		}

		resp.turnToBadRequest("No existe handler");
	}
	return resp;

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
