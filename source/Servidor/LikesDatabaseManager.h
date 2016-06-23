#ifndef SOURCE_LIKESDATABASEMANAGER_H
#define SOURCE_LIKESDATABASEMANAGER_H


#include "DatabaseManager.h"

class LikesDatabaseManager : public DatabaseManager{
public:
    LikesDatabaseManager(rocksdb::DB *database);

    bool matches(std::string user1, std::string user2);
};


#endif //SOURCE_LIKESDATABASEMANAGER_H
