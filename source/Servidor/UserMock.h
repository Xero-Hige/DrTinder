/*
 * UserMock.h
 *
 *  Created on: Jun 2, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_USERMOCK_H_
#define SERVIDOR_USERMOCK_H_
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

class UserMock {
public:
	UserMock(std::string userData, dataType type);
	std::string exportTo(dataType type);
	bool matches(UserMock otroUser);
	virtual ~UserMock();
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

#endif /* SERVIDOR_USERMOCK_H_ */
