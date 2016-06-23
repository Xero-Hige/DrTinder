#include "ChatDatabaseManager.h"
#include "api_server_constants.h"

using std::string;

ChatDatabaseManager::ChatDatabaseManager(rocksdb::DB *database) : DatabaseManager(database) {

}



void ChatDatabaseManager::saveMessage(std::string& message, std::string& sender, std::string& receiver){
    string aux1, aux2;
    string key1 = sender + DB_SEPARATOR + receiver;
    string key2 = receiver + DB_SEPARATOR + sender;

    if (! getEntry(key1, aux1) && ! getEntry(key2, aux2)) {
        DatabaseManager::addEntry(key1, message + "\n");
        return;
    }
    if (aux1.empty()) {
        replaceEntry(key2, aux2 + message + "\n");
    } else {
        replaceEntry(key1, aux1 + message + "\n");
    }
}

bool ChatDatabaseManager::getHistory(std::string& sender, std::string& receiver, std::string& chat_history){
    string aux;
    if (! getEntry(sender + DB_SEPARATOR + receiver, aux) ) {
        if (! getEntry(receiver + DB_SEPARATOR + sender, aux)) {
            return false;
        }
    }
    chat_history = aux;
    return true;
}

void ChatDatabaseManager::addEntry(string sender, string receiver, string message) {
    string aux;
    string key1 = sender + DB_SEPARATOR + receiver;
    string key2 = receiver + DB_SEPARATOR + sender;

    if (! getEntry(key1, aux) ) {
        if (! getEntry(key2, aux)) {
            DatabaseManager::addEntry(key1, message + "\n");
            return;
        }
        DatabaseManager::addEntry(key2, aux + message + "\n");
        return;
    }
    DatabaseManager::addEntry(key1, aux + message + "\n");
}
