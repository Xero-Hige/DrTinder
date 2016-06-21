#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include "../libs/rocksdb/include/rocksdb/db.h"
#include <iostream>
#include "../libs/loger/easylogging++.h"

#define ALL_USRS "all_users_"
#define USER_DB "user_"
#define USER_ID_DB "user_id_"
#define CHAT_DB "chat_"
#define AVAILABLE_MATCHES_DB "av_match_"
#define PAST_MATCHES_DB "past_match_"
#define INT_PHOTO_DB "int_phot_"

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
	/* Get value of key in found, returns false if not found */
		bool getEntry(std::string key, std::string &found);
	private:
		rocksdb::DB* db;
};

#endif // DATABASE_H
