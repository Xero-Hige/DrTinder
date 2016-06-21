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
	parsed = parser.userToCsv(&user);

	parser.makeUser(parsed,user2);
	parsed2 = parser.userToCsv(&user2);

	ASSERT_TRUE(parsed.compare(parsed2) == 0);
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
