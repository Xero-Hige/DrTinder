/*
 * Tokenizer.cpp
 *
 *  Created on: Jun 3, 2016
 *      Author: tobias
 */

#include "Tokenizer.h"

using namespace std;

Tokenizer::Tokenizer() {
	// TODO Singleton, solo un tokenizer, solo una base de datos, donde deberia estar guardado esto

}

time_t Tokenizer::getNow(){
	time_t now;
	time(&now);
	return now;
}

std::string Tokenizer::newToken(std::string mail, std::string pass){
	//hash
    unsigned char digest[16];
	time_t now = this->getNow();
	char *time = ctime(&now);
	std::string timeStamp(time);
	std::string toHash = mail + pass + timeStamp;
    const char* string = toHash.c_str();

    //cout << "string length: " << strlen(string) << endl;

	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, string, strlen(string));
	MD5_Final(digest, &ctx);

	char mdString[33];
	for (int i = 0; i < 16; i++)
	sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

	//cout << "md5 digest: " << mdString << endl;
	std::string hashed(mdString);
	this->tokens[hashed] = now;
	return hashed;

}

bool Tokenizer::hasExpired(std::string token){
	double timePassed;
	std::map<std::string,time_t>::iterator it = this->tokens.find(token);

	if (it != this->tokens.end()){
		time_t timestamp = it->second;
		time_t now = this->getNow();
		timePassed = difftime(now,timestamp);

		if ( timePassed > EXPIRATION_TIME ){
			this->tokens.erase(it);
			return true;
		}else{
			return false;
		}

	}else{
		return true;
	}
}

void Tokenizer::remove(std::string token){
	std::map<std::string,time_t>::iterator it = this->tokens.find(token);
	if (it != this->tokens.end()){
		this->tokens.erase(it);
	}
}
Tokenizer::~Tokenizer() {
	// TODO Auto-generated destructor stub
}

