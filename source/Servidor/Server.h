#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <sstream>
#include "../libs/mongoose/mongoose.h"
#include "MessageHandler.h"
#include "../libs/rocksdb/include/rocksdb/db.h"

#define DEFAULT_PORT_NUMBER 8000

/* Server that listens incomming connections. */
class Server {
    public:
	/* Create new multithreaded server. */
        Server();
	/* Close connections and destroy server. */
        virtual ~Server();
	/* Check for new event. */
        void run();
    /* Set initialized usersDB. */
        void setUsersDB(rocksdb::DB *database);

    protected:
	/* Proccess new_event from act_connection. */
        static void handleEvent(struct mg_connection* act_connection, int new_event, void* data);
        static bool validateToken(MessageHandler* msgHandler, struct http_message* http_msg);
        static MessageHandler *acceptConnection(http_message *http_msg, mg_connection *connection);
        static void listenUsersRequest(http_message *http_msg, struct mg_connection* connection);
    private:
        struct mg_mgr manager_;
        struct mg_connection* connection_;
        rocksdb::DB *usersDB;


    static void listenIdRequest(http_message *http_msg, struct mg_connection* connection);


    static void listenFriendsRequest(http_message *http_msg, struct mg_connection* connection);

    static void listenInterestRequest(http_message *http_msg, struct mg_connection* connection);

    static void listenChatRequest(http_message *http_msg, struct mg_connection* connection);

    static void listenPhotoRequest(http_message *http_msg, struct mg_connection* connection);

    static void sendHttpLine(mg_connection *connection, int status_code);

    static void sendHttpReply(mg_connection *connection, std::string reply, std::string content_type);
};

#endif // SERVER_H
