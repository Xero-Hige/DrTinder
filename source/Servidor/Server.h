#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <sstream>
#include "../libs/mongoose/mongoose.h"
#include "MessageHandler.h"

#define DEFAULT_PORT_NUMBER 8080

class Server {
    public:
        Server();
        virtual ~Server();
        void run();
		void setMessageHandler(MessageHandler* msgHandler);
    protected:
        static void handleEvent(struct mg_connection* act_connection, int new_event, void* data);
    private:
        struct mg_mgr manager_;
        struct mg_connection* connection_;
		MessageHandler* msgHandler;
};

#endif // SERVER_H
