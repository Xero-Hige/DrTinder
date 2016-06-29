/*
 * Tokenizer.h
 *
 *  Created on: Jun 3, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_TOKENIZER_H_
#define SERVIDOR_TOKENIZER_H_
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>
#include <time.h>
#include <ctime>
#include <vector>
#include "../libs/mongoose/mongoose.h"
#include "DatabaseManager.h"
#include "../libs/loger/easylogging++.h"

#define EXPIRATION_TIME 7200 //2h
#define TS_YEAR 0
#define TS_MON 1
#define TS_DAY 2
#define TS_HOUR 3
#define TS_MIN 4
#define TS_SEC 5
#define TS_COUNT 6
#define TOKENIZER_PREFIX "token_"

class Tokenizer {
public:
	Tokenizer(DatabaseManager *data);
	virtual ~Tokenizer();
	/*Generates new Hash*/
	std::string newToken(std::string mail,std::string pass);
	/*Verify if token has expired*/
	bool hasExpired(std::string token);
	/*Remove token from existance*/
	void remove(std::string token);
private:
	/*Get TimeStamp*/
	std::string getNowTimeStamp();
	/*Verifys if timeStamp has expired*/
	bool timeStampExpired(std::string timeStamp);
	DatabaseManager *database;
};

#endif /* SERVIDOR_TOKENIZER_H_ */
