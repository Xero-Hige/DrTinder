/*
 * Handler.cpp
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#include "Handler.h"

Handler::~Handler() {
	this->db = NULL;
	this->sharedConnection = NULL;
}

Handler::Handler() {
	this->mapMethod[GET_S] = GET;
	this->mapMethod[DELETE_S] = DELETE;
	this->mapMethod[POST_S] = POST;
	this->mapMethod[PUT_S] = PUT;

	this->db = NULL;
    this->sharedConnection = NULL;

}

int Handler::getType(std::string method){
	 std::map<std::string,int>::iterator it;

	 it = mapMethod.find(method);
	 if (it != mapMethod.end())
		 return it->second;
	 return -1;

}

HttpResponse Handler::httpRequest(struct http_message *hm){
	Parser parser;
	std::string method = parser.getMethod(hm);
	int type = getType(method);
	switch (type){
	case GET:
		return httpGet(hm);
		break;
	case POST:
		return httpPost(hm);
		break;
	case PUT:
		return httpPut(hm);
		break;
	case DELETE:
		return httpDelete(hm);
		break;
	default:
		return methodNotExist();
	}
}

HttpResponse Handler::httpGet(struct http_message *hm){
	return this->methodNotExist();
}

HttpResponse Handler::httpPut(struct http_message *hm){
	return this->methodNotExist();
}

HttpResponse Handler::httpPost(struct http_message *hm){
	return this->methodNotExist();
}

HttpResponse Handler::httpDelete(struct http_message *hm){
	return this->methodNotExist();
}

bool Handler::manages(std::string uri){
	std::cout << uri.c_str() << "<>" << this->uri.c_str() <<"\n";
	return (uri.compare(this->uri) == 0);
}
