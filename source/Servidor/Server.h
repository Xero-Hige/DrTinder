#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <sstream>
#include "../libs/mongoose/mongoose.h"
#include "MessageHandler.h"
#include "../libs/rocksdb/include/rocksdb/db.h"

#define DEFAULT_PORT_NUMBER 8080

class Server {
    public:
        Server();
        virtual ~Server();
        void run();
        void setUsersDB(rocksdb::DB *database);

    protected:
        static void handleEvent(struct mg_connection* act_connection, int new_event, void* data);
    private:
        struct mg_mgr manager_;
        struct mg_connection* connection_;
        rocksdb::DB *usersDB;
};

#endif // SERVER_H
