/*
 * UserMatcher.h
 *
 *  Created on: Jun 22, 2016
 *      Author: gaston
 */

#ifndef SERVIDOR_MATCHES_USERMATCHER_H_
#define SERVIDOR_MATCHES_USERMATCHER_H_

#include <cmath>
#include <cstdbool>
#include <cstdlib>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "../Interests.h"
#include "../../libs/loger/easylogging++.h"
#include "../User.h"

class UserMatcher {
private:
	const int AGE_RANGE = 3;
	const int MAX_DISTANCE = 20;
	const string SEX_CATEGORY = "sex";

	bool isLookingForUserSex(User* appUser, User* user);
	bool isInAgeRange(User* appUser, User* user);
	bool isNearby(User* appUser, User* user);
	bool hasCommonInterest(User* appUser, User* user);
	bool contains(vector<string> vect, const string& stringToFind);
	bool containsKey(map<string, vector<string> >& map, const string& keyToFind);
	bool userInMatches(User* user, string userMatches);

public:

	UserMatcher();

	list<User*> filterPossibleMatches(User* appUser, list<User*>* users, string userMatches);

	virtual ~UserMatcher();

};

#endif /* SERVIDOR_MATCHES_USERMATCHER_H_ */
