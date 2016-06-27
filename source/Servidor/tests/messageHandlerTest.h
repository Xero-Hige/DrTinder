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
#include "setUpDataBase.h"
#include "gtest/gtest.h"
#define US_HAND_TEST_DB "TestDB"
#define CH_HAND_TEST_DB "ChatTestDB"
using namespace rocksdb;
using namespace std;

TEST(MsgHandler,CreateTokenAndItsValid){
	DB* db;
	DB* dbc;
	setUpDatabaseTest(&db,US_HAND_TEST_DB);
	setUpDatabaseTest(&dbc,CH_HAND_TEST_DB);
	server_databases_t databases;
	databases.usersDB = db;
	databases.chatDB = dbc;
	string user = "aaa@aaa.com", password = "pepe";
	MessageHandler handler(&databases);
	handler.setUser(user);
	handler.setSharedLink(SHARED_SERVER_URL);
	string token = handler.getToken();
	ASSERT_TRUE(handler.validateToken(token));
	delete db;
	delete dbc;

}

TEST(MsgHandler,CantAuthenticateUserNotCreated){
	DB* db;
	DB* dbc;
	setUpDatabaseTest(&db,US_HAND_TEST_DB);
	setUpDatabaseTest(&dbc,CH_HAND_TEST_DB);
	server_databases_t databases;
	databases.usersDB = db;
	databases.chatDB = dbc;
	string user = "aaa@aaaa.com", password = "pepe";
	MessageHandler handler(&databases);
	handler.setUser(user);
	handler.setSharedLink(SHARED_SERVER_URL);
	ASSERT_FALSE(handler.authenticate(user,password));
	delete db;
	delete dbc;
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
	DB* dbc;
	setUpDatabaseTest(&db,US_HAND_TEST_DB);
	setUpDatabaseTest(&dbc,CH_HAND_TEST_DB);
	server_databases_t databases;
	databases.usersDB = db;
	databases.chatDB = dbc;
	string user = mail, password = "pepe";
	MessageHandler handler(&databases);
	handler.setUser(user);
	handler.setSharedLink(SHARED_SERVER_URL);
	ASSERT_TRUE(handler.createUser(complete, password));
	ASSERT_TRUE(handler.authenticate(mail,password));
	ASSERT_TRUE(handler.addLocalization("1.5638,-1.2536"));
	ASSERT_TRUE(handler.deleteUser());
	delete db;
	delete dbc;
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
	DB* dbc;
	setUpDatabaseTest(&db,US_HAND_TEST_DB);
	setUpDatabaseTest(&dbc,CH_HAND_TEST_DB);
	server_databases_t databases;
	databases.usersDB = db;
	databases.chatDB = dbc;
	string user = mail, password = "pepe", photo;
	MessageHandler handler(&databases);
	handler.setUser(user);
	handler.setSharedLink(SHARED_SERVER_URL);
	cout << complete.c_str() << endl;
	ASSERT_TRUE(handler.createUser(complete, password));
	ASSERT_TRUE(handler.postPhoto("pepe"));
	ASSERT_TRUE(handler.getPhoto(user, photo));
	ASSERT_TRUE(photo.compare("pepe") == 0);
	ASSERT_TRUE(handler.updateUser(modified));
	ASSERT_TRUE(handler.deleteUser());
	delete db;
	delete dbc;
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
	DB* dbc;
	setUpDatabaseTest(&db,US_HAND_TEST_DB);
	setUpDatabaseTest(&dbc,CH_HAND_TEST_DB);
	server_databases_t databases;
	databases.usersDB = db;
	databases.chatDB = dbc;
	string user = mail, password = "pepe", data;
	MessageHandler handler(&databases);
	handler.setUser(user);
	handler.setSharedLink(SHARED_SERVER_URL);
	ASSERT_FALSE(handler.getUser("asdas@aasdasda.com",data));
	delete db;
	delete dbc;

}

TEST(MsgHandler, GetInterestPhotoExistant){
	DB* db;
	DB* dbc;
	setUpDatabaseTest(&db,US_HAND_TEST_DB);
	setUpDatabaseTest(&dbc,CH_HAND_TEST_DB);
	server_databases_t databases;
	databases.usersDB = db;
	databases.chatDB = dbc;
	string user = "bbb@bbb.com", password = "pepe", photo;
	MessageHandler handler(&databases);
	handler.setUser(user);
	handler.setSharedLink(SHARED_SERVER_URL);
	ASSERT_TRUE(handler.getInterestPhoto(photo,"sport"));
	ASSERT_FALSE(handler.getInterestPhoto(photo,"pepe"));
	delete db;
	delete dbc;
}

TEST(MsgHandler,GetMatchesForUser){
	vector<string> user_data;
	string mail = "marcelo@prueba.com";
	user_data.resize(USER_DATA_FOR_CLIENT_COUNT);
	user_data[SEX_IDX] = "man";
	user_data[NAME_IDX] = "Marcelo";
	user_data[MAIL_IDX] = mail;
	user_data[ALIAS_IDX] = "Marce";
	user_data[AGE_IDX] = "21";
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
	DB* dbc;
	setUpDatabaseTest(&db,US_HAND_TEST_DB);
	setUpDatabaseTest(&dbc,CH_HAND_TEST_DB);
	server_databases_t databases;
	databases.usersDB = db;
	databases.chatDB = dbc;
	string user = mail, password = "pepe";
	MessageHandler handler(&databases);
	handler.setUser(user);
	handler.setSharedLink(SHARED_SERVER_URL);
	handler.createUser(complete, password);

	string users;
	bool result = handler.getUsers(users);
	LOGG(INFO) << "IMPRIMIENTDO USERS" << "\n";
	LOGG(INFO) << users << "\n";
	ASSERT_TRUE(result);
	//elimino para evitar creacion de user en tests consecutivos
	ASSERT_TRUE(handler.deleteUser());
	delete db;
	delete dbc;
}

#endif /* SERVIDOR_TESTS_MESSAGEHANDLERTEST_H_ */
