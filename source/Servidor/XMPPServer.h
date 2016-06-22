#ifndef SOURCE_XMPPSERVER_H
#define SOURCE_XMPPSERVER_H

#include <iostream>
#include <boost/bind.hpp>

#include <Swiften/Swiften.h>
#include <Swiften/Network/NetworkFactories.h>
#include <Swiften/Elements/Presence.h>
#include <Swiften/Elements/Message.h>
#include <Swiften/Component/Component.h>
#include <Swiften/Component/ComponentXMLTracer.h>
#include "api_server_constants.h"
#include "XMPPMessageHandler.h"

class XMPPServer {
public:
    XMPPServer(Swift::NetworkFactories* networkFactories);
    ~XMPPServer();

private:
    void handlePresenceReceived(Swift::Presence::ref presence);

    void handleConnected();

    void handleMessageReceived(Swift::Message::ref message);

    Swift::JID jid;
    Swift::Component* component;
    Swift::ComponentXMLTracer* tracer;

    void resendMessage(XMPPMessageHandler& msgHandler);
};


#endif //SOURCE_XMPPSERVER_H
