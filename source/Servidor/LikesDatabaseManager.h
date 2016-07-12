#ifndef SOURCE_LIKESDATABASEMANAGER_H
#define SOURCE_LIKESDATABASEMANAGER_H

#define MATCHES_OF "MatchesOf"
#define NEW_MATCHES_OF "NewMatchesOf"
#define USERS_INTERACTED_OF "UserInteractedOf"

#include "DatabaseManager.h"

using std::string;

class LikesDatabaseManager : public DatabaseManager{
public:
    LikesDatabaseManager(rocksdb::DB *database);

    bool matches(std::string user1, std::string user2);

    void saveLike(std::string deUser, std::string aUser, std::string boolean);

    void getMatches(std::string deUser, std::string &usenamesMatches);

    void getNewMatches(std::string deUser, std::string &nameMatches);

    void getInteractedUsers(std::string deUser, std::string &usenamesInteracted);

private:
	void saveInteraction(string fromUser, string toUser);
	void saveMatch(string fromUser, string toUser);
	void saveNewMatch(string fromUser, string toUser);
	void deleteCurrentNewMatches(string user);
};


#endif //SOURCE_LIKESDATABASEMANAGER_H
