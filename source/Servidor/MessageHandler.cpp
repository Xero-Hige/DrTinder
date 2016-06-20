#include "MessageHandler.h"


using std::string;

MessageHandler::MessageHandler(DatabaseManager *pDatabase, string name, string pass) :
	usersDB(pDatabase), username(name), token(NULL) {

	//si ya existe no sobreescribe
	string found;
	if (! usersDB->getEntry(USER_DB + username,found)){
		usersDB->addEntry(USER_DB + username, pass);
	}
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
	return usersDB->correctEntry(USER_DB + username, password);
}

void MessageHandler::createUser(string user_data) {
	CsvParser csvParser;
	JsonParser jsonParser;
	User new_user;
	string user_created;
	csvParser.makeUser(user_data, new_user);

	Json::Value jsonUser = jsonParser.userToJson(&new_user);
	Json::Value data_to_post;
	data_to_post[META_KEY][VERSION_KEY] = VERSION_VALUE;
	data_to_post[USER_KEY] = jsonUser;

	string parsed_user = jsonParser.getAsString(data_to_post);
	bool posted = ssClient.postUsers(user_created, &parsed_user);

	if (posted){
		string id, mail;
		jsonParser.parsing(user_created);
		id = jsonParser.getValue(USER_KEY)[ID_KEY].asString();
		usersDB->addEntry(USER_ID_DB + username, id);
		//Mandar ok
		//guardar pass en DB
	}else{
		//no se pudo crear -> user_created -> ue
	}
}

void MessageHandler::updateUser(string user_data) {
	CsvParser csvParser;
	User new_user;
	csvParser.makeUser(user_data, new_user);
	//TODO: cambiar datos en shared
	//ssClient.changeUser(username, );
}

void MessageHandler::deleteUser() {
	string id, reply;
	usersDB->getEntry(USER_ID_DB + username, id);

	//TODO: quitar de shared
	if ( ssClient.deleteUser(id, &reply )){
		usersDB->deleteEntry(USER_DB + username);
		usersDB->deleteEntry(USER_ID_DB + username);
		//TODO: delete all info
	}
}

void MessageHandler::getInterestPhoto(std::string& photo_64, std::string id_interest) {
	ssClient.getInterestPhoto(id_interest, photo_64);
}

void MessageHandler::getChat(string& chat_history) {
	//TODO: get chat
}

void MessageHandler::getPhoto(string& photo_64) {
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
