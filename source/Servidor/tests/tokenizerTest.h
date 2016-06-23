/*
 * tokenizerTest.h
 *
 *  Created on: Jun 3, 2016
 *      Author: tobias
 */

#ifndef TOKENIZERTEST_H_
#define TOKENIZERTEST_H_
#include "gtest/gtest.h"
#include "../Tokenizer.h"
#include "setUpDataBase.h"
using namespace rocksdb;
using namespace std;

#define TEST_TOKEN_DB "TestUserDB"

TEST(Tokenizer,CreatesTokenizerCheckIfExpiresInmediately){
	DB* db;
	setUpDatabaseTest(&db,TEST_TOKEN_DB);
	DatabaseManager dbManager(db);
	Tokenizer tokenizer(&dbManager);
	std::string token = tokenizer.newToken("aaa@aaa.com","asdasd");
	EXPECT_FALSE(tokenizer.hasExpired(token));
	tokenizer.remove(token);
	delete db;
}

 //Test lleva tiempo, por eso lo comento
TEST(Tokenizer,CreatesTokenizerWaitForItToExpire){
	DB* db;
	setUpDatabaseTest(&db,TEST_TOKEN_DB);
	DatabaseManager dbManager(db);
	Tokenizer tokenizer(&dbManager);
	std::string token = tokenizer.newToken("aaa@aaa.com","asdasd");
	sleep( (EXPIRATION_TIME + 1) );
	ASSERT_TRUE(tokenizer.hasExpired(token));
	delete db;
}

TEST(Tokenizer,CreatesAndRemoveTokenItExpires){
	DB* db;
	setUpDatabaseTest(&db,TEST_TOKEN_DB);
	DatabaseManager dbManager(db);
	Tokenizer tokenizer(&dbManager);
	std::string token = tokenizer.newToken("aaa@aaa.com","asdasd");
	tokenizer.remove(token);
	ASSERT_TRUE(tokenizer.hasExpired(token));
	delete db;
}

TEST(Tokenizer, CreatesManyTokensCheckExistanceOfAll){
	DB* db;
	setUpDatabaseTest(&db,TEST_TOKEN_DB);
	DatabaseManager dbManager(db);
	Tokenizer tokenizer(&dbManager);
	std::string token = tokenizer.newToken("aaa@aaa.com","asdasd");
	std::string token1 = tokenizer.newToken("bbb@bbb.com","asdasd");
	std::string token2 = tokenizer.newToken("ccc@ccc.com","asdasd");
	EXPECT_FALSE(tokenizer.hasExpired(token));
	EXPECT_FALSE(tokenizer.hasExpired(token1));
	EXPECT_FALSE(tokenizer.hasExpired(token2));
	tokenizer.remove(token);
	tokenizer.remove(token1);
	tokenizer.remove(token2);
	delete db;
}


#endif /* TOKENIZERTEST_H_ */
