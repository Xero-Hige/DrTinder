#include "MessageHandler.h"

#include <cstdbool>
#include <list>

#include "../libs/jsoncpp/dist/json/json.h"
#include "User.h"


using std::string;


MessageHandler::MessageHandler(DatabaseManager *pDatabase, string name) :
	usersDB(pDatabase) {
	username = name;
	this->tokenizer = new Tokenizer(usersDB);
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
	string usersJson;
	ssClient.getUsers(&usersJson);
	CsvParser csvParser;
	JsonParser jsonParser;

//	TODO
//	list<User*> users = jsonParser.getUserList(userJson);
	list<User*> user;



	resultMsg = usersJson;
	//	TODO: servicio para filtrar usuarios
	return true;
}

bool MessageHandler::authenticate(string username, string password) {
	LOGG(DEBUG) << "Authenticating " + username;
	return usersDB->correctEntry(USER_DB + username, password);
}

bool MessageHandler::createUser(string user_data, std::string pass) {
	if (! usersDB->addEntry(USER_DB + username, pass)) {
		throw ExistentUserException();
	}

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
	LOGG(INFO) << "Creating user " + parsed_user;
	bool posted = ssClient.postUsers(user_created, &parsed_user);

	if (posted){
		string id, mail;
		jsonParser.parsing(user_created);

		id = jsonParser.getValue(USER_KEY)[ID_KEY].asString();
		usersDB->addEntry(USER_ID_DB + username, id);

		LOGG(DEBUG) << "User signup  New Id: " + id;
		return true;
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

	LOGG(INFO) << "Updating info of " + id ;

	return ssClient.changeUser(id, jsonParser.getAsString(data_to_post).c_str());
}

bool MessageHandler::deleteUser() {
	string id = this->getId(), reply;

	if ( ssClient.deleteUser(id, &reply )){
		usersDB->deleteEntry(USER_DB + username);
		usersDB->deleteEntry(USER_ID_DB + username);
		//TODO: delete all info
		LOGG(INFO) << "User Deleted " + username;
		return true;
	}
	return false;
}

bool MessageHandler::getInterestPhoto(std::string& photo_64, std::string id_interest) {
	return ssClient.getInterestPhoto(id_interest, photo_64);
}

bool MessageHandler::getChat(std::string username, string& chat_history) {
	//TODO: get chat
}

bool MessageHandler::getPhoto(std::string other_username, string& photo_64) {
	string user_id;
	usersDB->getEntry(USER_ID_DB + other_username, user_id);
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
	LOGG(INFO) << "Devolviendo usuarios matcheados.";
	//TODO: calcular matches
}

string MessageHandler::getToken() {
	string password, token;
	LOGG(INFO) << "Generating new token for " + username;
	this->usersDB->getEntry(USER_DB + username, password);
	return this->tokenizer->newToken(username, password);
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






