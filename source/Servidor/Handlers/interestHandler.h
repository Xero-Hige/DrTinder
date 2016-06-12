/*
 * interestHandler.h
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_HANDLERS_INTERESTHANDLER_H_
#define SERVIDOR_HANDLERS_INTERESTHANDLER_H_

#include "Handler.h"

class interestHandler: public Handler {
public:
	interestHandler();
	HttpResponse httpGet(struct http_message * hm);
	virtual ~interestHandler();
};

#endif /* SERVIDOR_HANDLERS_INTERESTHANDLER_H_ */
