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
	int desf = 0;
	if (parsed.size() == USER_DATA_COUNT){
		int id = stoi(parsed[ID_FULL_IDX]);
		user.setId(id);
		desf = 1;
	}else{
		LOGG(WARNING) << "Parsing user without ID";
	}

	string name = parsed[NAME_IDX + desf];
	string alias = parsed[ALIAS_IDX + desf];
	string sex = parsed[SEX_IDX + desf];
	string mail = parsed[MAIL_IDX + desf];

	user.setCommonData(mail,alias,name,sex);

	float x =stof(parsed[LOCX_FULL_IDX]);
	float y =stof(parsed[LOCY_FULL_IDX]);

	user.setLocation(x,y);

	string age = parsed[AGE_IDX + desf];
	user.setAge(stoi(age));
	user.setDescription(parsed[DSC_IDX + desf]);

	this->makeInterests(parsed[INT_IDX + desf], interests);
	user.setInterests(interests);



}
void CsvParser::makeSignupUser(string user_str,User &user){
	vector<string> parsed = this->parseLine(&user_str);
	Interests interests;

	string name = parsed[NAME_IDX];
	string alias = parsed[ALIAS_IDX];
	string sex = parsed[SEX_IDX];
	string mail = parsed[MAIL_IDX];

	user.setCommonData(mail, alias, name, sex);

	//defualt location
	user.setLocation(0, 0);

	string age = parsed[AGE_IDX];
	user.setAge(stoi(age));
	user.setDescription(parsed[DSC_IDX]);

	this->makeInterests(parsed[INT_IDX], interests);
	user.setInterests(interests);

}

void CsvParser::makePutUser(string user_str, string base_user, User& user){
	vector<string> parsedOriginal = this->parseLine(&base_user);
	vector<string> modifications = this->parseLine(&user_str);
	vector<string> parsed = this->parseLine(&user_str);

	if (modifications.size() > USER_DATA_FOR_CLIENT_COUNT - PUT_SHUFF){
		LOGG(WARNING) << "Bad line to parse user";
	}
	//FromOriginal
	int id = stoi(parsedOriginal[ID_FULL_IDX]);
	user.setId(id);
	string alias = parsedOriginal[ALIAS_FULL_IDX];
	string mail = parsedOriginal[MAIL_FULL_IDX];
	float x = stof(parsedOriginal[LOCX_FULL_IDX]);
	float y = stof(parsedOriginal[LOCY_FULL_IDX]);
	user.setLocation(x, y);

	//Modifications
	string name = modifications[NAME_IDX];
	string age = modifications[AGE_IDX];
	user.setAge(stoi(age));

	//Modifications shuffled
	string sex = modifications[SEX_IDX - PUT_SHUFF];
	user.setCommonData(mail, alias, name, sex);
	user.setDescription(modifications[DSC_IDX - PUT_SHUFF]);

	Interests interests;
	this->makeInterests(modifications[INT_IDX - PUT_SHUFF], interests);
	user.setInterests(interests);

}

string CsvParser::userToCsvFull(User *user){
	string user_str[USER_DATA_COUNT];
	string parsed = "\"";

	user_str[NAME_FULL_IDX] = user->getName();
	user_str[MAIL_FULL_IDX]= user->getMail();
	user_str[ALIAS_FULL_IDX] = user->getAlias();
	user_str[SEX_FULL_IDX] = user->getSex();
	user_str[AGE_FULL_IDX] = to_string(user->getAge());
	user_str[ID_FULL_IDX] = to_string(user->getID());
	user_str[DSC_FULL_IDX] = user->getDescription();
	user_str[LOCX_FULL_IDX] = to_string(user->getX());
	user_str[LOCY_FULL_IDX] = to_string(user->getY());
	user_str[INT_FULL_IDX] = this->interestToCsv(user->getInterests());

	for (int i = 0; i < USER_DATA_COUNT; i++){
		parsed+= user_str[i] + "\",\"";
	}
	parsed = parsed.substr(0,parsed.length() - 2);

	return parsed;
}

string CsvParser::userToCsvForClient(User *user, bool with_id){
	int addId = 0;

	if (with_id){
		addId = 1;
	}

	string user_str[USER_DATA_FOR_CLIENT_COUNT + addId];
	string parsed = "\"";

	user_str[NAME_IDX + addId] = user->getName();
	user_str[MAIL_IDX + addId]= user->getMail();
	user_str[ALIAS_IDX + addId] = user->getAlias();
	user_str[SEX_IDX + addId] = user->getSex();
	user_str[AGE_IDX + addId] = to_string(user->getAge());
	user_str[DSC_IDX + addId] = user->getDescription();
	user_str[INT_IDX + addId] = this->interestToCsv(user->getInterests());

	if (with_id)
		user_str[ID_FULL_IDX] = to_string(user->getID());

	for (int i = 0; i < USER_DATA_FOR_CLIENT_COUNT + addId; i++){
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

void CsvParser::removeId(string &data){
	vector<string> parsed = this->parseLine(&data);
	string user_str;
	parsed.erase(parsed.begin() + ID_FULL_IDX);

	user_str = "\"";
	for (unsigned int i = 0; i < parsed.size(); i++){
		user_str+= parsed[i] + "\",\"";
		}
	data = user_str.substr(0,user_str.length() - 2);


}

CsvParser::~CsvParser() {
	// TODO Auto-generated destructor stub
}

