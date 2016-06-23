/*
 * UserMatcher.h
 *
 *  Created on: Jun 22, 2016
 *      Author: gaston
 */

#ifndef SERVIDOR_MATCHES_USERMATCHER_H_
#define SERVIDOR_MATCHES_USERMATCHER_H_

#include <cstdbool>
#include <list>

#include "../User.h"

class UserMatcher {
private:
	const int AGE_RANGE = 3;
	const int MAX_DISTANCE = 20;

	bool isSameSex(User* appUser, User* user);
	bool isInAgeRange(User* appUser, User* user);
	bool isNearby(User* appUser, User* user);
	bool hasCommonInterest(User* appUser, User* user);


public:

	UserMatcher();

	list<User*> filterPossibleMatches(User* appUser, list<User*>* users);

	virtual ~UserMatcher();

};

#endif /* SERVIDOR_MATCHES_USERMATCHER_H_ */
