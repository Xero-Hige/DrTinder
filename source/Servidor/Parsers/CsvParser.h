/*
 * CsvParser.h
 *
 *  Created on: Jun 16, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_PARSERS_CSVPARSER_H_
#define SERVIDOR_PARSERS_CSVPARSER_H_

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include "../User.h"
#include "../../libs/loger/easylogging++.h"

#define ID_FULL_IDX 0
#define NAME_FULL_IDX 1
#define AGE_FULL_IDX 2
#define ALIAS_FULL_IDX 3
#define MAIL_FULL_IDX 4
#define SEX_FULL_IDX 5
#define DSC_FULL_IDX 6
#define INT_FULL_IDX 7
#define LOCX_FULL_IDX 8
#define LOCY_FULL_IDX 9
#define USER_DATA_COUNT 10
#define NAME_IDX 0
#define AGE_IDX 1
#define ALIAS_IDX 2
#define MAIL_IDX 3
#define SEX_IDX 4
#define DSC_IDX 5
#define INT_IDX 6
#define USER_DATA_FOR_CLIENT_COUNT 7
#define PUT_SHUFF 2

using namespace std;

class CsvParser {
public:
	CsvParser();
	vector<string> parseLine(string *line);
	void makeInterests(string keyValues, Interests &interests);
	void makeUser(string user_str, User &user);
	void makeSignupUser(string user_str, User &user);
	void makePutUser(string user_str, string base_user, User &user);
	string interestToCsv(Interests *interests);
	string userToCsvFull(User *user);
	string userToCsvForClient(User *user, bool with_id=false);
	void removeId(std::string &data);
	virtual ~CsvParser();
private:
	void splitInterest(string cell, string &key, string &value);
};

#endif /* SERVIDOR_PARSERS_CSVPARSER_H_ */
