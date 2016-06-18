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

#define NAME_IDX 0
#define AGE_IDX 1
#define ALIAS_IDX 2
#define DSC_IDX 3
#define INT_IDX 4
#define MAIL_IDX 5
#define SEX_IDX 6
#define LOCX_IDX 7
#define LOCY_IDX 8
#define PHOTO_IDX 9
#define ID_IDX 10
#define USER_DATA_COUNT 11


using namespace std;

class CsvParser {
public:
	CsvParser();
	vector<string> parseLine(string *line);
	void makeInterests(string keyValues, Interests &interests);
	void makeUser(string user_str, User &user);
	string interestToCsv(Interests *interests);
	string userToCsv(User *user);

	virtual ~CsvParser();
private:
	void splitInterest(string cell, string &key, string &value);
};

#endif /* SERVIDOR_PARSERS_CSVPARSER_H_ */
