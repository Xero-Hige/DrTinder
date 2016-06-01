/*
 * HttpMsg.h
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_HTTPRESPONSE_H_
#define SERVIDOR_HTTPRESPONSE_H_
#include <string>
#include "../libs/mongoose/mongoose.h"

class HttpResponse {
public:
	HttpResponse();
	HttpResponse(int status, std::string body){
		this->status = status;
		this->body = body;
	};
	virtual ~HttpResponse();
	int getStatus(){ return this->status; };
	std::string getBody() { return this->body;};
	void setStatus(int status){ this->status = status; };
	void setBody(std::string body) { this->body = body; };
	void turnToBadRequest(std::string error){
		//no se si lo voy a usar
		this->status = 400;
		this->body = error;
	};
	void sentTo(struct mg_connection* act_connection);
private:
	int status;
	std::string body;
};

#endif /* SERVIDOR_HTTPRESPONSE_H_ */
