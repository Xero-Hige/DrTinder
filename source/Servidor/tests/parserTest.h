/*
 * parserTest.h
 *
 *  Created on: Jun 12, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_TESTS_PARSERTEST_H_
#define SERVIDOR_TESTS_PARSERTEST_H_
#include <iostream>
#include "../Parser.h"
#include "../Parsers/CsvParser.h"
#include "gtest/gtest.h"

TEST(Parser,UrlAtIndexExists){
	Parser parser;
	std::string url = "0/1/2/3/4/5/6";
	std::string val;

	ASSERT_TRUE(parser.urlAt(url,0,val));
}

TEST(Parser,UrlAtIndexSameValue){
	Parser parser;
	std::string url = "0/1/2/3/4/5/6";
	std::string val;

	ASSERT_TRUE(parser.urlAt(url,0,val));
	ASSERT_TRUE(val.compare("0") == 0);
}

TEST(Parser,UrlAtLasElement){
	Parser parser;
	std::string url = "0/1/2/3/4/5/6";
	std::string val;

	ASSERT_TRUE(parser.urlAt(url,6,val));
	ASSERT_TRUE(val.compare("6") == 0);
}

TEST(Parser,UrlAtIndexDoesNotExist){
	Parser parser;
	std::string url = "0/1/2/3/4/5/6";
	std::string val;

	ASSERT_FALSE(parser.urlAt(url,7,val));
}

TEST(Parser,UrlIndexDoesNotExistLasElementUrlWithBar){
	Parser parser;
	std::string url = "0/1/2/3/4/5/6/";
	std::string val;

	ASSERT_FALSE(parser.urlAt(url,7,val));
}

TEST(Parser,UrlIndexLasElementUrlWithBar){
	Parser parser;
	std::string url = "0/1/2/3/4/5/6/";
	std::string val;

	ASSERT_TRUE(parser.urlAt(url,6,val));
	ASSERT_TRUE(val.compare("6") == 0);
}

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


#endif /* SERVIDOR_TESTS_PARSERTEST_H_ */
