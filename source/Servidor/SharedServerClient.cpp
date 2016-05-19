#include "SharedServerClient.h"

#define DEFAULT_MILISECS_POLL 3000

using std::string;
using std::array;

SharedServerClient::SharedServerClient() {
    mg_mgr_init(&manager_, NULL);
    connection_ = mg_connect_http(&manager_, handleEvent, SHARED_SERVER_URL, NULL, NULL);
    mg_set_protocol_http_websocket(connection_);
}

SharedServerClient::~SharedServerClient() {
    mg_mgr_free(&manager_);
}

string SharedServerClient::formUrl(string sub_urls[], size_t sub_urls_size) {
    string url(SHARED_SERVER_URL);
    for (size_t i = 0; i < sub_urls_size; i++) {
        url += "/" + sub_urls[i];
    }
    return url;
}

string* SharedServerClient::connect(string url, string* post_data) {
    connection_ = mg_connect_http(&manager_, handleEvent, url.c_str(), NULL, post_data->c_str());
    mg_set_protocol_http_websocket(connection_);

    connection_->user_data = NULL;
    while (! connection_->user_data){
        run();
    }

    return (string*) connection_->user_data;
}

void SharedServerClient::getUsers(string* users) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL};
    string url = formUrl(sub_urls, 1);
    *users = *connect(url, NULL);
}

void SharedServerClient::getUser(string user_id, string* user_data) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL, user_id};
    string url = formUrl(sub_urls, 2);
    *user_data = *connect(url, NULL);
}

void SharedServerClient::postUsers(string user_id, string* user_data) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL};
    string url = formUrl(sub_urls, 1);
    connect(url, user_data);
}

void SharedServerClient::changeUser(string user_id, string user_data) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL, user_id};
    string url = formUrl(sub_urls, 2);
    //TODO: connect con put
//    connect(url, user_data);
}

void SharedServerClient::changeUserPhoto(string user_id, string photo) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL, user_id, SHARED_SERVER_PHOTO_SUB_URL};
    string url = formUrl(sub_urls, 3);
    //TODO: connect con put
//    connect(url, photo);
}

void SharedServerClient::deleteUser(string user_id) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL, user_id};
    string url = formUrl(sub_urls, 2);
    //TODO: connect con delete
//    connect(url, NULL);
}

void SharedServerClient::getUsersInterests(string* interests) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL, SHARED_SERVER_INTERESTS_SUB_URL};
    string url = formUrl(sub_urls, 2);
    *interests = *connect(url, NULL);
}

void SharedServerClient::postUsersInterests(string* interest) {
    string sub_urls[] = {SHARED_SERVER_USERS_SUB_URL, SHARED_SERVER_INTERESTS_SUB_URL};
    string url = formUrl(sub_urls, 2);
    connect(url, interest);
}


void SharedServerClient::handleEvent(struct mg_connection *act_connection, int new_event, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;

    switch (new_event) {
        case MG_EV_CONNECT:
            int connect_status = *(int *) ev_data;
            if (connect_status != 0) {
                //TODO: error no se pudo conectar al ss
            }
            break;
        case MG_EV_HTTP_REPLY:
            act_connection->user_data = new string(hm->body.p, hm->body.len);
            break;
        default:
            break;
    }
}

void SharedServerClient::run() {
    mg_mgr_poll(&manager_, DEFAULT_MILISECS_POLL);
}
