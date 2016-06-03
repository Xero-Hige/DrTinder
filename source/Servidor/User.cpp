/*
 * UserMock.cpp
 *
 *  Created on: Jun 2, 2016
 *      Author: tobias
 */

#include "User.h"


User::User(std::string userData, dataType type){
	switch(type){
	case(JSON_DATA):
			break;
	case(CSV_DATA):
			break;
	default:
		// TODO
		return;
	}
}

User::~User() {
	// TODO Auto-generated destructor stub
}

