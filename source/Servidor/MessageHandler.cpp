#include "MessageHandler.h"


using std::string;

MessageHandler::MessageHandler(DatabaseManager *pDatabase, string name) :
		usersDB(pDatabase), username(name), token(NULL) {
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

void MessageHandler::setUsersDB(DatabaseManager * usersDB) {
	this->usersDB = usersDB;
}

bool MessageHandler::getUsers(std::string& resultMsg) {
	ssClient.getUsers(&resultMsg);

	//	TODO: servicio para filtrar usuarios
	return true;
}

bool MessageHandler::authenticate(string username, string password) {
	return usersDB->correctEntry(username, password);
}

void MessageHandler::createUser(string user_data, std::string pass) {
	if (! usersDB->addEntry(username, pass)) {
		throw ExistentUserException();
	}
	CsvParser csvParser;
	User new_user;
	csvParser.makeUser(user_data, new_user);
	//TODO: enviar datos a shared
	//ssClient.postUsers();
}

void MessageHandler::updateUser(string user_data) {
	CsvParser csvParser;
	User new_user;
	csvParser.makeUser(user_data, new_user);
	//TODO: cambiar datos en shared
	//ssClient.changeUser(username, );
}

void MessageHandler::deleteUser() {
	usersDB->deleteEntry(username);
	//TODO: quitar de shared
	//ssClient.deleteUser(username, );
}

void MessageHandler::getInterestPhoto(std::string& photo_64, std::string id_interest) {
	ssClient.getInterestPhoto(id_interest, photo_64);
}

void MessageHandler::getChat(string username, string& chat_history) {
	//TODO: get chat
}

void MessageHandler::getPhoto(string username, string& photo_64) {
	ssClient.getUserPhoto(username, photo_64);
}

void MessageHandler::postPhoto(string photo_64) {
	ssClient.changeUserPhoto(username, photo_64);
}

bool MessageHandler::validateToken(std::string user_token) {
	return (token.compare(user_token) == 0);
}

void MessageHandler::getMatches(std::string id) {
	//TODO: calcular matches
}

string MessageHandler::getToken() {
	return token;
}

void MessageHandler::addLocalization(string localization) {
	int i = localization.find(SEPARATOR);
	string latitude = localization.substr(0, i);
	string longitude = localization.substr(i+1, localization.length());
	//TODO: enviar localizacion al shared
	ssClient.postLocalization(latitude, longitude);
}

void MessageHandler::getUser(string username, string &user_data) {
	ssClient.getUser(username, &user_data);
}

void MessageHandler::receiveChatMessage(string message) {
	//TODO: procesar mensage
}






