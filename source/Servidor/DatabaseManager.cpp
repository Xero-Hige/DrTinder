#include "DatabaseManager.h"

#define DB_NAME "users"

using namespace rocksdb;
using std::string;

DatabaseManager::DatabaseManager(DB *database) : db(database){
	Options options;
	options.create_if_missing = true;
	Status status = DB::Open(options, DB_NAME, &db);
	if (! status.ok()) {
		//TODO: error
	}
}

DatabaseManager::~DatabaseManager() {
	delete db;
}

bool DatabaseManager::correctEntry(string key, string value) {
	string aux;
	Status status = db->Get(ReadOptions(), key, &aux);
	if (! status.ok()) {
		std::cout << "status";
		return false;
	} else if (value.compare(aux) != 0) {
		std::cout << "value";
		return false;
	}
	return true;
}

bool DatabaseManager::addEntry(string key, string value) {
	return db->Put(WriteOptions(), key, value).ok();
}

bool DatabaseManager::deleteEntry(string key) {
	return db->Delete(WriteOptions(), key).ok();
}
