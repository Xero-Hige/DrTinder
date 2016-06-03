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
#include <map>
#include <time.h>
#include <ctime>
#include <openssl/md5.h>

#define EXPIRATION_TIME 5

class Tokenizer {
public:
	Tokenizer();
	virtual ~Tokenizer();
	std::string newToken(std::string mail,std::string pass);
	bool hasExpired(std::string token);
	void remove(std::string token);
private:
	time_t getNow();
	std::map<std::string,time_t> tokens;

};

#endif /* SERVIDOR_TOKENIZER_H_ */
