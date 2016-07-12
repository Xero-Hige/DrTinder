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

std::string ChatDatabaseManager::getNewMessagesKey(std::string sender, std::string receiver){
	return NEW_MSGS_PREFIX + sender + DB_SEPARATOR + receiver;
}

void ChatDatabaseManager::saveNewMsgs(std::string messageWithSender, std::string sender, std::string receiver){
	string currentNewMessages;

	string newMessageskey = getNewMessagesKey(sender, receiver);
	if (!getEntry(newMessageskey, currentNewMessages)) {
		DatabaseManager::addEntry(newMessageskey, messageWithSender + "\n");
		return;
	}
	replaceEntry(newMessageskey, currentNewMessages + messageWithSender + "\n");

}

void ChatDatabaseManager::saveMessage(std::string message, std::string sender, std::string receiver){
	LOGG(DEBUG) << "Saving message " << message <<" from " << sender << " to " << receiver;
    string conversationHistory;
    string key;
    key = getConversationKey(sender, receiver);

    string messageWithSender = sender + DB_SEPARATOR + message;

    //Saving new message in both users conversations.
    saveNewMsgs(messageWithSender,sender,receiver);
    saveNewMsgs(messageWithSender,receiver,sender);

    if (! getEntry(key, conversationHistory)) {
        DatabaseManager::addEntry(key, messageWithSender + "\n");
        return;
    }

    replaceEntry(key, conversationHistory + messageWithSender + "\n");

}


void ChatDatabaseManager::deleteCurrentNewMessages(std::string sender, std::string receiver){
	string newMessages;
	string key = getNewMessagesKey(sender, receiver);
	if (getEntry(key, newMessages)){
		deleteEntry(key);
	}
}

bool ChatDatabaseManager::getHistory(std::string sender, std::string receiver, std::string& chat_history){
    string key;
    bool conversationFound;

    key = getConversationKey(sender, receiver);
    conversationFound = getEntry(key, chat_history);

    deleteCurrentNewMessages(sender, receiver);
    return conversationFound;

}

void ChatDatabaseManager::getNewMsgs(string sender, string receiver, string &newMsgs){
	string newMessageskey = getNewMessagesKey(sender, receiver);
	bool foundNewMessages;

	foundNewMessages = getEntry(newMessageskey,newMsgs);

	if (foundNewMessages){
		LOGG(DEBUG) << "Found new messages from " << sender << " and " << receiver;
		deleteCurrentNewMessages(sender, receiver);
	}
}
