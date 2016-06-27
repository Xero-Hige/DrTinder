/*
 * serverUtilsTest.h
 *
 *  Created on: Jun 26, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_TESTS_SERVERUTILSTEST_H_
#define SERVIDOR_TESTS_SERVERUTILSTEST_H_
#include "../ServerUtils.h"
#include "gtest/gtest.h"
#include <string>
using namespace std;

TEST(ServerUtils,stringIsFloat){
	string floats = "1.53";
	ASSERT_TRUE(isFloat(floats));
}
TEST(ServerUtils,stringIsNotFloatMixLetterAndNumb){
	string floats = "1.53asd";
	ASSERT_FALSE(isFloat(floats));
}

TEST(ServerUtils,stringIsNotFloat){
	string floats = "asdasd";
	ASSERT_FALSE(isFloat(floats));
}

TEST(ServerUtils, stringIsInt){
	string integer = "153";
	ASSERT_TRUE(isInteger(integer));
}

TEST(ServerUtils,stringIsNotInt){
	string notinteger = "asdasd";
	ASSERT_FALSE(isInteger(notinteger));
}

TEST(ServerUtils,stringIsNotIntMixed){
	string mixed = "153asd";
	ASSERT_FALSE(isInteger(mixed));
}

TEST(ServerUtils,configureDeafult){
	int argc = 1;
	char* argv[1] = {"server"};
	string port,shared;
	configure(argc,argv,port,shared);
	ASSERT_TRUE(port.compare(DEFAULT_PORT) == 0);
	ASSERT_TRUE(shared.compare(DEFAULT_SHARED) == 0);
}

TEST(ServerUtils,configurePortAndSharedNotDefault){
	int argc = 7;
	char port_param[] = "5000";
	char shared_param[] = "http://localhost:8000";
	char* argv[7] = {"server","-p",port_param,"-s",shared_param,"-l","debug"};
	string port,shared;
	configure(argc,argv,port,shared);
	ASSERT_FALSE(port.compare(DEFAULT_PORT) == 0);
	ASSERT_FALSE(shared.compare(DEFAULT_SHARED) == 0);
	ASSERT_TRUE(port.compare(port_param) == 0);
	ASSERT_TRUE(shared.compare(shared_param) == 0);

}

#endif /* SERVIDOR_TESTS_SERVERUTILSTEST_H_ */
