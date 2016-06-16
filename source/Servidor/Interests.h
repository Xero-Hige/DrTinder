/*
 * Interests.h
 *
 *  Created on: Jun 2, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_INTERESTS_H_
#define SERVIDOR_INTERESTS_H_
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "dataTypes.h"

using namespace std;

class Interests {
public:
	Interests();
	void importFrom( std::string interest, dataType type);
	void add(std::string category, std::string value);
	bool has(std::string category, std::string value);
	bool getAll(std::string category, std::vector<std::string> &values);
	void remove(std::string category, std::string value);
	map<string,vector<string>> allInterests();
	virtual ~Interests();
private:

	map<string,vector<string>> intereses;
};

#endif /* SERVIDOR_INTERESTS_H_ */
