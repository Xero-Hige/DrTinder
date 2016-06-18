/*
 * UserMock.h
 *
 *  Created on: Jun 2, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_USER_H_
#define SERVIDOR_USER_H_

#include <string>
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
	User();
	bool matches(User otroUser);

	//set
	void setId(int id);
	void setAge(int age);
	void setLocation(float x, float y);
	void setMail(std::string mail);
	void setAlias(std::string alias);
	void setName(std::string name);
	void setSex(std::string sex);
	void setCommonData(std::string mail, std::string alias, std::string name, std::string sex);
	void setDescription(std::string description);
	void setPhoto(std::string photo);
	void setInterests(Interests interest);
	void addInterest(std::string category, std::string value);
	void removeInterest(std::string category, std::string value);

	//get
	std::string getMail();
	std::string getName();
	std::string getAlias();
	std::string getDescription();
	std::string getPhoto();
	std::string getSex();
	int getID();
	int getAge();
	Interests* getInterests();
	float getX();
	float getY();

	virtual ~User();
private:
	int id;
	int age;
	std::string mail;
	std::string alias;
	std::string name;
	std::string sex;
	std::string description;
	std::string photo;
	float location[LOC_COORD];
	Interests interests;
};

#endif /* SERVIDOR_USER_H_ */
