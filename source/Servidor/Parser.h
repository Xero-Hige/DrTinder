#ifndef SOURCE_PARSER_H
#define SOURCE_PARSER_H

#include <iostream>
#include <string>
#include <exception>
#include "../libs/jsoncpp/dist/json/json.h"
#include "api_constants_shared.h"
#include "../libs/mongoose/mongoose.h"

class Parser {
    public:
        Parser();
        ~Parser();
        std::string getBody(struct http_message *hm);
        std::string getUri(struct http_message *hm);
        std::string getMethod(struct http_message *hm);
        std::string getUriStart(struct http_message *hm);
        bool urlAt(std::string url, int index, std::string &value);
    protected:
        std::string deleteFromFirstSpace(const char * line);



};


#endif //SOURCE_PARSER_H
