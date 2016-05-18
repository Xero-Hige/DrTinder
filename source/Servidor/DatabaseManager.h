#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include "../libs/rocksdb/include/rocksdb/db.h"
#include <iostream>

class DatabaseManager {
	public:
		DatabaseManager(rocksdb::DB *database);
		~DatabaseManager();
		bool correctEntry(std::string key, std::string value);
		bool addEntry(std::string key, std::string value);
		bool deleteEntry(std::string key);
		private:
		rocksdb::DB* db;
};

#endif // DATABASE_H
