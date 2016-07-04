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
	//TODO parse username to take @ and  .
	makeUsername(name);
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
	if ( ssClient != NULL){
		delete ssClient;
	}
	delete tokenizer;
}

void MessageHandler::updateUsersWithSS(){
	string usersJson;
	ssClient->getUsers(&usersJson);
	CsvParser csvParser;
	JsonParser jsonParser;
	UserParser userParser;

	list<User*> users = userParser.JsonToList(usersJson);
	LOGG(DEBUG) << "Updating username - id of SS users";
	for (User* user : users){
		string username, id_aux;
		username = user->getMail();
		makeUsername(username);//TODO parse email
		if (! usersDB->getEntry(USER_ID_DB + username, id_aux)){
			usersDB->addEntry(USER_DB + username, DEFAULT_PASS);
			usersDB->addEntry(USER_ID_DB + username, to_string(user->getID()));
		}
	}

	while (!users.empty()) {
		delete users.front();
		users.front() = NULL;
		users.pop_front();
	}

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

	if (! this->getUser(username,currentUserData)){
		LOGG(WARNING) << "User does not exist in SS";
		return false;
	}

	User currentUser;
	csvParser.makeUser(currentUserData,currentUser);

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
	makeUsername(username);
	bool found = usersDB->correctEntry(USER_DB + username, password);
	if (!found){
		this->updateUsersWithSS();
		if (! usersDB->correctEntry(USER_DB + username, password)){
			LOGG(DEBUG) << "Incorrect Username-Password";
			return false;
		}
	}
	return found;
}

void MessageHandler::createInterests(Interests* interests){
	map<string,vector<string>> inter = interests->allInterests();
	map<string,vector<string>>::iterator it_c = inter.begin();
	for(; it_c != inter.end(); ++it_c){
		LOGG(DEBUG) << "Creating interests of: " << it_c->first;
		vector<string> values = it_c->second;
		vector<string>::iterator it_v = values.begin();
		for (;it_v != values.end(); ++it_v){
			LOGG(DEBUG) << "Iterating value: " << (*it_v);
			Json::Value interest;
			Json::Value metadata;
			Json::Value to_post;
			interest[CATEGORY_KEY] = it_c->first;
			interest[VALUE_KEY] = (*it_v);
			to_post[INTEREST_KEY] = interest;
			metadata[VERSION_KEY] = VERSION_VALUE;
			metadata[COUNT_KEY] = 1;
			to_post[META_KEY] = metadata;
			JsonParser json;
			string formated_data = json.getAsString(to_post);
			ssClient->postUsersInterests(&formated_data);
		}
	}

}
bool MessageHandler::createUser(string user_data, std::string pass) {
	string aux_pass;
	LOGG(DEBUG)<< "Creating user from data: " + user_data;

	if ( usersDB->getEntry(USER_DB + username, aux_pass)) {
		LOGG(DEBUG)<< "User Already Existed";
		throw ExistentUserException();
	}

	CsvParser csvParser;
	JsonParser jsonParser;
	UserParser userParser;
	User new_user;
	string user_created;
	if (! csvParser.makeSignupUser(user_data, new_user)){
		LOGG(WARNING) << "Bad data for creating user";
		return false;
	}
	createInterests(new_user.getInterests());
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

		LOGG(DEBUG) << "User signup  New Id: " + id;
		return true;
	}else{
		LOGG(DEBUG) << "User not created";
		return false;
	}
}

bool MessageHandler::updateUser(string user_data) {
	LOGG(DEBUG) << "Updating user";
	CsvParser csvParser;
	JsonParser jsonParser;
	User new_user;
	string id = this->getId(), base_user, desc;

	if ( ! this->getUser(username, base_user) ){
		LOGG(WARNING) << "Wanting to update unexistant user " << username;
		return false;
	}

	if (! csvParser.makePutUser(user_data, base_user, new_user)){
		LOGG(WARNING) << "Bad format of user data to modify";
		return false;
	}
	Json::Value jsonUser = jsonParser.userToJson(&new_user, true);
	Json::Value data_to_post;
	data_to_post[META_KEY][VERSION_KEY] = VERSION_VALUE;
	data_to_post[USER_KEY] = jsonUser;
	LOGG(DEBUG) << jsonParser.getAsString(data_to_post);
	LOGG(DEBUG) << "Updating info of " + id ;

	bool changed = ssClient->changeUser(id, jsonParser.getAsString(data_to_post).c_str());
	if (changed){
		desc = new_user.getDescription();
		this->usersDB->addEntry(USER_LOOKING_DB + username, desc);
		//save_user = csvParser.userToCsvFull(&new_user);
		//this->usersDB->addEntry(USER_CSV_DB + username, save_user);
	}
	return changed;
}

bool MessageHandler::deleteUser() {
	string id = this->getId(), reply;
	string token;

	if ( ssClient->deleteUser(id, &reply )){
		usersDB->deleteEntry(USER_DB + username);
		usersDB->deleteEntry(USER_ID_DB + username);
		usersDB->deleteEntry(USER_LOOKING_DB + username);
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
	makeUsername(other_username);

	bool userExists = usersDB->getEntry(USER_ID_DB + other_username, user_id);

	if (! userExists) {
		LOGG(WARNING)<<"Wanted to get photo of unexistant: " << other_username;
		return false;
	}

	return ssClient->getUserPhoto(user_id, photo_64);
}

bool MessageHandler::postPhoto(string photo_64) {
	LOGG(DEBUG) << "Updating photo of " + this->getId();
	if (ssClient->changeUserPhoto(this->getId(), photo_64)){
		return true;
	}
	LOGG(DEBUG) << "Could not update photo";
	return false;
}


bool MessageHandler::validateToken(std::string user_token) {
	bool expired =  this->tokenizer->hasExpired(user_token);
	if (! expired ){
		string user;
		if (this->usersDB->getEntry(USER_OF_TOKEN_DB + user_token,user)){
			LOGG(DEBUG) << "Token corresponds to " << user;
			this->setUser(user);
			return true;
		}
		LOGG(DEBUG)<<"Inexistant user for token";
		return false;
	}else{
		LOGG(DEBUG)<<"Token expired";
		return false;
	}

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

	//si ya tiene un token asociado lo remuevo antes
	//de pedir el nuevo por si son iguales (en el mismo segundo)
	if (this->usersDB->getEntry(TOKEN_OF_USER_DB + username, pastToken)) {
		this->tokenizer->remove(pastToken);
	}

	token = this->tokenizer->newToken(username, password);

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
		LOGG(DEBUG) << "Localization without floats. Latitude: " << latitude << " - longitude: " << longitude;
		return false;
	}
	//TODO: Refactor!
	JsonParser jsonParser;
	UserParser userParser;
	User new_user;
	string id = this->getId(), base_user;

	if (! this->getUser(username, base_user)){
		LOGG(DEBUG) << "Inexistant user";
		return false;
	}

	Json::Value jsonUser;
	userParser.CsvToJsonFull(base_user,jsonUser,1);

	if (id.compare(jsonUser[ID_KEY].asString()) != 0){
		LOGG(WARNING) << "Id in SS does not match with server";
		return false;
	}

	Json::Value new_localization;
	new_localization[LATITUDE_KEY] = stof(latitude);
	new_localization[LONGITUDE_KEY] = stof(longitude);
	jsonUser[LOCATION_KEY] = new_localization;
	jsonUser[ID_KEY] = stoi(id);
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
	makeUsername(username);
	string id;
	//check and update if not found
	if ( ! usersDB->getEntry(USER_ID_DB + username, id)) {
		this->updateUsersWithSS();
	}

	if ( ! usersDB->getEntry(USER_ID_DB + username, id)) {
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
		LOGG(DEBUG)<< "Parsing user: " << json.getAsString(json.getValue(USER_KEY));
		parser.JsonToCsvFull(json.getValue(USER_KEY), user_data, lookingFor);
	} else {
		LOGG(DEBUG) << "Does not exist in shared";
	}
	return ok;
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







