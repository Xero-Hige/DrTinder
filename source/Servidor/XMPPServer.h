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

    void sendACK(Swift::Message::ref message);
};


#endif //SOURCE_XMPPSERVER_H
