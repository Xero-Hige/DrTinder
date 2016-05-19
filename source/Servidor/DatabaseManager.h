#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include "../libs/rocksdb/include/rocksdb/db.h"
#include <iostream>

/* Database for users and passwords. */
class DatabaseManager {
	public:
	/* Create new database. */
		DatabaseManager(rocksdb::DB *database);
	/* Destroy database. */
		~DatabaseManager();
	/* Returns if key, value is a valid entry in database. */
		bool correctEntry(std::string key, std::string value);
	/* Add new entry to database. */
		bool addEntry(std::string key, std::string value);
	/* Delete entry from database. */
		bool deleteEntry(std::string key);
	private:
		rocksdb::DB* db;
};

#endif // DATABASE_H
