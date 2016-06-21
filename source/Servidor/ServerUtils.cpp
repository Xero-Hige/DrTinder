#include "ServerUtils.h"

void intToString(int number, std::string& result) {
    std::ostringstream convert;
    convert << number;
    result = convert.str();
}

int is_equal(const struct mg_str *s1, const std::string s2) {
    return mg_vcmp(s1, s2.c_str()) == 0;
}