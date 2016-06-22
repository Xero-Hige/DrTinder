#include <boost/asio/detail/shared_ptr.hpp>
#include <Swiften/Elements/Message.h>
#include <json.h>
#include <JsonParser.h>
#include "XMPPMessageHandler.h"
#include "api_server_constants.h"


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
    type_ = (like == NULL) ? Chat : Like;
    type_ = (like.asString() == LIKED_TOKEN) ? Like : Dislike;

    (type_ == Chat) ? parseChat(root) : parseLike(root);
}

XMPPMessageType XMPPMessageHandler::getType() {
    return type_;
}

void XMPPMessageHandler::saveMessage(boost::optional<boost::posix_time::ptime> timestamp) {

}

void XMPPMessageHandler::saveLike() {

}

void XMPPMessageHandler::saveDislike() {

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











