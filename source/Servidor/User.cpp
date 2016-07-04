/*
 * UserMock.cpp
 *
 *  Created on: Jun 2, 2016
 *      Author: tobias
 */

#include "User.h"

User::User() : id(0), age(18) {
	location[0] = 0;
	location[1] = 0;
}

void User::setInterests(Interests interest){
	this->interests = interest;
}

void User::addInterest(string category, string value){
	this->interests.add(category, value);
}

void User::removeInterest(string category, string value){
	this->interests.remove(category,value);
}

void User::setId(int id){
	this->id = id;
}

void User::setAge(int age){
	this->age = age;
}
void User::setLocation(float x, float y){
	this->location[LOC_X] = x;
	this->location[LOC_Y] = y;
}

void User::setMail(std::string mail){
	this->mail = mail;
}

void User::setAlias(std::string alias){
	this->alias = alias;
}

void User::setName(std::string name){
	this->name = name;
}

void User::setSex(std::string sex){
	this->sex = sex;
}

void User::setCommonData(std::string mail, std::string alias, std::string name, std::string sex){
	this->mail = mail;
	this->alias = alias;
	this->name = name;
	this->sex = sex;
}

void User::setDescription(std::string description){
	this->description = description;
}

void User::setPhoto(std::string photo){
	this->photo = photo;
}

std::string User::getMail(){
	return this->mail;
}
std::string User::getName(){
	return this->name;
}
std::string User::getAlias(){
	return this->alias;
}
std::string User::getDescription(){
	return this->description;
}
std::string User::getPhoto(){
	return this->photo;
}
std::string User::getSex(){
	return this->sex;
}
int User::getID(){
	return this->id;
}
int User::getAge(){
	return this->age;
}
Interests* User::getInterests(){
	return &(this->interests);
}
float User::getX(){
	return this->location[LOC_X];
}
float User::getY(){
	return this->location[LOC_Y];
}

User::~User() {
	// TODO Auto-generated destructor stub
}

