/*
 * JsonParser.cpp
 *
 *  Created on: Jun 16, 2016
 *      Author: tobias
 */

#include "JsonParser.h"

using namespace std;

JsonParser::JsonParser() {
	// TODO Auto-generated constructor stub

}
bool JsonParser::parsing(string json_object){
	return reader.parse(json_object, root, false);
}

void JsonParser::makeInterests(Json::Value json_interests,Interests &interests){
	Json::Value array;
	JsonParser json;
	if (! json_interests.isArray()){
		 array = json_interests[INTERESTS_KEY];
	}else{
		array = json_interests;
	}

	Json::Value::iterator it = array.begin();
	for (; it != array.end(); ++it){
		Json::Value interest = (*it);
		interests.add(interest[CATEGORY_KEY].asString(), interest[VALUE_KEY].asString());
	}


}

void JsonParser::makeUser(Json::Value json_user,User &user){
	Json::Value location = json_user[LOCATION_KEY];
	Json::Value json_interests = json_user[INTERESTS_KEY];
	JsonParser pars;
	if (json_user[PHOTO_KEY].isNull()){
		user.setPhoto("");
	}else{
		user.setPhoto(json_user[PHOTO_KEY].asString());
	}


	Interests interests;
	this->makeInterests(json_interests, interests);
	user.setInterests(interests);

	user.setAlias(json_user[ALIAS_KEY].asString());

	user.setMail(json_user[MAIL_KEY].asString());

	user.setName(json_user[NAME_KEY].asString());

	user.setSex(json_user[SEX_KEY].asString());

	if (json_user.isMember(ID_KEY)){

		user.setId(json_user[ID_KEY].asInt());
	}

	user.setAge(json_user[AGE_KEY].asInt());
	user.setLocation(location[LATITUDE_KEY].asFloat(), location[LONGITUDE_KEY].asFloat());

}

Json::Value JsonParser::interestsToJson(Interests* interests){
	Json::Value interests_json;
	map<string,vector<string>> inter = interests->allInterests();
	map<string,vector<string>>::iterator it = inter.begin();

	for(; it != inter.end(); ++it){
		string key = it->first;
		for (unsigned int i = 0; i < it->second.size(); i++){
			Json::Value pair;
			string value = it->second[i];
			pair[CATEGORY_KEY] = key;
			pair[VALUE_KEY] = value;
			interests_json.append(pair);
		}
	}

	return interests_json;
}

Json::Value JsonParser::userToJson(User *user, bool id){
	Json::Value user_json;
	Json::Value location;

	if (id){
		user_json[ID_KEY] = user->getID();
	}

	user_json[ALIAS_KEY] = user->getAlias();
	user_json[NAME_KEY] = user->getName();
	user_json[MAIL_KEY] = user->getMail();
	user_json[PHOTO_KEY] = user->getPhoto();
	user_json[SEX_KEY] = user->getSex();
	user_json[AGE_KEY] = user->getAge();
	user_json[INTERESTS_KEY] = interestsToJson(user->getInterests());

	location[LATITUDE_KEY] = user->getX();
	location[LONGITUDE_KEY] = user->getY();

	user_json[LOCATION_KEY] = location;
	return user_json;
}

string JsonParser::photoToJson(string *photo){
	Json::Value json;
	json[PHOTO_PUT_KEY] = *photo;
	return writer.write(json);
}

Json::Value JsonParser::getValue( string key ){
	return root[key];
};

string JsonParser::getAsString(Json::Value value){
	return writer.write(value);
}
JsonParser::~JsonParser() {
	// TODO Auto-generated destructor stub
}

