#ifndef SOURCE_PARSER_H
#define SOURCE_PARSER_H


#include <string>
#include <exception>
#include "../libs/jsoncpp/dist/json/json.h"
#include "api_constants_shared.h"
#include "../libs/mongoose/mongoose.h"

class Parser {
    public:
        Parser();
        ~Parser();
        int parse(std::string message, std::string* parameters_names, std::string& values);
        std::string makeMessage(std::string verb, std::string* parameters);
        void parseArray(Json::Value node, std::string* parameters_names, std::string& values);
        void parseObject(Json::Value node, std::string* parameters_names, std::string& values);
        std::string parseValue(Json::Value node, std::string parameter_name);
        std::string getBody(struct http_message *hm);
        std::string getUri(struct http_message *hm);
        std::string getMethod(struct http_message *hm);
        std::string getUriStart(struct http_message *hm);
    protected:
        void parse(Json::Value node, std::string *parameters_names, std::string& values);
        std::string deleteFromFirstSpace(const char * line);
    private:
        Json::Value root;
        Json::Reader reader;


};


#endif //SOURCE_PARSER_H
