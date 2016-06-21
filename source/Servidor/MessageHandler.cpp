#include "MessageHandler.h"


using std::string;

MessageHandler::MessageHandler(DatabaseManager *pDatabase, string name, string pass) :
	usersDB(pDatabase) {
	username = name;
	this->tokenizer = new Tokenizer(usersDB);

	//si ya existe no sobreescribe, problemas con el signup???
	string found;
	if (! usersDB->getEntry(USER_DB + username,found)){
		usersDB->addEntry(USER_DB + username, pass);
	}

}

MessageHandler::~MessageHandler() {
	delete tokenizer;
}

string MessageHandler::getId(){
	string id;
	if (usersDB->getEntry(USER_ID_DB + username, id))
		return id;
	return "";
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
	LOGG(DEBUG) << "Authenticating " + username;
	return usersDB->correctEntry(USER_DB + username, password);
}

bool MessageHandler::createUser(string user_data) {
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
	LOGG(DEBUG) << "Creating user " + parsed_user;
	bool posted = ssClient.postUsers(user_created, &parsed_user);

	if (posted){
		string id, mail;
		jsonParser.parsing(user_created);

		id = jsonParser.getValue(USER_KEY)[ID_KEY].asString();
		usersDB->addEntry(USER_ID_DB + username, id);

		LOGG(DEBUG) << "User signup  New Id: " + id;
		return true;
		//TODO guardar pass en DB
	}else{
		LOGG(DEBUG) << "User not created";
		return false;
	}
}

bool MessageHandler::updateUser(string user_data) {
	CsvParser csvParser;
	JsonParser jsonParser;
	User new_user;
	string id = this->getId();

	//feo
	user_data += ",\""+ id +"\"";
	csvParser.makeUser(user_data, new_user);

	Json::Value jsonUser = jsonParser.userToJson(&new_user,true);
	Json::Value data_to_post;
	data_to_post[META_KEY][VERSION_KEY] = VERSION_VALUE;
	data_to_post[USER_KEY] = jsonUser;

	LOGG(DEBUG) << "Updating info of " + id ;

	return ssClient.changeUser(id, jsonParser.getAsString(data_to_post).c_str());
}

bool MessageHandler::deleteUser() {
	string id = this->getId(), reply;

	if ( ssClient.deleteUser(id, &reply )){
		usersDB->deleteEntry(USER_DB + username);
		usersDB->deleteEntry(USER_ID_DB + username);
		//TODO: delete all info
		LOGG(DEBUG) << "User Deleted " + username;
		return true;
	}
	return false;
}

bool MessageHandler::getInterestPhoto(std::string& photo_64, std::string id_interest) {
	return ssClient.getInterestPhoto(id_interest, photo_64);
}

bool MessageHandler::getChat(string& chat_history) {
	//TODO: get chat
}

bool MessageHandler::getPhoto(string& photo_64) {
	return ssClient.getUserPhoto(this->getId(), photo_64);
}

bool MessageHandler::postPhoto(string photo_64) {
	LOGG(INFO) << "Updating photo of " + this->getId();
	return ssClient.changeUserPhoto(this->getId(), photo_64);
}

bool MessageHandler::validateToken(std::string user_token) {
	return ! this->tokenizer->hasExpired(user_token);
}

void MessageHandler::getMatches(std::string id) {
	//TODO: calcular matches
}

string MessageHandler::getToken() {
	string password, token;
	LOGG(INFO) << "Generating new token for " + username;
	this->usersDB->getEntry(USER_DB + username, password);
	return this->tokenizer->newToken(username, password);
}
