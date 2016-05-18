#include "Parser.h"

using std::string;
using std::map;

Parser::Parser() {
}

Parser::~Parser() {
}

int Parser::parse(string message, string* parameters_names, map* parameters) {
    bool parsedSuccess = reader.parse(message, root, false);

    if(not parsedSuccess) {
        throw std::invalid_argument(reader.getFormattedErrorMessages());
    }
    if (root.isArray()) {
        parseArray(root, parameters_names, parameters);
    } else if (root.isObject()) {
        parseObject(root, parameters_names, parameters);
    } else {
        parseValue(root, parameters_names[0]);
    }


}

string Parser::parseValue(Json::Value node, string parameter_name) {
    return node[parameter_name].asString();
}

void Parser::parseArray(Json::Value node, string* parameters_names, std::map* parameters) {
//    const Json::Value array = node[parameters_names[0]];
//    for(unsigned int i = 0; i < array.size(); ++i) {
//        Json::Value element = array[i];
//        if (element.isArray()) {
//            parseArray(element, )
//        }
//    }
}

string Parser::makeMessage(string verb, map* parameters) {
//    string message = "{" + verb;
}

