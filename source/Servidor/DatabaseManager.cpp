#include <rocksdb/utilities/transaction.h>
#include "DatabaseManager.h"

#define DB_NAME "users"
#define DB_CHAT "chats"
using namespace rocksdb;
using std::string;

DatabaseManager::DatabaseManager(DB *database){
	db = database;
	iter = NULL;
}

DatabaseManager::~DatabaseManager() {
	db = NULL;
	deleteIterator();
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

void DatabaseManager::createIterator() {
	iter = db->NewIterator(rocksdb::ReadOptions());
	iter->SeekToFirst();
}

bool DatabaseManager::advanceIterator() {
	if (iter == NULL || ! iter->Valid()) {
		return false;
	}
	iter->Next();
	return true;
}
bool DatabaseManager::getActualPair(std::string& key, std::string &value) {
	if (iter == NULL) {
		return false;
	}
	key = iter->key().ToString();
	value = iter->value().ToString();
	return true;
}

void DatabaseManager::deleteIterator() {
	if (iter == NULL) {
		return;
	}
	delete iter;
	iter = NULL;
}

bool DatabaseManager::validIterator() {
	if (iter == NULL)
		return false;
	return iter->Valid();
}
