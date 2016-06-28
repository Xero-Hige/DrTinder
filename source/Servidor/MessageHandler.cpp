#include "MessageHandler.h"


using std::string;


MessageHandler::MessageHandler(server_databases_t *databases) :
	usersDB(new DatabaseManager(databases->usersDB)), chatDB(new ChatDatabaseManager(databases->chatDB)),
	likesDB(new LikesDatabaseManager(databases->likesDB)) {
	username = "";
	ssClient = NULL;
	this->tokenizer = new Tokenizer(usersDB);
}

void MessageHandler::setUser(string name){
	LOGG(DEBUG) << "Set user of msgHandler to " << name;
	username = name;
}

void MessageHandler::setSharedLink(std::string linkToShared){
	if (ssClient == NULL){
		ssClient = new SharedServerClient(linkToShared);
	}
	else{
		delete ssClient;
		ssClient = new SharedServerClient(linkToShared);
	}
}

bool MessageHandler::isUserSet(){
	return (! username.empty());
}

MessageHandler::~MessageHandler() {
	if ( ! ssClient == NULL){
		delete ssClient;
	}
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
	ssClient->getUsers(&usersJson);
	CsvParser csvParser;
	JsonParser jsonParser;
	UserParser userParser;

	list<User*> users = userParser.JsonToList(usersJson);

	string currentUserData;

	usersDB->getEntry(USER_CSV_DB + username, currentUserData);
	LOGG(DEBUG) << "User Taken from DB: "<< currentUserData ;
	User currentUser;
	csvParser.makeUser(currentUserData, currentUser);

	string userMatches;
	getMatches(userMatches);

	UserMatcher matcher;

	list<User*> filtered_users = matcher.filterPossibleMatches(&currentUser, &users, userMatches);

	resultMsg = userParser.ListToCsv(filtered_users);

	while(!users.empty()){
		delete users.front();
		users.front() = NULL;
		users.pop_front();
	}

	return true;
}

bool MessageHandler::authenticate(string username, string password) {
	LOGG(DEBUG) << "Authenticating " + username;
	return usersDB->correctEntry(USER_DB + username, password);
}

bool MessageHandler::createUser(string user_data, std::string pass) {
	string aux_pass;
	LOGG(DEBUG)<< "Creating user";
	if ( usersDB->getEntry(USER_DB + username, aux_pass)) {
		LOGG(DEBUG)<< "User Already Existed";
		throw ExistentUserException();
	}

	CsvParser csvParser;
	JsonParser jsonParser;
	UserParser userParser;
	User new_user;
	string user_created;
	csvParser.makeSignupUser(user_data, new_user);

	Json::Value jsonUser = jsonParser.userToJson(&new_user);
	Json::Value data_to_post;
	data_to_post[META_KEY][VERSION_KEY] = VERSION_VALUE;
	data_to_post[USER_KEY] = jsonUser;

	string parsed_user = jsonParser.getAsString(data_to_post);
	LOGG(INFO) << "Creating user " + parsed_user;
	bool posted = ssClient->postUsers(user_created, &parsed_user);

	if (posted){
		string id, mail, user_csv;
		jsonParser.parsing(user_created);
		Json::Value json_user =jsonParser.getValue(USER_KEY);
		id = json_user[ID_KEY].asString();

		userParser.JsonToCsvFull(json_user, user_csv, new_user.getDescription());

		usersDB->addEntry(USER_DB + username, pass);
		usersDB->addEntry(USER_LOOKING_DB + username, new_user.getDescription());
		usersDB->addEntry(USER_ID_DB + username, id);
		usersDB->addEntry(USER_CSV_DB + username, user_csv);

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
	string id = this->getId(), base_user;

	if ( !usersDB->getEntry(USER_CSV_DB + username, base_user) ){
		LOGG(WARNING) << "Wanting to update unexistant user " << username;
		return false;
	}

	csvParser.makePutUser(user_data, base_user, new_user);

	Json::Value jsonUser = jsonParser.userToJson(&new_user,true);
	Json::Value data_to_post;
	data_to_post[META_KEY][VERSION_KEY] = VERSION_VALUE;
	data_to_post[USER_KEY] = jsonUser;
	cout << jsonParser.getAsString(data_to_post) << endl;
	LOGG(INFO) << "Updating info of " + id ;

	return ssClient->changeUser(id, jsonParser.getAsString(data_to_post).c_str());
}

bool MessageHandler::deleteUser() {
	string id = this->getId(), reply;
	string token;

	if ( ssClient->deleteUser(id, &reply )){
		usersDB->deleteEntry(USER_DB + username);
		usersDB->deleteEntry(USER_ID_DB + username);
		usersDB->deleteEntry(USER_LOOKING_DB + username);
		usersDB->deleteEntry(USER_CSV_DB + username);
		usersDB->deleteEntry(USER_PHOTO_DB + username);
		if (usersDB->getEntry(TOKEN_OF_USER_DB + username, token))
			usersDB->deleteEntry(USER_OF_TOKEN_DB + token);
		usersDB->deleteEntry(TOKEN_OF_USER_DB + username);
		//TODO: Delete matches, friends, y si ambos no estan chat
		LOGG(INFO) << "User Deleted " + username;
		return true;
	}
	return false;
}

bool MessageHandler::getInterestPhoto(std::string& photo_64, std::string id_interest) {
	return ssClient->getInterestPhoto(id_interest, photo_64);
}

bool MessageHandler::getChat(std::string other_username, string& chat_history) {
	return chatDB->getHistory(username, other_username, chat_history);
}

bool MessageHandler::getPhoto(std::string other_username, string& photo_64) {
	string user_id, photo;
	LOGG(DEBUG)<<"Getting user for " << other_username;
	if (usersDB->getEntry(USER_PHOTO_DB + other_username, photo)){
		LOGG(DEBUG)<<"Photo already in database";
		photo_64 = photo;
		return true;
	}else{
		if ( ! usersDB->getEntry(USER_ID_DB + other_username, user_id)){
			LOGG(WARNING)<<"Wanted to get photo of unexistant: " << other_username ;
			return false;
		}
		return ssClient->getUserPhoto(this->getId(), photo_64);
	}
}

bool MessageHandler::postPhoto(string photo_64) {
	LOGG(DEBUG) << "Updating photo of " + this->getId();
	if (ssClient->changeUserPhoto(this->getId(), photo_64)){
		usersDB->addEntry(USER_PHOTO_DB + username, photo_64);
		return true;
	}
	LOGG(DEBUG) << "Could not update photo";
	return false;
}


bool MessageHandler::validateToken(std::string user_token) {
	bool expired =  this->tokenizer->hasExpired(user_token);
	if (! expired ){
		return this->usersDB->getEntry(USER_OF_TOKEN_DB + user_token,this->username);
	}
	return expired;
}


void MessageHandler::getMatches(std::string& matches) {
	LOGG(INFO) << "Devolviendo usuarios matcheados.";
	usersDB->createIterator();
	while (usersDB->validIterator()) {
		string users, liked, candidate_data;

		usersDB->getActualPair(users, liked);
		if (liked == LIKED_TOKEN && match(users, candidate_data)) {
			matches.append(candidate_data + "\n");
		}
		usersDB->advanceIterator();
	}
	usersDB->deleteIterator();
}

string MessageHandler::getToken() {
	string password, token, pastToken;
	LOGG(DEBUG) << "Generating new token for " + username;
	if (!this->usersDB->getEntry(USER_DB + username, password))
	{
		LOGG(WARNING) << "User has no registered pass";
	}
	token = this->tokenizer->newToken(username, password);

	//si ya tiene un token asociado lo remuevo
	if (this->usersDB->getEntry(TOKEN_OF_USER_DB + username,pastToken)){
		this->tokenizer->remove(pastToken);
	}

	//renuevo la referencia token-user
	this->usersDB->deleteEntry(USER_OF_TOKEN_DB + pastToken);
	this->usersDB->addEntry(USER_OF_TOKEN_DB + token, username);

	this->usersDB->addEntry(TOKEN_OF_USER_DB + username, token);
	return token;

}

bool MessageHandler::addLocalization(string localization) {
	LOGG(DEBUG) << "Changing " << localization << " for " << username;
	int i = localization.find(SEPARATOR);
	string latitude = localization.substr(0, i);
	string longitude = localization.substr(i+1, localization.length());
	if ( !isFloat(latitude) || !isFloat(longitude)){
		LOGG(DEBUG) << "Localization withot floats";
		return false;
	}
	//TODO: Refactor!
	CsvParser csvParser;
	JsonParser jsonParser;
	User new_user;
	string id = this->getId(), base_user;

	usersDB->getEntry(USER_CSV_DB + username, base_user);
	csvParser.makeUser(base_user, new_user);

	Json::Value jsonUser = jsonParser.userToJson(&new_user,true);
	Json::Value new_localization;
	new_localization[LATITUDE_KEY] = stof(latitude);
	new_localization[LONGITUDE_KEY] = stof(longitude);
	jsonUser[LOCATION_KEY] = new_localization;
	Json::Value data_to_post;
	data_to_post[META_KEY][VERSION_KEY] = VERSION_VALUE;
	data_to_post[USER_KEY] = jsonUser;

	LOGG(DEBUG) << "Updating info of " + id ;
	LOGG(DEBUG) << "User: " <<  jsonParser.getAsString(data_to_post);
	return ssClient->changeUser(id, jsonParser.getAsString(data_to_post));
}

bool MessageHandler::getUser(string username, string &user_data) {
	//busca en DB
	LOGG(DEBUG) << "Getting user " << username;
	if (! usersDB->getEntry(USER_CSV_DB + username, user_data)){
		string id;
		LOGG(DEBUG) << "Not in database";
		if ( !usersDB->getEntry(USER_ID_DB + username, id)){
			LOGG(DEBUG) << "Does not exist in database";
			return false;
		}
		//busca en SHARED
		bool ok = ssClient->getUser(id, &user_data);
		if (ok) {
			LOGG(DEBUG) << "Got user from shared";
			UserParser parser;
			JsonParser json;
			string copy(user_data), lookingFor = "";
			json.parsing(copy);
			usersDB->getEntry(USER_LOOKING_DB + username, lookingFor);
			parser.JsonToCsvFull(json.getRoot(), user_data, lookingFor);
			usersDB->addEntry(USER_CSV_DB + username, user_data);
			usersDB->addEntry(USER_PHOTO_DB + username, json.getValue(USER_KEY)[PHOTO_KEY].asString());
		}else{
			LOGG(DEBUG) << "Does not exist in shared";
		}
		return ok;
	}
	CsvParser csv;
	csv.removeId(user_data);
	return true;

}

bool MessageHandler::match(string& users, string& candidate_data) {
	size_t i = users.find(DB_SEPARATOR);
	string user1 = users.substr(0, i);
	string user2 = users.substr(i+1, users.length());

	if (user1 != username ) {
		if (user2 != username) {
			return false;
		}
		getUser(user1, candidate_data);
		return true;
	}
	getUser(user2, candidate_data);
	return true;
}







