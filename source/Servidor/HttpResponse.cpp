/*
 * HttpMsg.cpp
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#include "HttpResponse.h"

HttpResponse::HttpResponse() {
	// TODO Auto-generated constructor stub
	this->body = "";
	this->status = 400;
}

HttpResponse::~HttpResponse() {
	// TODO Auto-generated destructor stub
}

void HttpResponse::sentTo(mg_connection * connection){
	mg_printf(connection, "HTTP/1.1 %d\r\n"
			"Transfer-Encoding: chunked\r\n"
			"\r\n", this->status);
	mg_printf_http_chunk(connection, "%s", this->body.c_str());
	mg_send_http_chunk(connection,"",0);
}
