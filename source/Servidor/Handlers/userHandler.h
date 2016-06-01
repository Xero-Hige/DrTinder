/*
 * userHandler.h
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_HANDLERS_USERHANDLER_H_
#define SERVIDOR_HANDLERS_USERHANDLER_H_

#include "Handler.h"

class userHandler: public Handler {
public:
	userHandler();
	HttpResponse httpGet(struct http_message *hm);
	HttpResponse httpPost(struct http_message *hm);
	virtual ~userHandler();
};

#endif /* SERVIDOR_HANDLERS_USERHANDLER_H_ */
