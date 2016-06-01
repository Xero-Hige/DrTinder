/*
 * Hanlder.h
 *
 *  Created on: May 31, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_HANDLER_H_
#define SERVIDOR_HANDLER_H_
#include <string>
#include <map>
#include "../HttpResponse.h"
#include "../DatabaseManager.h"
#include "../SharedServerClient.h"
#include "../Parser.h"
#include "../api_server_constants.h"


class Handler {
public:
	virtual ~Handler();
	Handler();
	void setDatabse(DatabaseManager *db){ this->db = db;};
	void setSharedConnection(SharedServerClient *sharedConnection){
		this->sharedConnection = sharedConnection;
	};
	HttpResponse httpRequest(struct http_message *hm);
	bool manages(std::string uri);
protected:
	virtual HttpResponse httpGet(struct http_message *hm);
	virtual HttpResponse httpPost(struct http_message *hm);
	virtual HttpResponse httpPut(struct http_message *hm);
	virtual HttpResponse httpDelete(struct http_message *hm);
	int getType(std::string method);
	DatabaseManager *db;
	SharedServerClient *sharedConnection;
	std::map<std::string, int> mapMethod;
	HttpResponse methodNotExist(){
			HttpResponse resp;
			resp.turnToBadRequest("No existe el metodo, " + this->uri);
			return resp;
	}
	std::string uri;

};

#endif /* SERVIDOR_HANDLER_H_ */
