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

UserParser::~UserParser() {
	// TODO Auto-generated destructor stub
}

