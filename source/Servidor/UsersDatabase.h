#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include "../libs/rocksdb/include/rocksdb/db.h"
#include <iostream>

/* Database for users and passwords. */
class UsersDatabase {
	public:
		/* Create new database. */
		UsersDatabase();
		/* Destroy database. */
		~UsersDatabase();
		/* Returns if name is a valid user in database with password pass. */
		bool correctCredentials(std::string name, std::string pass);
		/* Add new user to database with id name and password pass. */
		bool addUser(std::string name, std::string pass);
		/* Delete user from database. */
		bool deleteUser(std::string name);
	private:
		rocksdb::DB* db;
};

#endif // DATABASE_H
