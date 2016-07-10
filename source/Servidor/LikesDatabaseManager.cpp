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
    std::string aux, debug_msg;
    bool existed1 = getEntry(user1 + DB_SEPARATOR + user2, aux);
    bool liked1 = (existed1 && (aux.compare(LIKED_TOKEN) == 0));
    debug_msg = (existed1) ? " has " + aux: " did not interact with "  + user2;
    LOGG(DEBUG) << user1 << debug_msg;

    bool existed2 = getEntry(user2 + DB_SEPARATOR + user1, aux);
    bool liked2 = ( existed2 && (aux.compare(LIKED_TOKEN) == 0));
    debug_msg = (existed2) ? " has " + aux: " did not interacted with " + user1;
    LOGG(DEBUG) << user2 << debug_msg;

    return liked1 && liked2;
}

void LikesDatabaseManager::saveLike(std::string deUser, std::string aUser, std::string boolean) {
	std::string interacted_aux, matches_aux, new_aux;
	this->addEntry(deUser + DB_SEPARATOR + aUser, boolean);

	if (getEntry(USERS_INTERACTED_OF + deUser, interacted_aux)){
		addEntry(USERS_INTERACTED_OF + deUser,interacted_aux + aUser + "\n");
	}else{
		addEntry(USERS_INTERACTED_OF + deUser,aUser + "\n");
	}

	if( matches(deUser,aUser)){
		getEntry(MATCHES_OF + deUser, matches_aux);
		addEntry(MATCHES_OF + deUser, matches_aux  + aUser + "\n");

		getEntry(NEW_MATCHES_OF + deUser, new_aux);
		addEntry(NEW_MATCHES_OF + deUser, new_aux + aUser + "\n");
	}
}

void LikesDatabaseManager::getMatches(std::string deUser, std::string &usernamesMatched){
	getEntry(MATCHES_OF + deUser, usernamesMatched);
	this->replaceEntry(NEW_MATCHES_OF + deUser, "");
}

void LikesDatabaseManager::getNewMatches(std::string deUser, std::string &usernamesMatched){
	//lo borra cuando lo pide y cuando pide todos los matches
	getEntry(NEW_MATCHES_OF + deUser, usernamesMatched);
	this->replaceEntry(NEW_MATCHES_OF + deUser, "");
}

void LikesDatabaseManager::getInteractedUsers(std::string deUser, std::string &usernamesInteracted){
	getEntry(USERS_INTERACTED_OF + deUser, usernamesInteracted);
}
