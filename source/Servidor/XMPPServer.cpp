#include "XMPPServer.h"
#include "api_server_constants.h"
#include "XMPPMessageHandler.h"

using namespace Swift;
using namespace boost;

XMPPServer::XMPPServer(NetworkFactories* networkFactories) :
        jid(FIREBASE_SERVER_SEND_URL) {
    component = new Component(jid, "XMPPServer", networkFactories);
    component->onConnected.connect(bind(&XMPPServer::handleConnected, this));
    component->onMessageReceived.connect(
            bind(&XMPPServer::handleMessageReceived, this, _1));
    component->onPresenceReceived.connect(
            bind(&XMPPServer::handlePresenceReceived, this, _1));
    tracer = new ComponentXMLTracer(component);
    component->connect(FIREBASE_SERVER_SEND_URL, FIREBASE_SERVER_PORT);
}

XMPPServer::~XMPPServer() {
    delete tracer;
    delete component;
}

void XMPPServer::handlePresenceReceived(Presence::ref presence) {
    // Automatically approve subscription requests
    if (presence->getType() == Presence::Subscribe) {
        Presence::ref response = Presence::create();
        response->setTo(presence->getFrom());
        response->setType(Presence::Subscribed);
        component->sendPresence(response);
    }
}

void XMPPServer::handleConnected() {
}

void XMPPServer::handleMessageReceived(Message::ref message) {
    boost::optional<boost::posix_time::ptime> timestamp = message->getTimestamp();

    std::string body = message->getBody().get();
    XMPPMessageHandler msgHandler;
    msgHandler.parseMessage(body);
//    message->setTo(message->getFrom());
//    message->setFrom(jid);
//    component->sendMessage(message);
}
