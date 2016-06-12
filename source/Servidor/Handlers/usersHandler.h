/*
 * usersHandler.h
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_HANDLERS_USERSHANDLER_H_
#define SERVIDOR_HANDLERS_USERSHANDLER_H_

#include "Handler.h"

class usersHandler: public Handler {
public:
	usersHandler();
	HttpResponse httpGet(struct http_message *hm);
	HttpResponse httpPost(struct http_message *hm);
	HttpResponse httpPut(struct http_message *hm);
	HttpResponse httpDelete(struct http_message *hm);
	virtual ~usersHandler();
private:
	bool urlToken(std::string url, std::string &token);
	bool urlPhoto(std::string url);
};

#endif /* SERVIDOR_HANDLERS_USERSHANDLER_H_ */
