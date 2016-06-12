/*
 * parserTest.h
 *
 *  Created on: Jun 12, 2016
 *      Author: tobias
 */

#ifndef SERVIDOR_TESTS_PARSERTEST_H_
#define SERVIDOR_TESTS_PARSERTEST_H_
#include <iostream>
#include "../Parser.h"
#include "gtest/gtest.h"

TEST(Parser,UrlAtIndexExists){
	Parser parser;
	std::string url = "0/1/2/3/4/5/6";
	std::string val;

	ASSERT_TRUE(parser.urlAt(url,0,val));
}

TEST(Parser,UrlAtIndexSameValue){
	Parser parser;
	std::string url = "0/1/2/3/4/5/6";
	std::string val;

	ASSERT_TRUE(parser.urlAt(url,0,val));
	ASSERT_TRUE(val.compare("0") == 0);
}

TEST(Parser,UrlAtLasElement){
	Parser parser;
	std::string url = "0/1/2/3/4/5/6";
	std::string val;

	ASSERT_TRUE(parser.urlAt(url,6,val));
	ASSERT_TRUE(val.compare("6") == 0);
}

TEST(Parser,UrlAtIndexDoesNotExist){
	Parser parser;
	std::string url = "0/1/2/3/4/5/6";
	std::string val;

	ASSERT_FALSE(parser.urlAt(url,7,val));
}


#endif /* SERVIDOR_TESTS_PARSERTEST_H_ */
