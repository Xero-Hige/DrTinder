#include "Parser.h"

using std::string;

Parser::Parser() {
}

Parser::~Parser() {
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

	//remove first and last /
	char first_char = *url.begin();
	char last_char = *url.rbegin();

	if ( first_char == '/' ){
		url = url.substr(1, url.length());
	}

	if ( last_char == '/' ){
		url = url.substr(0, url.length() - 1);
	}

	while ((next = url.find("/", last)) != string::npos && (i <= index) ){

		breadCrumb = url.substr(last, next-last);
		last = next + 1;
		i++;

	}

	if (i < index){
		value = "";
		return false;
	}

	if (next == string::npos && i == index){
		breadCrumb = url.substr(last);
	}

	value = breadCrumb;
	return true;
}
