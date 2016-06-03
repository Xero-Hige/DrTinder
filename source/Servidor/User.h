/*
 * UserMock.h
 *
 *  Created on: Jun 2, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_USER_H_
#define SERVIDOR_USER_H_
#include <string>
#include "dataTypes.h"
#include "Interests.h"

#define USER_AGE "age"
#define USER_ID "id"
#define USER_MAIL "email"
#define USER_ALIAS "alias"
#define USER_PHOTO "photo_profile"
#define USER_
#define LOC_COORD 2
#define LOC_X 0
#define LOC_Y 1

class User {
public:
	User(std::string userData, dataType type);
	std::string exportTo(dataType type);
	bool matches(User otroUser);
	virtual ~User();
private:
	int id;
	int age;
	std::string mail;
	std::string alias;
	std::string name;
	std::string sex;
	std::string photo;
	float location[LOC_COORD];
	Interests interests;
};

#endif /* SERVIDOR_USER_H_ */
