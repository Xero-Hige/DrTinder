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
#include "../Parsers/UserParser.h"
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
TEST(CsvParser, MakeFromCsvWithouIdHasEveryData){
	CsvParser parser;
	User user;
	string parsed, base, fromClient;
	base = "\"Pepe\",\"15\",\"Pepe\",\"aaa@aaa.com\",\"sex\",\"asd\",\"interest_id1::interest1||interest_id2::interest2\","
				"\"-0.153\",\"1.56345\"";
	parser.makeUser(base, user);
	ASSERT_TRUE(user.getName().compare("Pepe") == 0);
	ASSERT_TRUE(user.getAge() == 15);
	ASSERT_TRUE(user.getX() == -0.153f);
	ASSERT_TRUE(user.getY() == 1.56345f);
	ASSERT_TRUE(user.getAlias().compare("Pepe") == 0);
	ASSERT_TRUE(user.getMail().compare("aaa@aaa.com") == 0);
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

TEST(UserParser, TransformUsersToListThenToCsvSameAmountOfUsers){
	UserParser usParser;
	JsonParser json;
	string users =
			"{\"users\":[{\"user\":{\"id\":1,"
					"\"alias\":\"JeRo\",\"name\":\"Pedro\",\"email\":\"copito@gmail.com\","
					"\"sex\":\"man\",\"age\":21,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/1/photo\","
					"\"location\":{\"latitude\":-9.87,\"longitude\":1.234},\"interests\":[{\"category\":\"music\","
					"\"value\":\"rock\"},{\"category\":\"music/band\",\"value\":\"pearl jam\"},{\"category\":\"sport\",\"value\":\"tennis\"}]}},{\"user\":{\"id\":3,\"alias\":\"Leona\",\"name\":\"Diana\",\"email\":\"kdian@yahoo.com\",\"sex\":\"woman\",\"age\":null,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/3/photo\",\"location\":{\"latitude\":0,\"longitude\":0},\"interests\":[{\"category\":\"outdoors\",\"value\":\"running\"},{\"category\":\"music/band\",\"value\":\"radiohead\"},{\"category\":\"sex\",\"value\":\"man\"}]}},{\"user\":{\"id\":12,\"alias\":\"Junana\",\"name\":\"Juana\",\"email\":\"asd@usuasdario.com\",\"sex\":\"woman\",\"age\":23,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/12/photo\",\"location\":{\"latitude\":-121.45356,\"longitude\":46.51119},\"interests\":[{\"category\":\"music/band\",\"value\":\"radiohead\"},{\"category\":\"music/band\",\"value\":\"pearl jam\"},{\"category\":\"outdoors\",\"value\":\"running\"}]}},{\"user\":{\"id\":14,\"alias\":\"Alex\",\"name\":\"Alejando\",\"email\":\"elale@gmail.com\",\"sex\":\"man\",\"age\":26,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/14/photo\",\"location\":{\"latitude\":-3.23,\"longitude\":3.1416},\"interests\":[{\"category\":\"music\",\"value\":\"rock\"},{\"category\":\"music/band\",\"value\":\"pearl jam\"},{\"category\":\"sport\",\"value\":\"tennis\"}]}},{\"user\":{\"id\":15,\"alias\":\"Aru\",\"name\":\"Ariel\",\"email\":\"sirenita@gmail.com\",\"sex\":\"woman\",\"age\":31,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/15/photo\",\"location\":{\"latitude\":-3.23,\"longitude\":3.1416},\"interests\":[{\"category\":\"music\",\"value\":\"rock\"},{\"category\":\"music/band\",\"value\":\"pearl jam\"},{\"category\":\"sport\",\"value\":\"tennis\"}]}},{\"user\":{\"id\":16,\"alias\":\"ASD\",\"name\":\"Aladin\",\"email\":\"aaa@aaa.com\",\"sex\":\"man\",\"age\":25,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/16/photo\",\"location\":{\"latitude\":1.53,\"longitude\":-1.23},\"interests\":[{\"category\":\"music/band\",\"value\":\"pearl jam\"},{\"category\":\"sex\",\"value\":\"woman\"}]}},{\"user\":{\"id\":17,\"alias\":\"Juje\",\"name\":\"Juana\",\"email\":\"bbb@bbb.com\",\"sex\":\"woman\",\"age\":25,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/17/photo\",\"location\":{\"latitude\":1.33,\"longitude\":-1.6535896},\"interests\":[{\"category\":\"music/band\",\"value\":\"pearl jam\"},{\"category\":\"sex\",\"value\":\"man\"}]}},{\"user\":{\"id\":18,\"alias\":\"Lucho\",\"name\":\"Lucio\",\"email\":\"lllll@lllll.com\",\"sex\":\"man\",\"age\":28,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/18/photo\",\"location\":{\"latitude\":1.3946486,\"longitude\":-0.123546886},\"interests\":[{\"category\":\"music/band\",\"value\":\"radiohead\"},{\"category\":\"sex\",\"value\":\"woman\"},{\"category\":\"outdoors\",\"value\":\"walking\"}]}},{\"user\":{\"id\":19,\"alias\":\"Pato\",\"name\":\"Donald\",\"email\":\"tttrumpp@usa.com\",\"sex\":\"man\",\"age\":50,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/19/photo\",\"location\":{\"latitude\":0,\"longitude\":-1},\"interests\":[{\"category\":\"sex\",\"value\":\"woman\"},{\"category\":\"outdoors\",\"value\":\"running\"},{\"category\":\"sport\",\"value\":\"rugby\"}]}},{\"user\":{\"id\":21,\"alias\":\"Kalista\",\"name\":\"Karina\",\"email\":\"escudo@gmail.com\",\"sex\":\"woman\",\"age\":24,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/21/photo\",\"location\":{\"latitude\":-3.23,\"longitude\":1.1416},\"interests\":[{\"category\":\"music\",\"value\":\"rock\"},{\"category\":\"music/band\",\"value\":\"pearl jam\"},{\"category\":\"sport\",\"value\":\"tennis\"}]}},{\"user\":{\"id\":22,\"alias\":\"Pirulo\",\"name\":\"Patricio\",\"email\":\"pato@live.com.ar\",\"sex\":\"man\",\"age\":24,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/22/photo\",\"location\":{\"latitude\":-1.54,\"longitude\":1.23},\"interests\":[{\"category\":\"music/band\",\"value\":\"fun\"},{\"category\":\"sex\",\"value\":\"man\"},{\"category\":\"outdoors\",\"value\":\"running\"},{\"category\":\"outdoors\",\"value\":\"walking\"},{\"category\":\"sport\",\"value\":\"rugby\"}]}},{\"user\":{\"id\":25,\"alias\":\"Pirulo\",\"name\":\"Raul\",\"email\":\"aaa1@aaa.com\",\"sex\":\"man\",\"age\":25,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/25/photo\",\"location\":{\"latitude\":1.53,\"longitude\":-1.23},\"interests\":[{\"category\":\"music/band\",\"value\":\"pearl jam\"},{\"category\":\"music/band\",\"value\":\"fun\"},{\"category\":\"sex\",\"value\":\"man\"},{\"category\":\"outdoors\",\"value\":\"running\"}]}},{\"user\":{\"id\":31,\"alias\":\"asd\",\"name\":\"asd\",\"email\":\"asd@asd.com\",\"sex\":\"man\",\"age\":28,\"photo_profile\":\"http://dr-tinder.herokuapp.com/users/31/photo\",\"location\":{\"latitude\":1.563,\"longitude\":-1.3685},\"interests\":[{\"category\":\"music/band\",\"value\":\"pearl jam\"}]}}],\"metadata\":{\"version\":\"0.2\",\"count\":13}}";

	json.parsing(users);
	unsigned int count = json.getValue(META_KEY)[COUNT_KEY].asInt();
	list<User *> list = usParser.JsonToList(users);
	ASSERT_TRUE(list.size() == count);

	string csv_users = usParser.ListToCsv(list);
	unsigned int size = std::count(csv_users.begin(), csv_users.end(), '\n');
	ASSERT_TRUE(size == count);

	//delete from data
	while(!list.empty()) delete list.front(), list.pop_front();
}
#endif /* SERVIDOR_TESTS_PARSERTEST_H_ */
