#ifndef SOURCE_SERVERUTILS_H
#define SOURCE_SERVERUTILS_H

#include <string>
#include <sstream>
#include <fstream>
#include <mongoose.h>
#include "../libs/loger/easylogging++.h"
#include "Parsers/JsonParser.h"

void intToString(int number, std::string& result);
int is_equal(const struct mg_str *s1, const std::string s2);
bool isFloat( std::string string );
bool isInteger(std::string &string);
void configure(int argc, char **argv,std::string &port, std::string &shared);



#endif //SOURCE_SERVERUTILS_H
