#include "LikesDatabaseManager.h"
#include "api_server_constants.h"

LikesDatabaseManager::LikesDatabaseManager(rocksdb::DB *database) : DatabaseManager(database) {

}

bool LikesDatabaseManager::matches(std::string user1, std::string user2){
    std::string aux;
    bool user1_liked = getEntry(user1 + DB_SEPARATOR + user2, aux) && aux.compare(LIKED_TOKEN);
    bool user2_liked = getEntry(user2 + DB_SEPARATOR + user1, aux) && aux.compare(LIKED_TOKEN);
    return user1_liked && user2_liked;
}

