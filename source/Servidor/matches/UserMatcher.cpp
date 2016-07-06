/*
 * UserMatcher.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: gaston
 */

#include "UserMatcher.h"

#include <algorithm>
#include <iterator>


UserMatcher::UserMatcher(){

}

/*
 * Deja en la lista los usuarios con las siguientes caracteristicas en relacion al usuario de la app:
 * 1) De sexo que se esta buscando.
 * 2) Con edad a no mas de AGE_RANGE anios de distancia.
 * 3) Localidad a menos de MAX_DISTANCE.
 * 4) Al menos un interes en comun.
 */
list<User*> UserMatcher::filterPossibleMatches(User* appUser, list<User*>* users, string userMatches) {
	list<User*> filteredUsers;
	LOGG(DEBUG) << "Filtering users.";
	LOGG(DEBUG) << "Past interactions with: " << userMatches;
	for (User* user : *users) {
		LOGG(INFO) << "Checking if user is a match candidate: " << user->getMail();
		if(!sameUser(appUser, user)){
			if(!userInMatches(user, userMatches)){
				if (isLookingForUserSex(appUser, user) && isInAgeRange(appUser, user) && isNearby(appUser, user)
						&& hasCommonInterest(appUser, user)) {
					LOGG(INFO) << "Found a match candidate: " << user->getMail();
					filteredUsers.push_back(user);
				}
			}
		}
	}

	return filteredUsers;

}

bool UserMatcher::sameUser(User* appUser, User* user){
	return (appUser->getID() == user->getID());
}

bool UserMatcher::userInMatches(User* user, string userMatches){
	string user_mail = user->getMail();
	CsvParser csv;
	std::istringstream f(userMatches);
	std::string line;
	while (std::getline(f, line)) {
		vector<string> elems = csv.parseLine(&line);
		if (elems[MAIL_FULL_IDX].compare(user_mail) == 0) {
			LOGG(DEBUG)<< "Already interacted with " << user->getMail();
			return true;
		}
	}
	return false;
}

bool UserMatcher::contains(vector<string> vect, const string& stringToFind) {
	return (find(vect.begin(), vect.end(), stringToFind) != vect.end());
}

bool UserMatcher::isLookingForUserSex(User* appUser, User* user) {
	string userSex = user->getSex();
	string category = SEX_CATEGORY;

	Interests* appUserInterests = appUser->getInterests();

	map<string,vector<string>> appUserInterestMap = appUserInterests->allInterests();

	if ( appUserInterestMap.find(category) != appUserInterestMap.end() ) {
		vector<string> sexesLookingFor = appUserInterestMap.at(category);
		if (contains(sexesLookingFor, userSex)) {
			return true;
		}
	}
	return false;
}

bool UserMatcher::isInAgeRange(User* appUser, User* user) {
	int diff = user->getAge() - appUser->getAge();
	return (abs(diff) <= AGE_RANGE);
}

bool UserMatcher::isNearby(User* appUser, User* user) {
	float userX = user->getX();
	float userY = user->getY();
	float appUserX = appUser->getX();
	float appUserY = appUser->getY();

	float distance = sqrt(pow(userX - appUserX, 2) + pow(userY - appUserY, 2));

	return (distance <= MAX_DISTANCE);

}

bool atLeastOneValueInCommon(vector<string> values1, vector<string> values2){
	for(string str1 : values1){
		for (string str2 : values2){
			if(str1 == str2){
				return true;
			}
		}
	}
	return false;
}

bool UserMatcher::containsKey(map<string, vector<string> >& map, const string& keyToFind) {
	return map.find(keyToFind) != map.end();
}

bool UserMatcher::hasCommonInterest(User* appUser, User* user) {
	Interests* userInterests = user->getInterests();
	Interests* appUserInterests = appUser->getInterests();

	map<string,vector<string>> userInterestMap = userInterests->allInterests();
	map<string,vector<string>> appUserInterestMap = appUserInterests->allInterests();

	for (map<string,vector<string>>::iterator it=userInterestMap.begin(); it!=userInterestMap.end(); ++it){
	    string category = it->first;
	    vector<string> values = it->second;
	    if (category != SEX_CATEGORY){
			if (containsKey(appUserInterestMap, category)) {
				vector<string> appUserValues = appUserInterestMap.at(category);
				if (atLeastOneValueInCommon(values, appUserValues)){
					return true;
				}
			}
	    }
	}
	return false;
}

UserMatcher::~UserMatcher() {
	// TODO Auto-generated destructor stub
}

