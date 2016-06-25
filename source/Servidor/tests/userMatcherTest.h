/*
 * userMatcherTest.h
 *
 *  Created on: Jun 22, 2016
 *      Author: gaston
 */

#ifndef SERVIDOR_TESTS_USERMATCHERTEST_H_
#define SERVIDOR_TESTS_USERMATCHERTEST_H_

#include <gtest/gtest.h>
#include <list>

#include "../Interests.h"
#include "../matches/UserMatcher.h"
#include "../User.h"

using namespace std;

int expectedNumberOfMatches = 2;

User createAppUser(){
	User user;
	user.setName("Pablo");
	user.setAge(21);
	user.setLocation(30.2, 15.5);
	user.setSex("man");
	Interests interests;
	interests.add("musica", "beatles");
	interests.add("musica", "acdc");
	interests.add("deportes", "basket");
	user.setInterests(interests);
	return user;
}

list<User*> createPossibleMatches(){

	User* user1 = new User();
	user1->setName("Luica");
	user1->setAge(22);
	user1->setLocation(30.0, 16.5);
	user1->setSex("woman");
	Interests interests1;
	interests1.add("deportes", "basket");
	interests1.add("hobbies", "videojuegos");
	user1->setInterests(interests1);

	User* user2 = new User();
	user2->setName("Sofia");
	user2->setAge(20);
	user2->setLocation(28, 16);
	user2->setSex("woman");
	Interests interests2;
	interests2.add("musica", "acdc");
	user2->setInterests(interests2);

	User* user3 = new User();
	user3->setName("Melisa");
	user3->setAge(30);
	user3->setLocation(30.0, 14.5);
	user3->setSex("woman");
	Interests interests3;
	interests3.add("musica", "beatles");
	interests3.add("musica", "rolling stones");
	user3->setInterests(interests1);

	User* user4 = new User();
	user4->setName("Juan");
	user4->setAge(21);
	user4->setLocation(30.2, 15.5);
	user4->setSex("man");
	Interests interests4;
	interests4.add("musica", "beatles");
	interests4.add("musica", "acdc");
	interests4.add("deportes", "basket");
	user4->setInterests(interests4);

	list<User*> possibleMatches;
	possibleMatches.push_back(user1);
	possibleMatches.push_back(user2);
	possibleMatches.push_back(user3);
	possibleMatches.push_back(user4);

	return possibleMatches;
}

TEST(UserMatcher,FilterPossibleMatches){

	User appUser = createAppUser();
	UserMatcher matcher;

	list<User*> possibleMatches = createPossibleMatches();

	list<User*> filteredPossibleMatches = matcher.filterPossibleMatches(&appUser, &possibleMatches);

	ASSERT_EQ(filteredPossibleMatches.size(), expectedNumberOfMatches);

	while(!possibleMatches.empty()){
		delete possibleMatches.front();
		possibleMatches.front() = NULL;
		possibleMatches.pop_front();
	}

}



#endif /* SERVIDOR_TESTS_USERMATCHERTEST_H_ */
