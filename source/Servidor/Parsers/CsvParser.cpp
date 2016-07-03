/*
 * CsvParser.cpp
 *
 *  Created on: Jun 16, 2016
 *      Author: tobias
 */

#include "CsvParser.h"

using namespace std;

CsvParser::CsvParser() {

}

vector<string> CsvParser::parseLine(string *line){
	vector<string> elements;
	stringstream ss(*line);

	string cell;

	if(line->empty() || line->size() <= 2){
		return elements;
	}

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

    	elements.push_back(cell);
    }
    return elements;

}

void CsvParser::splitInterest(string cell, string &key, string &value){
	stringstream ss(cell);
	string temp;
	LOGG(DEBUG) << "Spliting interest " << cell;
	while (ss.good()){
		getline(ss, temp, ':');

		if (ss.peek() == ':'){
			ss.ignore();
			key = temp;
		}
		value = temp;
	}
	LOGG(DEBUG)<< "Splited in "<< key << "," << value;
}

void CsvParser::makeInterests(string keyValues, Interests &interests){
	string key,value;

	if (keyValues.back() == '\"') {
		keyValues.pop_back();
	}

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

bool CsvParser::makeUser(string user_str, User &user){
	vector<string> parsed = this->parseLine(&user_str);
	Interests interests;
	int desf = 0;
	int count_elem = parsed.size();

	if ( (count_elem < USER_DATA_COUNT - 1) || (count_elem > USER_DATA_COUNT)){
		return false;
	}

	if (parsed.size() == USER_DATA_COUNT){
		string idS = parsed[ID_FULL_IDX];
		if ( ! isInteger(idS)){
			return false;
		}
		int id = stoi(idS);
		user.setId(id);
		desf = 1;
	}else{
		LOGG(WARNING) << "Parsing user without ID";
	}
	string xS = parsed[LOCX_FULL_IDX - 1 + desf];
	string yS = parsed[LOCY_FULL_IDX - 1 + desf];
	string age = parsed[AGE_IDX + desf];


	if (! isFloat(xS) || ! isFloat(yS) || ! isInteger(age)){
		return false;
	}

	string name = parsed[NAME_IDX + desf];
	string alias = parsed[ALIAS_IDX + desf];
	string sex = parsed[SEX_IDX + desf];
	string mail = parsed[MAIL_IDX + desf];
	float x =stof(xS);
	float y =stof(yS);

	user.setCommonData(mail,alias,name,sex);
	user.setLocation(x,y);
	user.setAge(stoi(age));
	user.setDescription(parsed[DSC_IDX + desf]);

	this->makeInterests(parsed[INT_IDX + desf], interests);
	user.setInterests(interests);

	return true;

}

bool CsvParser::makeSignupUser(string user_str,User &user){
	vector<string> parsed = this->parseLine(&user_str);

	if (parsed.size() != USER_DATA_FOR_CLIENT_COUNT){
		return false;
	}

	Interests interests;
	string age = parsed[AGE_IDX];
	string name = parsed[NAME_IDX];
	string alias = parsed[ALIAS_IDX];
	string sex = parsed[SEX_IDX];
	string mail = parsed[MAIL_IDX];

	if (! isInteger(age)){
		return false;
	}

	user.setCommonData(mail, alias, name, sex);

	//defualt location
	user.setLocation(0, 0);


	user.setAge(stoi(age));
	user.setDescription(parsed[DSC_IDX]);

	this->makeInterests(parsed[INT_IDX], interests);
	user.setInterests(interests);
	return true;
}

bool CsvParser::makePutUser(string user_str, string base_user, User& user){
	vector<string> parsedOriginal = this->parseLine(&base_user);
	vector<string> modifications = this->parseLine(&user_str);
	vector<string> parsed = this->parseLine(&user_str);

	if ( modifications.size() > (USER_DATA_FOR_CLIENT_COUNT - PUT_SHUFF)){
		return false;
	}
	//FromOriginal
	string idS = parsedOriginal[ID_FULL_IDX];
	string alias = parsedOriginal[ALIAS_FULL_IDX];
	string mail = parsedOriginal[MAIL_FULL_IDX];
	string xS = parsedOriginal[LOCX_FULL_IDX];
	string yS = parsedOriginal[LOCY_FULL_IDX];

	//Modifications
	string name = modifications[NAME_IDX];
	string age = modifications[AGE_IDX];

	//Modifications shuffled
	string sex = modifications[SEX_IDX - PUT_SHUFF];
	if ( ! isInteger(age) || ! isInteger(idS)){
		return false;
	}

	if ( ! isFloat(xS) || ! isFloat(yS)){
		return false;
	}
	Interests interests;
	this->makeInterests(modifications[INT_IDX - PUT_SHUFF], interests);

	float x = stof(xS);
	float y = stof(yS);
	int id = stoi(idS);

	user.setInterests(interests);
	user.setLocation(x, y);
	user.setId(id);
	user.setCommonData(mail, alias, name, sex);
	user.setDescription(modifications[DSC_IDX - PUT_SHUFF]);
	user.setAge(stoi(age));

	return true;
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
}

