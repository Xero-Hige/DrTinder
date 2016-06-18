/*
 * Tokenizer.cpp
 *
 *  Created on: Jun 3, 2016
 *      Author: tobias
 */

#include "Tokenizer.h"

using namespace std;

Tokenizer::Tokenizer(DatabaseManager *database) {
	this->database = database;

}

//format year,month,day,hours,min,sec
bool Tokenizer::timeStampExpired(std::string timeStamp){
	vector<string> times;
	time_t now = time(NULL);

	stringstream ss(timeStamp);
	string cell;
	while (ss.good()) {
		getline(ss, cell, ',');
		times.push_back(cell);
	}
	if (times.size() != TS_COUNT){
		//TODO error
	}
	struct tm _then = {0};
	_then.tm_hour = stoi(times[TS_HOUR]) ;
	_then.tm_min = stoi(times[TS_MIN]);
	_then.tm_sec = stoi(times[TS_SEC]);
	_then.tm_year = stoi(times[TS_YEAR]);
	_then.tm_mon = stoi(times[TS_MON]);
	_then.tm_mday = stoi(times[TS_DAY]);

	double elapsedTime = difftime(now, mktime(&_then));

	return elapsedTime > (double)EXPIRATION_TIME;

}

std::string Tokenizer::getNowTimeStamp(){
	time_t now = time(NULL);
	struct tm * gmt = localtime(&now);
	std::string year, month, day, hours, min, sec;
	year = to_string(gmt->tm_year);
	month = to_string(gmt->tm_mon);
	day = to_string(gmt->tm_mday);
	hours = to_string(gmt->tm_hour);
	min = to_string(gmt->tm_min);
	sec = to_string(gmt->tm_sec);

	std::string timeStamp = year + "," + month + ",";
	timeStamp += day + "," + hours + "," + min + ",";
	timeStamp += sec;
	return timeStamp;
}

std::string Tokenizer::newToken(std::string mail, std::string pass){
	//hash
    unsigned char digest[16];
    string timeStamp = getNowTimeStamp();

	std::string toHash = mail + pass + timeStamp;
    const char* string = toHash.c_str();
    const unsigned char* unsignedString = (unsigned char*) string;
    //cout << "string length: " << strlen(string) << endl;

	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, unsignedString, strlen(string));
	MD5_Final(digest, &ctx);

	char mdString[33];
	for (int i = 0; i < 16; i++)
	sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

	//cout << "md5 digest: " << mdString << endl;
	std::string hashed(mdString);

	database->addEntry(hashed,timeStamp);
	return hashed;

}

bool Tokenizer::hasExpired(std::string token){
	std::string timeStamp;
	if (database->getEntry(token,timeStamp)){
		return this->timeStampExpired(timeStamp);
	}
	return true;

}

void Tokenizer::remove(std::string token){
	this->database->deleteEntry(token);
}
Tokenizer::~Tokenizer() {
	// TODO Auto-generated destructor stub
}

