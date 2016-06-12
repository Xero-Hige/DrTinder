/*
 * chatHandler.cpp
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#include "chatHandler.h"

chatHandler::chatHandler() {
	// TODO Auto-generated constructor stub
	this->uri = CHAT_URI;
}

HttpResponse chatHandler::httpGet(struct http_message *hm){
	HttpResponse resp;
	Parser parser;
	int friend_idx = 1, token_idx = 2;
	std::string friends, token;
	std::string url = parser.getUri(hm);
	if ( ( !parser.urlAt(url,friend_idx,friends )) || ( !parser.urlAt(url,token_idx,token ))){

		resp.turnToBadRequest("No hay friend o token el url");

	} else {
		//TODO validar token y buscar historial
		resp.setStatus(200);
		resp.setBody("pepe,hola\njuana,hola\njuana,como estas\n");
	}
	return resp;
}


chatHandler::~chatHandler() {
	// TODO Auto-generated destructor stub
}

