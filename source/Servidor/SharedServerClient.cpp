#include "SharedServerClient.h"

#define DEFAULT_MILISECS_POLL 3000

using std::string;
using std::array;

SharedServerClient::SharedServerClient() {
	// get a connection object
	conn = new RestClient::Connection(SHARED_SERVER_URL);
	RestClient::HeaderFields headers;
	headers["Content-Type"] = "application/json";
	conn->SetHeaders(headers);
}

SharedServerClient::~SharedServerClient() {
	delete conn;
}

string SharedServerClient::formUrl(string sub_urls[], size_t sub_urls_size) {
    string url = "";
    for (size_t i = 0; i < sub_urls_size; i++) {
        url += "/" + sub_urls[i];
    }
    return url;
}

bool SharedServerClient::valid(RestClient::Response *r ){
	if (r->code >= 400 || r->code < 0){
		return false;
	}
	return true;
}

bool SharedServerClient::setBody(string * body, RestClient::Response *r){
	*body = r->body;
	LOGG(DEBUG) << "SS response " << r->code;
	if (this->valid(r)){
		return true;
	}
	LOGG(DEBUG) << "SS body " + r->body;
	return false;


}

bool SharedServerClient::getUsers(string* users) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL};
    string url = formUrl(sub_urls, 1);
    RestClient::Response r = conn->get(url.c_str());
    return this->setBody(users,&r);

}

bool SharedServerClient::getUser(string user_id, string* user_data) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL, user_id};
    string url = formUrl(sub_urls, 2);
    RestClient::Response r = conn->get(url.c_str());
    return this->setBody(user_data,&r);

}

bool SharedServerClient::postUsers(string &response, string* user_data) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL};
    string url = formUrl(sub_urls, 1);
    RestClient::Response r = conn->post(url.c_str(), user_data->c_str());
    return this->setBody(&response,&r);

}

bool SharedServerClient::changeUser(string user_id, string user_data) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL, user_id};
    string url = formUrl(sub_urls, 2);
    RestClient::Response r = conn->put(url.c_str(), user_data.c_str());
    return this->valid(&r);
}

bool SharedServerClient::changeUserPhoto(string user_id, string photo) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL, user_id, SHARED_SERVER_PHOTO_SUB_URL};
    string url = formUrl(sub_urls, 3);
    JsonParser parser;
    string formated_data = parser.photoToJson(&photo);
    RestClient::Response r = conn->put(url.c_str(), formated_data.c_str());
    return this->valid(&r);
}

bool SharedServerClient::deleteUser(string user_id, string *response) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL, user_id};
    string url = formUrl(sub_urls, 2);
    RestClient::Response r = conn->del(url.c_str());
    return this->setBody( response, &r);

}

bool SharedServerClient::getUsersInterests(string* interests) {
    string sub_urls[] = { SHARED_SERVER_INTERESTS_SUB_URL};
    string url = formUrl(sub_urls, 1);
    RestClient::Response r = conn->get(url.c_str());
    return this->setBody( interests, &r);
}

bool SharedServerClient::postUsersInterests(string* interest) {
    string sub_urls[] = { SHARED_SERVER_INTERESTS_SUB_URL};
    string url = formUrl(sub_urls, 1);
    RestClient::Response r = conn->post(url.c_str(), interest->c_str());
    return this->valid(&r);
}

bool SharedServerClient::getUserPhoto(string user_id, string &photo_64) {
	string data;
	if (this->getUser(user_id,&data)){
		JsonParser parser;
		parser.parsing(data);
		photo_64 = parser.getValue(USER_KEY)[PHOTO_KEY].asString();
		return true;
	}
	return false;
}

bool SharedServerClient::getInterestPhoto(string interest_id, string &photo_64) {
	string sub_urls[] = { SHARED_SERVER_INTERESTS_SUB_URL,interest_id,SHARED_SERVER_PHOTO_SUB_URL};
	string url = formUrl(sub_urls, 3);
	RestClient::Response r = conn->get(url.c_str());
	return setBody(&photo_64,&r);
}
