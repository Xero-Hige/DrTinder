/*
 * usersHandler.cpp
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#include "usersHandler.h"

usersHandler::usersHandler() {
	// TODO Auto-generated constructor stub
	this->uri = USERS_URI;

}

HttpResponse usersHandler::httpGet(struct http_message *hm){

	HttpResponse resp;
	std::string url = parse.getUri(hm);
	std::string token;
	if (this->urlToken(url,token)){
		resp.setStatus(200);
		if (this->urlPhoto(url)){
			//TODO ss get user data
			resp.setBody("Devolver photo usuario");
		}else{
			//TODO algo matches
			resp.setBody("Devolver matches");
		}

	}else{
		resp.turnToBadRequest("Mal parametros de url");
	}
	return resp;

}

HttpResponse usersHandler::httpPost(struct http_message *hm){
	HttpResponse resp;
	std::string url = parse.getUri(hm);
	std::string token, data = parse.getBody(hm);
	if (this->urlPhoto(url) && this->urlToken(url,token)){
		resp.setStatus(200);
		resp.setBody("Guardar photo usuario " + data + ", para " + token);
		//TODO metodo
	}else if (!this->urlToken(url,token)){
		resp.setStatus(200);
		resp.setBody("Signup de " + data);
		//TODO metodo
	}else{
		resp.turnToBadRequest("Mal parametros de url");
	}
	return resp;

}

HttpResponse usersHandler::httpPut(struct http_message *hm){

	HttpResponse resp;
	std::string url = parse.getUri(hm);
	std::string token, data = parse.getBody(hm);
	if (!this->urlToken(url,token)){
		resp.turnToBadRequest("Mal parametros de url");
	}else{
		//TODO metodo
		resp.setStatus(200);
		resp.setBody("Modify for " + token + " , " + data);
	}
	return resp;

}

HttpResponse usersHandler::httpDelete(struct http_message *hm){
	HttpResponse resp;
	std::string url = parse.getUri(hm);
	std::string token, data = parse.getBody(hm);

	if (!this->urlToken(url,token)){
		resp.turnToBadRequest("Mal parametros de url");
	}else{
		//TODO metodo
		resp.setStatus(200);
		resp.setBody( "Delete for " + token );
	}
	return resp;

}

bool usersHandler::urlToken(std::string url, std::string &token){
	Parser parse;
	return parse.urlAt(url,1,token);

}

bool usersHandler::urlPhoto(std::string url){
	Parser parse;
	std::string photo;
	parse.urlAt(url,2,photo);

	if ( photo.compare("photo") == 0){
		return true;
	}
	return false;
}

usersHandler::~usersHandler() {
	// TODO Auto-generated destructor stub
}

