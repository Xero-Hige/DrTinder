#include <db.h>
#include "XMPPServer.h"


using namespace Swift;
using namespace boost;

XMPPServer::XMPPServer(NetworkFactories* networkFactories) :
        jid(FIREBASE_JID) {
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
    XMPPMessageHandler msgHandler(body);

    XMPPMessageHandler::XMPPMessageType type = msgHandler.getType();
    switch (type) {
        case XMPPMessageHandler::Chat :
            resendMessage(msgHandler);
            msgHandler.saveMessage(chatDB, timestamp);
            break;
        case XMPPMessageHandler::Like :
            msgHandler.saveLike(likesDB);
            break;
        case XMPPMessageHandler::Dislike :
            msgHandler.saveDislike(likesDB);
    }

}

void XMPPServer::resendMessage(XMPPMessageHandler& msgHandler) {
    Message::ref receiver_message;

    receiver_message->setTo(msgHandler.getReceiver());
    receiver_message->setBody(msgHandler.formResendMessage());

    component->sendMessage(receiver_message);
}

void XMPPServer::setChatDB(rocksdb::DB *db) {
    chatDB = db;
}

void XMPPServer::setLikesDB(rocksdb::DB *db) {
    likesDB = db;
}








