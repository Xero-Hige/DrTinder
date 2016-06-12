/*
 * interestHandler.cpp
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#include "interestHandler.h"

interestHandler::interestHandler() {
	// TODO Auto-generated constructor stub
	this->uri = INTEREST_URI;
}

HttpResponse interestHandler::httpGet(struct http_message *hm){
	HttpResponse resp;
	std::string url = parse.getUri(hm);
	std::string token;
	if (!parse.urlAt(url,1,token)){
		resp.turnToBadRequest("No hay token en el url");
	}else {
		//TODO harcode de imagen
		resp.setBody("devolver imagen interes");
		resp.setStatus(200);
	}
	return resp;
}

interestHandler::~interestHandler() {
	// TODO Auto-generated destructor stub
}

