#ifndef SOURCE_CHATDATABASEMANAGER_H
#define SOURCE_CHATDATABASEMANAGER_H


#include "DatabaseManager.h"

class ChatDatabaseManager : public DatabaseManager {
    public:
        ChatDatabaseManager(rocksdb::DB *database);

        void saveMessage(std::string& message, std::string& sender, std::string& receiver);
        bool getHistory(std::string& sender, std::string& receiver, std::string& chat_history);

    void addEntry(std::string sender, std::string receiver, std::string message);

};


#endif //SOURCE_CHATDATABASEMANAGER_H
