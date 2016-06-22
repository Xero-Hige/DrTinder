#ifndef SOURCE_XMPPMESSAGEHANDLER_H
#define SOURCE_XMPPMESSAGEHANDLER_H


#include <string>

class XMPPMessageHandler {

    ~XMPPMessageHandler();

public:
    void parseMessage(std::string message);

    XMPPMessageHandler();
};


#endif //SOURCE_XMPPMESSAGEHANDLER_H
