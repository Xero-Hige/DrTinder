#include "ChatDatabaseManager.h"
#include "api_server_constants.h"

using std::string;

ChatDatabaseManager::ChatDatabaseManager(rocksdb::DB *database) : DatabaseManager(database) {

}

void ChatDatabaseManager::saveNewMsgs(std::string& message, std::string& sender, std::string& receiver){
	//Solo guardo los nuevos que le enviaron
	string aux;
	string key = NEW_MSGS_PREFIX + receiver + DB_SEPARATOR + sender;
	if (!getEntry(key, aux)) {
		DatabaseManager::addEntry(key, message + "\n");
		return;
	}
	replaceEntry(key, aux + message + "\n");
}

void ChatDatabaseManager::saveMessage(std::string& message, std::string& sender, std::string& receiver){
    string aux;
    string key;
    if (sender.compare(receiver) < 0 ){
    	key = receiver + DB_SEPARATOR + sender;
    }else{
    	key = sender + DB_SEPARATOR + receiver;
    }

    this->saveNewMsgs(message,sender,receiver);

    if (! getEntry(key, aux)) {
        DatabaseManager::addEntry(key, message + "\n");
        return;
    }

    replaceEntry(key, aux + message + "\n");

}

bool ChatDatabaseManager::getHistory(std::string& sender, std::string& receiver, std::string& chat_history){
    string aux;
    string key;

    if (sender.compare(receiver) < 0) {
		key = receiver + DB_SEPARATOR + sender;
	} else {
		key = sender + DB_SEPARATOR + receiver;
	}

    if (! getEntry(key, aux) ) {
            return false;
    }

    chat_history = aux;
    return true;
}

void ChatDatabaseManager::getNewMsgs(string sender, string reciever, string &newMsgs){
	string aux;
	string key = sender + DB_SEPARATOR + reciever;

	if (getEntry(NEW_MSGS_PREFIX + key,aux)){
		newMsgs = aux;
		this->replaceEntry(NEW_MSGS_PREFIX + key,"");
	}

}
