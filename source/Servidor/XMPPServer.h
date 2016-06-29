#ifndef SOURCE_XMPPSERVER_H
#define SOURCE_XMPPSERVER_H

#include <iostream>
#include <boost/bind.hpp>
#include "../../libs/loger/easylogging++.h"
#include <Swiften/Swiften.h>
#include <Swiften/Network/NetworkFactories.h>
#include <Swiften/Elements/Presence.h>
#include <Swiften/Elements/Message.h>
#include <Swiften/Component/Component.h>
#include <Swiften/Component/ComponentXMLTracer.h>
#include "api_server_constants.h"
#include "XMPPMessageHandler.h"
#include "../libs/rocksdb/include/rocksdb/db.h"

class XMPPServer {
public:
    XMPPServer(Swift::NetworkFactories* networkFactories);
    ~XMPPServer();

    void setChatDB(rocksdb::DB *db);

    void setLikesDB(rocksdb::DB *db);

    void setDislikesDB(rocksdb::DB *db);

private:
    void handlePresenceReceived(Swift::Presence::ref presence);

    void handleConnected();

    void handleMessageReceived(Swift::Message::ref message);

    Swift::JID jid;
    Swift::Component* component;
    Swift::ComponentXMLTracer* tracer;

    void resendMessage(XMPPMessageHandler& msgHandler);

    rocksdb::DB *likesDB;
    rocksdb::DB *chatDB;
};


#endif //SOURCE_XMPPSERVER_H
