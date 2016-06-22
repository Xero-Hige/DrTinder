/*
 * UserParser.cpp
 *
 *  Created on: Jun 21, 2016
 *      Author: tobias
 */

#include "UserParser.h"

UserParser::UserParser() {
	// TODO Auto-generated constructor stub

}
void UserParser::JsonToCsvFull(Json::Value parsing, std::string &parsed, std::string lookingFor){
	User user;
	JsonParser json;
	CsvParser csv;
	json.makeUser(parsing, user);

	user.setDescription(lookingFor);

	parsed = csv.userToCsvFull(&user);
}

void UserParser::CsvToJsonFull(std::string parsing, Json::Value &parsed, bool with_id){
	User user;
	JsonParser json;
	CsvParser csv;
	csv.makeUser(parsing,user);

	parsed = json.userToJson(&user, with_id);
}

list<User *> UserParser::JsonToList(std::string users){
	JsonParser json;
	list<User *> list_users;
	json.parsing(users);
	Json::Value array_users = json.getValue(USERS_KEY);
	Json::Value::iterator it = array_users.begin();
	for (; it != array_users.end(); ++it) {
		Json::Value object = (*it);
		Json::Value user = object[USER_KEY];
		User * user_ = new User();
		json.makeUser(user,*user_);
		list_users.push_back(user_);
	}
	return list_users;
}

string UserParser::ListToCsv(list<User *> user_list){
	list<User*>::iterator it = user_list.begin();
	CsvParser csv;
	string users = "";
	for (; it != user_list.end(); ++it){
		users += csv.userToCsvFull((*it)) + "\n";
	}
	return users;
}
UserParser::~UserParser() {
	// TODO Auto-generated destructor stub
}

