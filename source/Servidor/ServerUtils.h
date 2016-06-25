#ifndef SOURCE_SERVERUTILS_H
#define SOURCE_SERVERUTILS_H

#include <string>
#include <sstream>
#include <mongoose.h>

void intToString(int number, std::string& result);
int is_equal(const struct mg_str *s1, const std::string s2);
void configure(int argc, char **argv,std::string &port, std::string &shared, std::string &logCnfg);
#endif //SOURCE_SERVERUTILS_H
