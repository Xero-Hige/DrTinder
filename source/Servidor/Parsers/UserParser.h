/*
 * UserParser.h
 *
 *  Created on: Jun 21, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_PARSERS_USERPARSER_H_
#define SERVIDOR_PARSERS_USERPARSER_H_
#include <string>
#include "JsonParser.h"
#include "CsvParser.h"

class UserParser {
public:
	UserParser();
	void JsonToCsvFull(Json::Value parsing, std::string &parsed, std::string lookingFor);
	void CsvToJsonFull(std::string parsing, Json::Value &parsed, bool with_id=false);
	virtual ~UserParser();
};

#endif /* SERVIDOR_PARSERS_USERPARSER_H_ */
