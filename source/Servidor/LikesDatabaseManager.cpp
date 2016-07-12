#include "LikesDatabaseManager.h"
#include "api_server_constants.h"

/* Para guardar:
 *  likes entre personas
 *  nombre nuevos matches
 *  todos los matches de un usuario
 *  todas las interacciones de un usario
 * */
LikesDatabaseManager::LikesDatabaseManager(rocksdb::DB *database) : DatabaseManager(database) {

}

bool LikesDatabaseManager::matches(std::string user1, std::string user2){
    std::string reaction, debug_msg;

    bool existed1 = getEntry(user1 + DB_SEPARATOR + user2, reaction);
    bool liked1 = (existed1 && (reaction.compare(LIKED_TOKEN) == 0));

    debug_msg = (existed1) ? " has " + reaction: " did not interact with "  + user2;
    LOGG(DEBUG) << user1 << debug_msg;

    bool existed2 = getEntry(user2 + DB_SEPARATOR + user1, reaction);
    bool liked2 = ( existed2 && (reaction.compare(LIKED_TOKEN) == 0));

    debug_msg = (existed2) ? " has " + reaction: " did not interacted with " + user1;
    LOGG(DEBUG) << user2 << debug_msg;

    return liked1 && liked2;
}

void LikesDatabaseManager::saveInteraction(string fromUser, string toUser){
	string interacted_users;
	string key = USERS_INTERACTED_OF + fromUser;
	if (getEntry(key, interacted_users)) {
		addEntry(key, interacted_users + toUser + "\n");
	} else {
		addEntry(key, toUser + "\n");
	}
}

void LikesDatabaseManager::saveMatch(string fromUser, string toUser){

	string matched_users;
	string key = MATCHES_OF + fromUser;

	if(getEntry(key, matched_users)){
		addEntry(key, matched_users  + toUser + "\n");
	}else{
		addEntry(key, toUser + "\n");
	}

}


void LikesDatabaseManager::saveNewMatch(string fromUser, string toUser){

	string new_matched_users;
	string key = NEW_MATCHES_OF + fromUser;

	if(getEntry(key, new_matched_users)){
		addEntry(key, new_matched_users  + toUser + "\n");
	}else{
		addEntry(key, toUser + "\n");
	}

}


void LikesDatabaseManager::saveLike(std::string deUser, std::string aUser, std::string boolean) {
	std::string matched_users, current_new_matches;

	addEntry(deUser + DB_SEPARATOR + aUser, boolean);

	saveInteraction(deUser, aUser);

	if( matches(deUser,aUser)){

		saveMatch(deUser, aUser);
		saveNewMatch(deUser, aUser);

		saveMatch(aUser, deUser);
		saveNewMatch(aUser, deUser);

	}
}

void LikesDatabaseManager::deleteCurrentNewMatches(string user){
	string key = NEW_MATCHES_OF + user;
	string new_matches;
	if (getEntry(key, new_matches)){
		deleteEntry(key);
	}
}

void LikesDatabaseManager::getMatches(std::string deUser, std::string &usernamesMatched){
	getEntry(MATCHES_OF + deUser, usernamesMatched);
	deleteCurrentNewMatches(deUser);
}

void LikesDatabaseManager::getNewMatches(std::string deUser, std::string &usernamesMatched){
	//lo borra cuando lo pide y cuando pide todos los matches
	getEntry(NEW_MATCHES_OF + deUser, usernamesMatched);
	deleteCurrentNewMatches(deUser);
}

void LikesDatabaseManager::getInteractedUsers(std::string deUser, std::string &usernamesInteracted){
	getEntry(USERS_INTERACTED_OF + deUser, usernamesInteracted);
}
