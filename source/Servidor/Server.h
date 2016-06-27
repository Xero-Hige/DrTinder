#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <sstream>
#include "../libs/mongoose/mongoose.h"
#include "MessageHandler.h"
#include "../libs/rocksdb/include/rocksdb/db.h"
#include "RequestHandler.h"
#include "ServerUtils.h"
#include "../libs/loger/easylogging++.h"

#define DEFAULT_PORT_NUMBER 8000

/* Server that listens incomming connections. */
class Server {
    public:
	/* Create new multithreaded server. */
        Server();
    /* Create new multithread server listening in port and sharedLink*/
        Server(std::string port, std::string shared);
	/* Close connections and destroy server. */
        virtual ~Server();
	/* Check for new event. */
        void run();
    /* Set initialized usersDB. */
        void setUsersDB(rocksdb::DB *database);

    void setChatDB(rocksdb::DB *db);

    void setLikesDB(rocksdb::DB *database);

protected:
	/* Proccess new_event from act_connection. */
        static void handleEvent(struct mg_connection* act_connection, int new_event, void* data);
        static std::string linkToShared;
    private:
        struct mg_mgr manager_;
        struct mg_connection* connection_;
        server_databases_t* databases;
};

#endif // SERVER_H
