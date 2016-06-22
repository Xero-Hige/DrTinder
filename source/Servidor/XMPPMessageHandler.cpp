#include <boost/asio/detail/shared_ptr.hpp>
#include <Swiften/Elements/Message.h>
#include <json.h>
#include <JsonParser.h>
#include <db.h>
#include <list>
#include "XMPPMessageHandler.h"
#include "api_server_constants.h"
#include "DatabaseManager.h"


XMPPMessageHandler::XMPPMessageHandler(std::string message) {
    parseMessage(message);
}

XMPPMessageHandler::~XMPPMessageHandler() {

}

XMPPMessageHandler::XMPPMessageType XMPPMessageHandler::parseMessage(std::string& message) {
    Json::Value root;
    Json::Reader reader;
    if (! reader.parse(message, root)) {
        //Todo:error
    }
    Json::Value like = root[LIKE_TOKEN];
    type_ = (like.isNull()) ? Chat : Like;
    type_ = (like.asString() == LIKED_TOKEN) ? Like : Dislike;

    (type_ == Chat) ? parseChat(root) : parseLike(root);
    return type_;
}

XMPPMessageHandler::XMPPMessageType XMPPMessageHandler::getType() {
    return type_;
}

void XMPPMessageHandler::saveMessage(rocksdb::DB *chatDB, boost::optional<boost::posix_time::ptime> timestamp) {

}

void XMPPMessageHandler::saveLike(rocksdb::DB *likesDB) {
    DatabaseManager databaseManager(likesDB);
    databaseManager.addEntry(sender+ DB_SEPARATOR + receiver, LIKED_TOKEN);
}

void XMPPMessageHandler::saveDislike(rocksdb::DB *dislikesDB) {
    DatabaseManager databaseManager(dislikesDB);
    databaseManager.addEntry(sender+ DB_SEPARATOR + receiver, DISLIKED_TOKEN);
}

std::string XMPPMessageHandler::getReceiver() {
    return receiver;
}

std::string XMPPMessageHandler::formResendMessage() {
    Json::Value root;
    root[CHAT_SENT_FROM_TOKEN] = sender;
    root[CHAT_MESSAGE_TOKEN] = dialog;
    return root.asString();
}

void XMPPMessageHandler::parseLike(Json::Value &root) {
    sender = root[LIKE_USER].asString();
    receiver = root[LIKE_CANDIDATE].asString();
    liked = (root[LIKE_TOKEN].asString() == LIKED_TOKEN);
}

void XMPPMessageHandler::parseChat(Json::Value &root) {
    sender = root[CHAT_SENDER_TOKEN].asString();
    receiver = root[CHAT_RECEIVER_TOKEN].asString();
    dialog = root[CHAT_MESSAGE_TOKEN].asString();
}











