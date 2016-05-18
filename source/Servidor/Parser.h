#ifndef SOURCE_PARSER_H
#define SOURCE_PARSER_H


#include <string>
#include <map>
#include <exception>
#include "../libs/jsoncpp/dist/json/json.h"
#include "api_constants_shared.h"

class Parser {
    public:
        Parser();
        ~Parser();
        int parse(std::string message, std::string* parameters_names, std::map* parameters);
        std::string makeMessage(std::string verb, std::map* parameters);
        void parseArray(Json::Value node, std::string* parameters_names, std::map* parameters);
        void parseObject(Json::Value node, std::string* parameters_names, std::map* parameters);
        std::string parseValue(Json::Value node, std::string parameter_name);
        private:
        Json::Value root;
        Json::Reader reader;
};


#endif //SOURCE_PARSER_H
