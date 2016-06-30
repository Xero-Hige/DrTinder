/*
 * InterestTest.cpp
 *
 *  Created on: Jun 3, 2016
 *      Author: tobias
 */

#include "interestTest.h"
#include "parserTest.h"
#include "serverUtilsTest.h"
#include "databasesTest.h"
#include "sharedServerTest.h"
#include "tokenizerTest.h"
#include "userMatcherTest.h"
#include "messageHandlerTest.h"
#include "gtest/gtest.h"
#include "../../libs/loger/easylogging++.h"
#include "../ServerUtils.h"

INITIALIZE_EASYLOGGINGPP


int main(int argc, char **argv) {
  string port, shared;
  configure(argc,argv,port,shared);
  LOGG(DEBUG) << "Corro test";
  ::testing::InitGoogleTest(&argc, argv);
  RestClient::init();
  int result = RUN_ALL_TESTS();
  RestClient::disable();
  LOGG(DEBUG) << "End test";
  return result;
}

