/*
 * databasesTest.h
 *
 *  Created on: Jun 27, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_TESTS_DATABASESTEST_H_
#define SERVIDOR_TESTS_DATABASESTEST_H_
#include "setUpDataBase.h"
#include "testDatabaseNames.h"
#include "../DatabaseManager.h"
#include "../ChatDatabaseManager.h"
#include "../LikesDatabaseManager.h"
#include "gtest/gtest.h"
using namespace rocksdb;
using namespace std;

TEST(DBManager,iterateCreateIterateItsValid){
	DB * database;
	setUpDatabaseTest(&database,CH_HAND_TEST_DB);
	DatabaseManager mgr(database);
	string key1="asdq",key2="asd13",key3="asd123";
	string value="asd";
	mgr.addEntry(key1,value);
	mgr.addEntry(key1,value);
	mgr.addEntry(key1,value);
	mgr.createIterator();
	ASSERT_TRUE(mgr.validIterator());
	mgr.deleteIterator();
	delete database;
}

TEST(DBManager,iterateDBHasActualPair){
	DB * database;
	setUpDatabaseTest(&database,CH_HAND_TEST_DB);
	DatabaseManager mgr(database);
	string key1="asdq",key2="asd13",key3="asd123";
	string value = "asd";
	string key_tmp, value_tmp;
	bool found = false;
	mgr.addEntry(key1, value);
	mgr.addEntry(key2, value);
	mgr.addEntry(key3, value);
	mgr.createIterator();
	do{
		mgr.getActualPair(key_tmp,value_tmp);
		if ((key3.compare(key_tmp) == 0) && (value_tmp.compare(value_tmp) == 0)){
			found = true;
			break;
		}
	}while (mgr.advanceIterator());
	mgr.deleteIterator();
	ASSERT_TRUE(found);
	delete database;
}

TEST(CHDBManager,messageHistoryAdd){
	DB * database;
	setUpDatabaseTest(&database,CH_HAND_TEST_DB);
	ChatDatabaseManager mgr(database);
	string msg = "holaaa";
	string sender = "pepe@aa.com";
	string to = "aaa@aa.com";
	string chat;
	mgr.saveMessage(msg,sender,to);
	mgr.getHistory(sender,to,chat);
	ASSERT_TRUE( chat.find(msg)!= std::string::npos );
	delete database;
}

TEST(LKDBManager,matches){
	DB * database;
	setUpDatabaseTest(&database,CH_HAND_TEST_DB);
	LikesDatabaseManager mgr(database);
	string user1 = "aa@aa.com";
	string user2 = "bb@bb.com";
	ASSERT_FALSE(mgr.matches(user1,user2));
	delete database;
}



#endif /* SERVIDOR_TESTS_DATABASESTEST_H_ */
