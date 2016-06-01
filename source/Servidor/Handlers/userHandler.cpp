/*
 * userHandler.cpp
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#include "userHandler.h"

userHandler::userHandler() {
	// TODO Auto-generated constructor stub
	this->uri = USER_URI;
}

userHandler::~userHandler() {
	// TODO Auto-generated destructor stub
}

HttpResponse userHandler::httpGet(http_message *hm){
	std::cout << "Deberia ser GET\n";
	std::cout << hm->method.p << "\n";
	HttpResponse resp(200,"Entro a user handler, GET");
	return resp;
}

HttpResponse userHandler::httpPost(http_message *hm){
	std::cout << "Deberia ser POST\n";
	std::cout << hm->method.p << "\n";
	HttpResponse resp(200,"Entro a user handler, POST");
	return resp;
}
