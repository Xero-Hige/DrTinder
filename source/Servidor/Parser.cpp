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

