#ifndef SOURCE_XMPPMESSAGEHANDLER_H
#define SOURCE_XMPPMESSAGEHANDLER_H


#include <string>
#include <Swiften/Elements/Message.h>
#include <db.h>
#include "../../libs/jsoncpp/dist/json/json.h"
#include <boost/asio/detail/shared_ptr.hpp>
#include <Swiften/Elements/Message.h>
#include <json.h>
#include <JsonParser.h>
#include <db.h>
#include <list>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "api_server_constants.h"
#include "DatabaseManager.h"
#include "ChatDatabaseManager.h"
#include "api_server_constants.h"
#include "DatabaseManager.h"
#include <boost/date_time/posix_time/posix_time.hpp>

class XMPPMessageHandler {

public:
    enum XMPPMessageType {Chat, Like, Dislike};

    XMPPMessageHandler(std::string message);

    ~XMPPMessageHandler();

    XMPPMessageType getType();

    void saveMessage(rocksdb::DB *chatDB, boost::optional<boost::posix_time::ptime> timestamp);

    void saveLike(rocksdb::DB *likesDB);

    void saveDislike(rocksdb::DB *dislikesDB);

    std::string getReceiver();

    std::string formResendMessage();

private:
    XMPPMessageHandler::XMPPMessageType parseMessage(std::string& message);

    XMPPMessageType type_;

    std::string sender;

    std::string receiver;

    std::string dialog;

    bool liked;

    void parseLike(Json::Value &root);

    void parseChat(Json::Value &root);
};


#endif //SOURCE_XMPPMESSAGEHANDLER_H
