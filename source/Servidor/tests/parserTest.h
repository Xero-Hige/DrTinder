/*
 * parserTest.h
 *
 *  Created on: Jun 12, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_TESTS_PARSERTEST_H_
#define SERVIDOR_TESTS_PARSERTEST_H_
#include <iostream>
#include "../Parsers/CsvParser.h"
#include "../Parsers/JsonParser.h"
#include "gtest/gtest.h"

TEST(CsvParser,ParseLine){
	CsvParser parser;
	string line = "\"Hola, como va?\",\"Probando \"comas y eso\",\" ultimo caso\"";
	std::vector<std::string> elem = parser.parseLine(&line);
	ASSERT_TRUE( elem.size() == 3 );
}

TEST(CsvParser,ParseInterest){
	CsvParser parser;
	Interests interests;
	string sport = "sport";
	string tennis = "tennis";
	string sex = "sex";
	string men = "men";
	string rugby = "rugby";
	string music = "music";
	string rock = "rock";
	string line = sport + "::" + tennis + "||" + sport + "::" + rugby + "||"
			+ sex + "::" + men + "||" + music + "::" + rock;
	parser.makeInterests(line,interests);
	ASSERT_TRUE( interests.has(sport,tennis) );
	ASSERT_TRUE( interests.has(sport,rugby) );
	ASSERT_TRUE( interests.has(sex,men) );
	ASSERT_TRUE( interests.has(music,rock) );
}


TEST(CsvParser,ParseInterestMakeItAgainHasAllInterests){
	CsvParser parser;
	Interests interests, interestsMade;
	string parsed, parsedMade;
	string sport = "sport";
	string tennis = "tennis";
	string sex = "sex";
	string men = "men";
	string rugby = "rugby";
	string music = "music";
	string rock = "rock";
	interests.add(sport,tennis);
	interests.add(sport,rugby);
	interests.add(sex,men);
	interests.add(music,rock);
	parsed = parser.interestToCsv(&interests);
	parser.makeInterests(parsed,interestsMade);

	ASSERT_TRUE( interestsMade.has(sport,tennis));
	ASSERT_TRUE( interestsMade.has(sport,rugby));
	ASSERT_TRUE( interestsMade.has(music,rock));
	ASSERT_TRUE( interestsMade.has(sex,men));
}

TEST(CsvParser, ParseUserMakeItAgainIsEqual){
	CsvParser parser;
	User user, user2;
	string parsed, parsed2;
	user.setAge(25);
	user.setCommonData("aaa@aaa.com","pepe","juan","man");
	user.setDescription("Hago lo que quiero lolollololo");
	user.setId(12);
	user.setPhoto("base64");
	user.setLocation(1.25,-1.4536);
	user.addInterest("sport","tennis");
	user.addInterest("sport","rugby");
	parsed = parser.userToCsvFull(&user);

	parser.makeUser(parsed,user2);
	parsed2 = parser.userToCsvFull(&user2);

	ASSERT_TRUE(parsed.compare(parsed2) == 0);
}

TEST(CsvParser, MakeCsvUserForClientWithOutID){
	CsvParser parser;
	User user;
	string parsed;
	user.setAge(25);
	user.setCommonData("aaa@aaa.com","pepe","juan","man");
	user.setDescription("Hago lo que quiero lolollololo");
	user.setId(12);
	user.setPhoto("base64");
	user.setLocation(1.25,-1.4536);
	user.addInterest("sport","tennis");
	user.addInterest("sport","rugby");
	parsed = parser.userToCsvForClient(&user,false);
	vector<string> elements = parser.parseLine(&parsed);
	ASSERT_TRUE(elements.size() == USER_DATA_FOR_CLIENT_COUNT);
}

TEST(CsvParser, MakeCsvUserForClientWithIDThenRemoveID){
	CsvParser parser;
	User user;
	string parsed;
	user.setAge(25);
	user.setCommonData("aaa@aaa.com","pepe","juan","man");
	user.setDescription("Hago lo que quiero lolollololo");
	user.setId(12);
	user.setPhoto("base64");
	user.setLocation(1.25,-1.4536);
	user.addInterest("sport","tennis");
	user.addInterest("sport","rugby");
	parsed = parser.userToCsvForClient(&user, true);
	vector<string> elements = parser.parseLine(&parsed);
	ASSERT_TRUE(elements.size() == USER_DATA_FOR_CLIENT_COUNT + 1);
	parser.removeId(parsed);
	elements = parser.parseLine(&parsed);
	ASSERT_TRUE(elements.size() == USER_DATA_FOR_CLIENT_COUNT);

}

TEST(CsvParser, CsvSignupUser){
	CsvParser parser;
	User user;
	string parsed, base;
	base = "\"name\",\"10\",\"Pepe\",\"aaa@aaa.com\",\"sex\",\"lookingFor\",\"interest_id1::interest1||interest_id2::interest2\"";
	parser.makeSignupUser(base,user);
	parsed = parser.userToCsvFull(&user);
	ASSERT_TRUE(user.getMail().compare("aaa@aaa.com") == 0);
	ASSERT_TRUE(user.getAlias().compare("Pepe") == 0);
}

TEST(CsvParser, CsvModifyUserFromBase){
	CsvParser parser;
	User user;
	string parsed, base, fromClient;
	fromClient =
			"\"name\",\"10\",\"sex\",\"lookingFor\",\"interest_id1::interest1||interest_id2::interest2\"";
	base = "\"1\",\"Pepe\",\"15\",\"Pepe\",\"aaa@aaa.com\",\"sex\",\"asd\",\"interest_id1::interest1||interest_id2::interest2\","
			"\"-0.153\",\"1.56345\"";
	parser.makePutUser(fromClient,base,user);
	parsed = parser.userToCsvFull(&user);
	ASSERT_TRUE(user.getAge() == 10);
	ASSERT_TRUE(user.getX() == -0.153f );
	ASSERT_TRUE(user.getDescription().compare("lookingFor") == 0 );
	ASSERT_TRUE(user.getName().compare("name") == 0);
}

TEST(JsonParser, ParseJsonPhoto){
	JsonParser json;
	string photo = "asdasdasdasd";
	string parsed = json.photoToJson(&photo);
	json.parsing(parsed);
	string photo_2 = json.getValue(PHOTO_PUT_KEY).asString();
	ASSERT_TRUE( photo.compare(photo_2) == 0);
}

TEST(JsonParser, ParseUserReMakeItStaysTheSame){
	JsonParser json;
	User user, user2;
	Json::Value parsed, parsed2;
	user.setAge(25);
	user.setCommonData("aaa@aaa.com","pepe","juan","man");
	user.setDescription("Hago lo que quiero lolollololo");
	user.setId(12);
	user.setPhoto("base64");
	user.setLocation(1.25,-1.4536);
	user.addInterest("sport","tennis");
	user.addInterest("sport","rugby");
	parsed = json.userToJson(&user);
	json.makeUser(parsed, user2);
	parsed2 = json.userToJson(&user2);
	ASSERT_TRUE( parsed == parsed2 );
}
#endif /* SERVIDOR_TESTS_PARSERTEST_H_ */
