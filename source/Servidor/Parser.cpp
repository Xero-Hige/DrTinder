#include "Parser.h"

using std::string;
using std::map;

Parser::Parser() {
}

Parser::~Parser() {
}

int Parser::parse(string message, string* parameters_names, string& values) {
    bool parsedSuccess = reader.parse(message, root, false);

    if(not parsedSuccess) {
        throw std::invalid_argument(reader.getFormattedErrorMessages());
    }
    parse(root, parameters_names, values);
}

void Parser::parse(Json::Value node, string* parameters_names, string& values) {
    if (node.isArray()) {
        parseArray(node, parameters_names, values);
    } else if (node.isObject()) {
        parseObject(node, parameters_names, values);
    } else {
        parseValue(node, parameters_names[0]);
    }
}

string Parser::parseValue(Json::Value node, string parameter_name) {
    return node[parameter_name].asString();
}

void Parser::parseArray(Json::Value array, string* parameters_names, string& values) {
    for(unsigned int i = 0; i < array.size(); ++i) {
        Json::Value element = array[i];
        parse(element, parameters_names, values);
        values += '\n';
    }
}

void Parser::parseObject(Json::Value object, string *parameters_names, string &values) {
   
}

string Parser::makeMessage(string verb, string* parameters) {
    string message = "{" + verb;
}

std::string Parser::deleteFromFirstSpace(const char * line){
    std::string str(line);
    std::size_t index = str.find(" ");
    if ( index <= 0)
        return str;
    return str.substr(0,index);
}

std::string Parser::getBody(struct http_message *hm){
	std::string str(hm->body.p);
	return str;
}

std::string Parser::getUri(struct http_message *hm){
    return deleteFromFirstSpace(hm->uri.p);
}

std::string Parser::getUriStart(struct http_message *hm){
    std::string str = getUri(hm);
    std::size_t index = str.find("/", 1);
    if ( index <= 0)
        return str;
    return str.substr(1, index - 1 );
}

std::string Parser::getMethod(struct http_message *hm){
    return deleteFromFirstSpace(hm->method.p);
}

bool Parser::urlAt(std::string url, int index, std::string &value){
	int i = 0;
	size_t last = 0;
	size_t next = 0;
	std::string breadCrumb;

	while ((next = url.find("/", last)) != string::npos && (i <= index) ){

		breadCrumb = url.substr(last, next-last);
		last = next + 1;
		i++;

	}

	if (i < index){
		value = "";
		return false;
	}

	if (next == string::npos){
		breadCrumb = url.substr(last);
	}

	value = breadCrumb;
	return true;
}
