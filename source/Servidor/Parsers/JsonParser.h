/*
 * JsonParser.h
 *
 *  Created on: Jun 16, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_JSONPARSER_H_
#define SERVIDOR_JSONPARSER_H_
#include <string>
#include <iostream>
#include "../../libs/jsoncpp/dist/json/json.h"
#include "../User.h"


#define USERS_KEY "users"
#define USER_KEY "user"
#define ID_KEY "id"
#define NAME_KEY "name"
#define ALIAS_KEY "alias"
#define SEX_KEY "sex"
#define AGE_KEY "age"
#define MAIL_KEY "email"
#define PHOTO_KEY "photo_profile"
#define PHOTO_PUT_KEY "photo"
#define LOCATION_KEY "location"
#define LATITUDE_KEY "latitude"
#define LONGITUDE_KEY "longitude"
#define INTERESTS_KEY "interests"
#define INTEREST_KEY "interest"
#define CATEGORY_KEY "category"
#define VALUE_KEY "value"
#define META_KEY "metadata"
#define COUNT_KEY "count"
#define VERSION_KEY "version"
#define VERSION_VALUE "0.2"

class JsonParser {
public:
	JsonParser();


	bool parsing(std::string json_object);
	std::string getAsString(Json::Value value);
	void makeInterests(Json::Value json_interests, Interests &interests);
	void makeUser(Json::Value json_user, User &user);

	Json::Value userToJson(User *user, bool id=false);
	Json::Value interestsToJson(Interests *interest);
	Json::Value interstToJson(std::string category, std::string value);
	std::string photoToJson(std::string *photo);

	Json::Value getValue(std::string key);
	virtual ~JsonParser();

private:
	Json::Value root;
	Json::Reader reader;
	Json::StyledWriter writer;

};

#endif /* SERVIDOR_JSONPARSER_H_ */
