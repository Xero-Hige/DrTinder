#include "DatabaseManager.h"

#define DB_NAME "users"

using namespace rocksdb;
using std::string;

DatabaseManager::DatabaseManager(DB *database) : db(database){

}

DatabaseManager::~DatabaseManager() {
	delete db;
}

bool DatabaseManager::correctEntry(string key, string value) {
	string aux;
	Status status = db->Get(ReadOptions(), key, &aux);
	if (! status.ok()) {
		return false;
	} else if (value.compare(aux) != 0) {
		return false;
	}
	return true;
}

bool DatabaseManager::addEntry(std::string key, std::string value) {
	return db->Put(WriteOptions(), key, value).ok();
}

bool DatabaseManager::deleteEntry(string key) {
	return db->Delete(WriteOptions(), key).ok();
}

bool DatabaseManager::getEntry(string key, string &found){
	string aux;
	Status status = db->Get(ReadOptions(), key, &aux);
	if (! status.ok()) {
		return false;
	}
	found = aux;
	return true;
}


void DatabaseManager::replaceEntry(std::string key, std::string value) {
	string aux;
	if (getEntry(key, aux)) {
		deleteEntry(key);
	}
	addEntry(key, value);
}




