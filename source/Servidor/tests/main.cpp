/*
 * InterestTest.cpp
 *
 *  Created on: Jun 3, 2016
 *      Author: tobias
 */
#include "userMatcherTest.h"
#include "interestTest.h"
#include "parserTest.h"
#include "sharedServerTest.h"
#include "tokenizerTest.h"
#include "messageHandlerTest.h"
#include "gtest/gtest.h"
#include "../../libs/loger/easylogging++.h"
INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  LOGG(DEBUG) << "Corro test";
  ::testing::InitGoogleTest(&argc, argv);
  RestClient::init();
  int result = RUN_ALL_TESTS();
  RestClient::disable();
  LOGG(DEBUG) << "End test";
  return result;
}

