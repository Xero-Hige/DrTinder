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

#define GET_S "GET"
#define POST_S "POST"
#define DELETE_S "DELETE"
#define PUT_S "PUT"
#define GET 1
#define POST 2
#define DELETE 3
#define PUT 4
#define NOT_CREATED 401
#define STATUS_OK 200
#define CREATED 201

class Handler {
public:
	virtual ~Handler();
	Handler();
	void setDatabse(DatabaseManager *db){ this->db = db;};
	void setSharedConnection(SharedServerClient *sharedConnection){
		this->sharedConnection = sharedConnection;
	};
	HttpResponse httpRequest(struct http_message *hm);
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
			resp.turnToBadRequest("No existe el metodo");
			return resp;
	}

};

#endif /* SERVIDOR_HANDLER_H_ */
