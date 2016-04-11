#ifndef CLIENT_H
#define CLIENT_H

#include "libs/mongoose/mongoose.h"
#include <string>

class Client {
    public:
        Client();
        virtual ~Client();
		void run();
		void send(std::string message);
    protected:
		static void handleEvent(struct mg_connection* act_connection, int new_event, void* data);
    private:
		struct mg_mgr manager_;
		struct mg_connection* connection_;
};

#endif // CLIENT_H
