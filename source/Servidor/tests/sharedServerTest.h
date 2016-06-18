/*
 * sharedServerTest.h
 *
 *  Created on: Jun 15, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_TESTS_SHAREDSERVERTEST_H_
#define SERVIDOR_TESTS_SHAREDSERVERTEST_H_

#include <iostream>
#include <string>
#include "../SharedServerClient.h"
#include "../Parsers/JsonParser.h"
#include "gtest/gtest.h"

using namespace std;
SharedServerClient ss;

TEST(SharedServer,GetUsers){
	string data;
	bool correct = ss.getUsers(&data);
	ASSERT_TRUE(correct);
}

TEST(SharedServer,GetUserExistant){
	string data;
	bool correct = ss.getUser("1",&data);
	ASSERT_TRUE(correct);
}

TEST(SharedServer,GetUsersNotExistant){
	string data;
	bool correct = ss.getUser("2",&data);
	ASSERT_FALSE(correct);
}

TEST(SharedServer,GetInterests){
	string data;
	bool correct = ss.getUsersInterests(&data);
	ASSERT_TRUE(correct);
}

TEST(SharedServer,CreateUserAndDeleteIt){
	JsonParser parser;
	string user =
	"{\"user\": {"
		"\"name\": \"usuario\","
        "\"alias\": \"not a user\","
        "\"email\": \"usuario@usuario.com\","
        "\"sex\": \"man\","
        "\"age\": 30,"
		"\"interests\": [{"
			"\"category\": \"music/band\","
			"\"value\": \"radiohead\"},"
			"{\"category\": \"music/band\","
			"\"value\": \"pearl jam\"},"
			"{\"category\": \"outdoors\","
			"\"value\": \"running\"}],"
		"\"location\": {"
			"\"latitude\": -121.45356,"
			"\"longitude\": 46.51119 }},"
	   "\"metadata\": {"
       "\"version\": \"0.1\"  }}";
	string response;

	bool correct = ss.postUsers(response,&user);

	ASSERT_TRUE(correct);

	parser.parsing(response);
	string id = parser.getValue(USER_KEY)[ID_KEY].asString();

	bool deleted = ss.deleteUser(id,&response);

	ASSERT_TRUE(deleted);

}

TEST(SharedServer,CreateUserModifyItModifyPhotoAndDeleteIt){
	JsonParser parser;
	string user =
	"{\"user\": {"
		"\"name\": \"usuario\","
        "\"alias\": \"not a user\","
        "\"email\": \"usuario@usuario.com\","
        "\"sex\": \"man\","
        "\"age\": 30,"
		"\"interests\": [{"
			"\"category\": \"music/band\","
			"\"value\": \"radiohead\"},"
			"{\"category\": \"music/band\","
			"\"value\": \"pearl jam\"},"
			"{\"category\": \"outdoors\","
			"\"value\": \"running\"}],"
		"\"location\": {"
			"\"latitude\": -121.45356,"
			"\"longitude\": 46.51119 }},"
	   "\"metadata\": {"
       "\"version\": \"0.1\"  }}";

	string photo = "{\"photo\":\"comida\"}";
	string response;

	bool correct = ss.postUsers(response,&user);

	ASSERT_TRUE(correct);
	parser.parsing(response);
	string id = parser.getValue(USER_KEY)[ID_KEY].asString();
	string user_modified = "{\"user\": {"
				"\"id\":" + id + ","
				"\"name\": \"usuario\","
		        "\"alias\": \"not a user\","
		        "\"email\": \"usuario@usuario.com\","
		        "\"sex\": \"man\","
		        "\"age\": 30,"
				"\"photo_profile\":\"asdasdasd\","
				"\"interests\": [{"
					"\"category\": \"music/band\","
					"\"value\": \"radiohead\"},"
					"{\"category\": \"music/band\","
					"\"value\": \"pearl jam\"},"
					"{\"category\": \"outdoors\","
					"\"value\": \"running\"}],"
				"\"location\": {"
					"\"latitude\": -121.45356,"
					"\"longitude\": 46.51119 }},"
			   "\"metadata\": {"
		       "\"version\": \"0.1\"  }}";
	bool modified = ss.changeUser(id,user_modified);
	ASSERT_TRUE(modified);

	bool photo_modified = ss.changeUserPhoto(id,photo);
	ASSERT_TRUE(photo_modified);

	bool deleted = ss.deleteUser(id,&response);
	ASSERT_TRUE(deleted);

}


#endif /* SERVIDOR_TESTS_SHAREDSERVERTEST_H_ */
