#ifndef SERVER_H
#define SERVER_H

#include "libs/mongoose/mongoose.h"

#define PORT_NUMBER 8080

class Server {
    public:
        Server();
        virtual ~Server();
    protected:
    private:
        struct mg_server* server;
};

#endif // SERVER_H
