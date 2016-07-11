#ifndef SOURCE_CHATDATABASEMANAGER_H
#define SOURCE_CHATDATABASEMANAGER_H

#define NEW_MSGS_PREFIX "NewMsgsFor_"

#include "DatabaseManager.h"

class ChatDatabaseManager: public DatabaseManager {
public:
	ChatDatabaseManager(rocksdb::DB *database);

	void saveMessage(std::string message, std::string sender,
			std::string receiver);

	/*En chat history quedan de a uno por linea
	 *    usernameSender,msg
	 * ordenados de mas viejo a mas nuevo.*/

	bool getHistory(std::string sender,
			std::string receiver, std::string& chat_history);

	void getNewMsgs(std::string sender,std::string receiver, std::string &newMsgs);

private:

	std::string getConversationKey(std::string sender, std::string receiver);

	std::string getNewMessagesKey(std::string sender, std::string receiver);

	void saveNewMsgs(std::string messageWithSender, std::string sender,
			std::string receiver);

};


#endif //SOURCE_CHATDATABASEMANAGER_H
