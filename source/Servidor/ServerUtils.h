#ifndef SOURCE_SERVERUTILS_H
#define SOURCE_SERVERUTILS_H

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <mongoose.h>
#include "../libs/loger/easylogging++.h"
#include "Parsers/JsonParser.h"
#define DEFAULT_LOG_CONFG "logCnfgs/default.conf"
#define DEBUG_LOG_CONFG "logCnfgs/debug.conf"
#define DEFAULT_CONFG "server.conf"
#define DEFAULT_PORT "8000"
#define DEFAULT_LOG "default"
#define DEBUG_LOG "debug"
#define DEFAULT_SHARED "http://dr-tinder.herokuapp.com"


void intToString(int number, std::string& result);
int is_equal(const struct mg_str *s1, const std::string s2);
bool isFloat( std::string string );
bool isInteger(std::string &string);
void configure(int argc, char **argv,std::string &port, std::string &shared);
void makeUsername(string &username);



#endif //SOURCE_SERVERUTILS_H
