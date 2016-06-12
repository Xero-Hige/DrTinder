/*
 * friendsHandler.cpp
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#include "friendsHandler.h"

friendsHandler::friendsHandler() {
	// TODO Auto-generated constructor stub
	this->uri = FRIENDS_URI;
}

HttpResponse friendsHandler::httpGet(struct http_message *hm){

	HttpResponse resp;
	std::string url = parse.getUri(hm);
	std::string token;
	if (! parse.urlAt(url,1,token)){
		resp.turnToBadRequest("No token in url");
	} else {
		//TODO matches existentes
		resp.setStatus(200);
		resp.setBody("Devolver friends");
	}
	return resp;

}

friendsHandler::~friendsHandler() {
	// TODO Auto-generated destructor stub
}

