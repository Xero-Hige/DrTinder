#include "ChatDatabaseManager.h"
#include "api_server_constants.h"

using std::string;

ChatDatabaseManager::ChatDatabaseManager(rocksdb::DB *database) : DatabaseManager(database) {

}

std::string ChatDatabaseManager::getConversationKey(std::string sender, std::string receiver){
	string key;
    if (sender.compare(receiver) < 0) {
		key = receiver + DB_SEPARATOR + sender;
	} else {
		key = sender + DB_SEPARATOR + receiver;
	}
    return key;
}

void ChatDatabaseManager::saveNewMsgs(std::string& message, std::string& sender, std::string& receiver){
	string currentNewMessages;
	string conversationKey = getConversationKey(sender, receiver);
	string newMessageskey = NEW_MSGS_PREFIX + conversationKey;

	string messageWithSender = sender + DB_SEPARATOR + message;

	if (!getEntry(newMessageskey, currentNewMessages)) {
		DatabaseManager::addEntry(newMessageskey, messageWithSender + "\n");
		return;
	}
	replaceEntry(newMessageskey, currentNewMessages + messageWithSender + "\n");
}

void ChatDatabaseManager::saveMessage(std::string& message, std::string& sender, std::string& receiver){
	LOGG(INFO) << "Saving message " << message <<" from " << sender << " to " << receiver;
    string conversationHistory;
    string key;
    key = getConversationKey(sender, receiver);

    this->saveNewMsgs(message,sender,receiver);

    string messageWithSender = sender + DB_SEPARATOR + message;

    if (! getEntry(key, conversationHistory)) {
        DatabaseManager::addEntry(key, messageWithSender + "\n");
        return;
    }

    replaceEntry(key, conversationHistory + messageWithSender + "\n");

}

bool ChatDatabaseManager::getHistory(std::string& sender, std::string& receiver, std::string& chat_history){
    string aux;
    string key;

    key = getConversationKey(sender, receiver);

    if (! getEntry(key, aux) ) {
            return false;
    }

    chat_history = aux;
    return true;
}

void ChatDatabaseManager::getNewMsgs(string sender, string receiver, string &newMsgs){
	string newMessages;
	string conversationKey = getConversationKey(sender, receiver);
	string newMessageskey = NEW_MSGS_PREFIX + conversationKey;

	if (getEntry(newMessageskey,newMessages)){

		this->replaceEntry(newMessageskey,"");
	} else {
		newMessages = "";
	}

	newMsgs = newMessages;
}
