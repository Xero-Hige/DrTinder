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
#include "../api_server_constants.h"
#include "../DatabaseManager.h"
#include "../ChatDatabaseManager.h"
#include "../LikesDatabaseManager.h"
#include "gtest/gtest.h"
using namespace rocksdb;
using namespace std;

TEST(DBManager,iterateCreateIterateItsValid){
	DB * database;
	setUpDatabaseTest(&database,US_HAND_TEST_DB);
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
	setUpDatabaseTest(&database,US_HAND_TEST_DB);
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
	delete database;
	ASSERT_TRUE(found);

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
	delete database;
	ASSERT_TRUE( chat.find(msg)!= std::string::npos );

}

TEST(CHDBManager,sendNewMessage){
	DB * database;
	setUpDatabaseTest(&database,CH_HAND_TEST_DB);
	ChatDatabaseManager mgr(database);
	string msg = "holaaa";
	string sender = "pepe@aa.com";
	string to = "aaa@aa.com";
	string chat;
	mgr.saveMessage(msg,sender,to);
	mgr.getNewMsgs(to, sender, chat);
	delete database;
	ASSERT_TRUE( chat.find(msg)!= std::string::npos );

}


TEST(LKDBManager,matches){
	DB * database;
	setUpDatabaseTest(&database,LIKE_HAND_TEST_DB);
	LikesDatabaseManager mgr(database);
	string user1 = "ww@WW.com";
	string user2 = "bb@bb.com";
	ASSERT_FALSE(mgr.matches(user1,user2));
	delete database;
}

TEST(LKDBManager, bothUsersLikeShouldMatch ){
	DB * database;
	setUpDatabaseTest(&database,LIKE_HAND_TEST_DB);
	string user1 = "aa@aa.com";
	string user2 = "bb@bb.com";
	LikesDatabaseManager mgr(database);
	mgr.saveLike(user1,user2,LIKED_TOKEN);
	mgr.saveLike(user2,user1,LIKED_TOKEN);
	ASSERT_TRUE(mgr.matches(user1,user2));
	delete database;
}

TEST(LKDBManager, oneLikeOtherDislikeShouldNotMatch ){
	DB * database;
	setUpDatabaseTest(&database,LIKE_HAND_TEST_DB);
	string user1 = "aaa@aaa.com";
	string user2 = "bbb@bbb.com";
	LikesDatabaseManager mgr(database);
	mgr.saveLike(user1,user2,LIKED_TOKEN);
	mgr.saveLike(user2,user1,DISLIKED_TOKEN);
	ASSERT_FALSE(mgr.matches(user1,user2));
	delete database;
}

TEST (LKDBManager, manyMatchesAllInNewAndMatches){
	DB * database;
	setUpDatabaseTest(&database,LIKE_HAND_TEST_DB);
	string newMatches, matches;
	string user1 = "aac@aac.com";
	string user2 = "bbc@bbc.com";
	string user3 = "bbd@bbd.com";
	string user4 = "bbe@bbe.com";
	LikesDatabaseManager mgr(database);
	mgr.saveLike(user1,user2,LIKED_TOKEN);
	mgr.saveLike(user1,user3,LIKED_TOKEN);
	mgr.saveLike(user1,user4,LIKED_TOKEN);
	mgr.saveLike(user2,user1,LIKED_TOKEN);
	mgr.saveLike(user3,user1,LIKED_TOKEN);
	mgr.saveLike(user4,user1,LIKED_TOKEN);

	mgr.getNewMatches(user1,newMatches);

	mgr.getMatches(user1,matches);

	delete database;
	ASSERT_NE(newMatches.find(user2), std::string::npos);
	ASSERT_NE(newMatches.find(user3), std::string::npos);
	ASSERT_NE(newMatches.find(user4), std::string::npos);

	ASSERT_NE(matches.find(user2), std::string::npos);
	ASSERT_NE(matches.find(user3), std::string::npos);
	ASSERT_NE(matches.find(user4), std::string::npos);

}

TEST (LKDBManager, oneSideLikeShouldInteractNotMatch){
	DB * database;
	setUpDatabaseTest(&database,LIKE_HAND_TEST_DB);
	LikesDatabaseManager mgr(database);
	string interacted, matches;
	string user1 = "aaac@aaac.com";
	string user2 = "bbbc@bbbc.com";

	mgr.saveLike(user1,user2,LIKED_TOKEN);

	mgr.getMatches(user1,matches);
	mgr.getInteractedUsers(user1,interacted);

	delete database;
	ASSERT_EQ(matches.find(user2),std::string::npos);
	ASSERT_NE(interacted.find(user2),std::string::npos);

}

TEST (LDBManager, allInteractedUsersIncludeMatchesAndNotMatches){
	DB * database;
	setUpDatabaseTest(&database,LIKE_HAND_TEST_DB);
	LikesDatabaseManager mgr(database);
	string interacted;
	string user1 = "aaac@aaac.com";
	string user2 = "bbbc@bbbc.com";
	string user3 = "ccc@ccc.com";
	string user4 = "ddde@ddde.com";

	mgr.saveLike(user1,user2,LIKED_TOKEN);
	mgr.saveLike(user1,user3,DISLIKED_TOKEN);
	mgr.saveLike(user1,user4,LIKED_TOKEN);

	mgr.saveLike(user2,user1,DISLIKED_TOKEN);
	mgr.saveLike(user3,user1,LIKED_TOKEN);
	mgr.saveLike(user4,user1,LIKED_TOKEN);

	mgr.getInteractedUsers(user1,interacted);
	delete database;

	ASSERT_NE(interacted.find(user2), std::string::npos);
	ASSERT_NE(interacted.find(user3), std::string::npos);
	ASSERT_NE(interacted.find(user4), std::string::npos);
}


#endif /* SERVIDOR_TESTS_DATABASESTEST_H_ */
