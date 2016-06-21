/*
 * CsvParser.cpp
 *
 *  Created on: Jun 16, 2016
 *      Author: tobias
 */

#include "CsvParser.h"

using namespace std;

CsvParser::CsvParser() {
	// TODO Auto-generated constructor stub

}

vector<string> CsvParser::parseLine(string *line){
	vector<string> elements;
	stringstream ss(*line);

	string cell;
    while( ss.good() )
    {
    	getline( ss, cell, ',' );
    	cell = cell.substr(1, cell.length() );
    	while (*cell.rbegin() != '"' && ss.good() ){
    		string other_parts;
    		getline( ss, other_parts, ',' );
    		cell = cell + other_parts;
    	}

    	cell = cell.substr( 0 , cell.length() - 1);
    	//cout << cell.c_str() << endl;
    	elements.push_back(cell);
    }
    return elements;

}

void CsvParser::splitInterest(string cell, string &key, string &value){
	stringstream ss(cell);
	string temp;

	while (ss.good()){
		getline(ss, temp, ':');

		if (ss.peek() == ':'){
			ss.ignore();
			key = temp;
		}
		value = temp;
	}

}

void CsvParser::makeInterests(string keyValues, Interests &interests){
	string key,value;
	stringstream ss(keyValues);

	string cell;
	while (ss.good()) {
		getline(ss, cell, '|');

		if (ss.peek() == '|'){
			ss.ignore();
		}

		this->splitInterest(cell, key, value);
		//cout << key.c_str() << "," << value.c_str() << endl;
		interests.add(key,value);
	}

}

void CsvParser::makeUser(string user_str, User &user){
	vector<string> parsed = this->parseLine(&user_str);
	Interests interests;

	string name = parsed[NAME_IDX];
	string alias = parsed[ALIAS_IDX];
	string sex = parsed[SEX_IDX];
	string mail = parsed[MAIL_IDX];

	user.setCommonData(mail,alias,name,sex);

	float x =stof(parsed[LOCX_IDX]);
	float y =stof(parsed[LOCY_IDX]);

	user.setLocation(x,y);

	string age = parsed[AGE_IDX];
	user.setAge(stoi(age));
	user.setDescription(parsed[DSC_IDX]);

	this->makeInterests(parsed[INT_IDX], interests);
	user.setInterests(interests);

	if (parsed.size() == USER_DATA_COUNT){
		//hay que asegurarse q ID_IDX sea el ultimo, medio pelo
		int id = stoi(parsed[ID_IDX]);
		user.setId(id);
		user.setPhoto(parsed[PHOTO_IDX]);
	}

}

string CsvParser::userToCsv(User *user){
	string user_str[USER_DATA_COUNT];
	string parsed = "\"";

	user_str[NAME_IDX] = user->getName();
	user_str[MAIL_IDX]= user->getMail();
	user_str[ALIAS_IDX] = user->getAlias();
	user_str[SEX_IDX] = user->getSex();
	user_str[AGE_IDX] = to_string(user->getAge());
	user_str[ID_IDX] = to_string(user->getID());
	user_str[DSC_IDX] = user->getDescription();
	user_str[LOCX_IDX] = to_string(user->getX());
	user_str[LOCY_IDX] = to_string(user->getY());
	user_str[INT_IDX] = this->interestToCsv(user->getInterests());
	user_str[PHOTO_IDX] = user->getPhoto();

	for (int i = 0; i < USER_DATA_COUNT; i++){
		parsed+= user_str[i] + "\",\"";
	}
	parsed = parsed.substr(0,parsed.length() - 2);

	return parsed;
}

string CsvParser::interestToCsv(Interests * interests){
	string str = "";
	map<string,vector<string>> inter = interests->allInterests();
	map<string,vector<string>>::iterator it = inter.begin();

	for(; it != inter.end(); ++it){
		string key = it->first;
		for (unsigned int i = 0; i < it->second.size(); i++){
			string value = it->second[i];
			str += key + "::" + value + "||";
		}
	}

	str = str.substr(0, str.length() - 2 );

	return str;
}

CsvParser::~CsvParser() {
	// TODO Auto-generated destructor stub
}

