/*
 * messageHandlerTest.h
 *
 *  Created on: Jun 20, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_TESTS_MESSAGEHANDLERTEST_H_
#define SERVIDOR_TESTS_MESSAGEHANDLERTEST_H_
#include <vector>
#include "../MessageHandler.h"
#include "../Parsers/CsvParser.h"
#include "gtest/gtest.h"

using namespace rocksdb;
using namespace std;

TEST(MsgHandler,CreateTokenAndItsValid){
	DB* db;
	DatabaseManager dbManager(db);
	string user = "aaa@aaa.com", password = "pepe";
	MessageHandler handler(&dbManager, user);
	string token = handler.getToken();
	ASSERT_TRUE(handler.validateToken(token));

}

TEST(MsgHandler,CantAuthenticateUserNotCreated){
	DB* db;
	DatabaseManager dbManager(db);
	string user = "aaa@aaaa.com", password = "pepe";
	MessageHandler handler(&dbManager, user);
	ASSERT_FALSE(handler.authenticate(user,password));
}

TEST(MsgHandler,CreateAuthenticateAddLocalizationAndDeleteUser){
	CsvParser parse;
	vector<string> user_data;
	string mail = "aaaaa@aaa.com";
	user_data.resize(USER_DATA_FOR_CLIENT_COUNT);
	user_data[SEX_IDX] = "man";
	user_data[NAME_IDX] = "Juan";
	user_data[MAIL_IDX] = mail;
	user_data[ALIAS_IDX] = "Julepe";
	user_data[AGE_IDX] = "18";
	user_data[INT_IDX] = "sport::tennis";
	user_data[DSC_IDX] = "Quiero ser un maestro pokemon";
	string complete = "\"";
	for (unsigned int i =0; i < user_data.size(); i++){
		complete += user_data[i] + "\",\"";
	}
	if (complete.size() > 2){
		complete = complete.substr(0, complete.size() - 2);
	}

	DB* db;
	DatabaseManager dbManager(db);
	string user = mail, password = "pepe";
	MessageHandler handler(&dbManager, user);

	ASSERT_TRUE(handler.createUser(complete, password));
	ASSERT_TRUE(handler.authenticate(mail,password));
	ASSERT_TRUE(handler.addLocalization("1.5638,-1.2536"));
	ASSERT_TRUE(handler.deleteUser());
}

TEST(MsgHandler,CreateUpdatePutPhotoCheckPhotoAndDeleteUser){
	CsvParser parse;
	vector<string> user_data;
	string mail = "asdas@aasdasda.com" , modified;
	user_data.resize(USER_DATA_FOR_CLIENT_COUNT);

	user_data[SEX_IDX] = "man";
	user_data[NAME_IDX] = "Juan";
	user_data[MAIL_IDX] = mail;
	user_data[ALIAS_IDX] = "Julepe";
	user_data[AGE_IDX] = "18";
	user_data[INT_IDX] = "sport::tennis";
	user_data[DSC_IDX] = "Quiero ser un maestro pokemon";
	string complete = "\"";
	//without photo or id
	for (unsigned int i =0; i < user_data.size(); i++){
		complete += user_data[i] + "\",\"";
	}
	if (complete.size() > 2){
		complete = complete.substr(0, complete.size() - 2);
	}
	modified = "\"Pepe\",\"18\",\"man\",\"Pokemooon\",\"sport::tennis\"";
	DB* db;
	DatabaseManager dbManager(db);
	string user = mail, password = "pepe", photo;
	MessageHandler handler(&dbManager, user);
	cout << complete.c_str() << endl;
	ASSERT_TRUE(handler.createUser(complete, password));
	ASSERT_TRUE(handler.postPhoto("pepe"));
	ASSERT_TRUE(handler.getPhoto(user, photo));
	ASSERT_TRUE(photo.compare("pepe") == 0);
	ASSERT_TRUE(handler.updateUser(modified));
	ASSERT_TRUE(handler.deleteUser());
}

TEST(MsgHandler,GetOtherUser){
	//Depends on before Test
	CsvParser parse;
	vector<string> user_data;
	string mail = "bbb@ccc.com";
	user_data.resize(USER_DATA_FOR_CLIENT_COUNT);
	user_data[SEX_IDX] = "man";
	user_data[NAME_IDX] = "Juan";
	user_data[MAIL_IDX] = mail;
	user_data[ALIAS_IDX] = "Julepe";
	user_data[AGE_IDX] = "18";
	user_data[INT_IDX] = "sport::tennis";
	user_data[DSC_IDX] = "Quiero ser un maestro pokemon";
	string complete = "\"";
	//without photo or id
	for (unsigned int i = 0; i < user_data.size(); i++) {
		complete += user_data[i] + "\",\"";
	}
	if (complete.size() > 2) {
		complete = complete.substr(0, complete.size() - 2);
	}
	DB* db;
	DatabaseManager dbManager(db);
	string user = mail, password = "pepe", data;
	MessageHandler handler(&dbManager, user);
	ASSERT_FALSE(handler.getUser("asdas@aasdasda.com",data));

}

TEST(MsgHandler, GetInterestPhotoExistant){
	DB* db;
	DatabaseManager dbManager(db);
	string user = "bbb@bbb.com", password = "pepe", photo;
	MessageHandler handler(&dbManager, user);

	ASSERT_TRUE(handler.getInterestPhoto(photo,"sport"));
	ASSERT_FALSE(handler.getInterestPhoto(photo,"pepe"));
}

#endif /* SERVIDOR_TESTS_MESSAGEHANDLERTEST_H_ */
