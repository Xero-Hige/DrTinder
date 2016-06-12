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
	HttpResponse resp;
	Parser parse;
	std::string id, token;
	std::string url = parse.getUri(hm);

	if ( parse.urlAt(url,1,token) ){

		bool get_token = parse.urlAt(url,2,id);

		resp.setStatus(200);
		if ( token.compare("token") == 0){
			resp.setBody("geting token for " + id);
			//TODO generate token for id
			//resp = this->generateToken(id);
		} else if (!get_token){
			//TODO get user data
			//resp = this->getUserData(token);
			resp.setBody("geting user data of " + token);
		}else{
			resp.turnToBadRequest("Hay datos de mas en la url");
		}

	}else{
		resp.turnToBadRequest("No indica que quiere hacer en la url");
	}

	return resp;

}

HttpResponse userHandler::httpPost(http_message *hm){
	Parser parse;
	HttpResponse resp;
	std::string url = parse.getUri(hm);
	std::string id;

	if (!parse.urlAt(url,1,id)){
		resp.turnToBadRequest("Faltan parametros en la url");
	}
	else{
		std::string pass = parse.getBody(hm);
		//TODO login
		//resp = this->login(id,pass);
		resp.setStatus(200);
		resp.setBody(id + "," + pass);
	}

	return resp;
}
