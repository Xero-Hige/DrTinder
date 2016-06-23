#include "MessageHandler.h"


using std::string;


MessageHandler::MessageHandler(server_databases_t *databases, string name) :
	usersDB(new DatabaseManager(databases->usersDB)), chatDB(new ChatDatabaseManager(databases->chatDB)),
	likesDB(new LikesDatabaseManager(databases->likesDB)) {
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
	UserParser userParser;

	list<User*> users = userParser.JsonToList(usersJson);

	//TODO REVISAR SI ESTA ES LA FORMA CORRECTA DE OBTENER EL USUARIO LOGEADO EN LA APP.
	string currentUserData;

	bool gotUser = usersDB->getEntry(USER_CSV_DB + username, currentUserData);
	cout << currentUserData << "\n User Taken from DB\n";
	User currentUser;
	csvParser.makeUser(currentUserData, currentUser);

	UserMatcher matcher;

	list<User*> filtered_users = matcher.filterPossibleMatches(&currentUser, &users);

	resultMsg = userParser.ListToCsv(filtered_users);
	while(!users.empty()){
		delete users.front();
		users.front() = NULL;
		users.pop_front();
	}

	while(!filtered_users.empty()){
		//Ya los eliminste en el anterior
		filtered_users.pop_front();
	}
	cout << "Filtered deleted\n";
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
	bool posted = ssClient.postUsers(user_created, &parsed_user);

	if (posted){
		string id, mail, user_csv;
		jsonParser.parsing(user_created);
		Json::Value json_user =jsonParser.getValue(USER_KEY);
		id = json_user[ID_KEY].asString();

		userParser.JsonToCsvFull(json_user, user_csv, new_user.getDescription());
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

	usersDB->getEntry(USER_CSV_DB + username, base_user);
	csvParser.makePutUser(user_data, base_user, new_user);

	Json::Value jsonUser = jsonParser.userToJson(&new_user,true);
	Json::Value data_to_post;
	data_to_post[META_KEY][VERSION_KEY] = VERSION_VALUE;
	data_to_post[USER_KEY] = jsonUser;
	cout << jsonParser.getAsString(data_to_post) << endl;
	LOGG(INFO) << "Updating info of " + id ;

	return ssClient.changeUser(id, jsonParser.getAsString(data_to_post).c_str());
}

bool MessageHandler::deleteUser() {
	string id = this->getId(), reply;

	if ( ssClient.deleteUser(id, &reply )){
		usersDB->deleteEntry(USER_DB + username);
		usersDB->deleteEntry(USER_ID_DB + username);
		usersDB->deleteEntry(USER_LOOKING_DB + username);
		usersDB->deleteEntry(USER_CSV_DB + username);
		usersDB->deleteEntry(USER_PHOTO_DB + username);
		//TODO: Delete matches, friends, y si ambos no estan chat
		LOGG(INFO) << "User Deleted " + username;
		return true;
	}
	return false;
}

bool MessageHandler::getInterestPhoto(std::string& photo_64, std::string id_interest) {
	return ssClient.getInterestPhoto(id_interest, photo_64);
}

bool MessageHandler::getChat(std::string other_username, string& chat_history) {
	return chatDB->getHistory(username, other_username, chat_history);
}

bool MessageHandler::getPhoto(std::string other_username, string& photo_64) {
	string user_id, photo;
	if (usersDB->getEntry(USER_PHOTO_DB + other_username, photo)){
		photo_64 = photo;
		return true;
	}else{
		if ( ! usersDB->getEntry(USER_ID_DB + other_username, user_id)){
			return false;
		}
		return ssClient.getUserPhoto(this->getId(), photo_64);
	}
}

bool MessageHandler::postPhoto(string photo_64) {
	LOGG(INFO) << "Updating photo of " + this->getId();
	if (ssClient.changeUserPhoto(this->getId(), photo_64)){
		usersDB->addEntry(USER_PHOTO_DB + username, photo_64);
		return true;
	}
	return false;
}


bool MessageHandler::validateToken(std::string user_token) {
	return ! this->tokenizer->hasExpired(user_token);
}


void MessageHandler::getMatches(std::string id) {
	LOGG(INFO) << "Devolviendo usuarios matcheados.";
	string matches;
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
	string password, token;
	LOGG(INFO) << "Generating new token for " + username;
	this->usersDB->getEntry(USER_DB + username, password);
	return this->tokenizer->newToken(username, password);
}

bool MessageHandler::addLocalization(string localization) {
	int i = localization.find(SEPARATOR);
	string latitude = localization.substr(0, i);
	string longitude = localization.substr(i+1, localization.length());
	//TODO: Refactor!
	CsvParser csvParser;
	JsonParser jsonParser;
	User new_user;
	string id = this->getId(), base_user;

	usersDB->getEntry(USER_CSV_DB + username, base_user);
	csvParser.makeUser(base_user, new_user);

	Json::Value jsonUser = jsonParser.userToJson(&new_user,true);
	Json::Value data_to_post;
	data_to_post[META_KEY][VERSION_KEY] = VERSION_VALUE;
	data_to_post[USER_KEY] = jsonUser;

	LOGG(DEBUG) << "Updating info of " + id ;

	return ssClient.changeUser(id, jsonParser.getAsString(data_to_post));
}

bool MessageHandler::getUser(string username, string &user_data) {
	//busca en DB
	if (! usersDB->getEntry(USER_CSV_DB + username, user_data)){
		string id;
		if ( !usersDB->getEntry(USER_ID_DB + username, id)){
			return false;
		}
		//busca en SHARED
		bool ok = ssClient.getUser(id, &user_data);
		if (ok) {
			UserParser parser;
			JsonParser json;
			string copy(user_data), lookingFor = "";
			json.parsing(copy);
			usersDB->getEntry(USER_LOOKING_DB + username, lookingFor);
			parser.JsonToCsvFull(json.getRoot(), user_data, lookingFor);
			usersDB->addEntry(USER_CSV_DB + username, user_data);
			usersDB->addEntry(USER_PHOTO_DB + username, json.getValue(USER_KEY)[PHOTO_KEY].asString());
		}
		return ok;
	}
	CsvParser csv;
	csv.removeId(user_data);
	return true;

}

bool MessageHandler::match(string &users, string &candidate_data) {
	size_t i = users.find(DB_SEPARATOR);
	string user1 = users.substr(0, i), user2 = users.substr(i+1, users.length());

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







