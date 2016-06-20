/*
 * messageHandlerTest.h
 *
 *  Created on: Jun 20, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_TESTS_MESSAGEHANDLERTEST_H_
#define SERVIDOR_TESTS_MESSAGEHANDLERTEST_H_

#include "../MessageHandler.h"
#include "gtest/gtest.h"

using namespace rocksdb;
using namespace std;

TEST(MsgHandler,CreateTokenAndItsValid){
	DB* db;
	DatabaseManager dbManager(db);
	string user = "aaa@aaa.com", password = "pepe";
	MessageHandler handler(&dbManager, user, password);
	string token = handler.getToken();
	ASSERT_TRUE(handler.validateToken(token));

}

TEST(MsgHandler,CanAuthenticateUser){
	DB* db;
	DatabaseManager dbManager(db);
	string user = "aaa@aaa.com", password = "pepe";
	MessageHandler handler(&dbManager, user, password);
	ASSERT_TRUE(handler.authenticate(user,password));
}


#endif /* SERVIDOR_TESTS_MESSAGEHANDLERTEST_H_ */
