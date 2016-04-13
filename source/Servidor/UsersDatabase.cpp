#include "UsersDatabase.h"

#define DB_NAME "users"

using namespace rocksdb;
using std::string;

UsersDatabase::UsersDatabase() {
	Options options;
	options.create_if_missing = true;
	Status status = DB::Open(options, DB_NAME, &db);
	if (! status.ok()) {
		//TODO: error
	}
}

UsersDatabase::~UsersDatabase() {
	delete db;
}

bool UsersDatabase::correctCredentials(string name, string pass) {
	string value;
	Status status = db->Get(ReadOptions(), name, &value);
	if (! status.ok()) {
		return false;
	} else if (value.compare(pass) != 0) {
		return false;
	}
	return true;
}

bool UsersDatabase::addUser(string name, string pass) {
	return db->Put(WriteOptions(), name, pass).ok();
}

bool UsersDatabase::deleteUser(string name) {
	return db->Delete(WriteOptions(), name).ok();
}
