#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include "../libs/rocksdb/include/rocksdb/db.h"
#include <iostream>

class UsersDatabase {
	public:
		UsersDatabase();
		~UsersDatabase();
		bool correctCredentials(std::string name, std::string pass);
		bool addUser(std::string name, std::string pass);
		bool deleteUser(std::string name);
		private:
		rocksdb::DB* db;
};

#endif // DATABASE_H
