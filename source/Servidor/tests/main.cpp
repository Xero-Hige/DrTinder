/*
 * InterestTest.cpp
 *
 *  Created on: Jun 3, 2016
 *      Author: tobias
 */

#include "interestTest.h"
#include "parserTest.h"
/*#include "sharedServerTest.h"
#include "tokenizerTest.h"
#include "userMatcherTest.h"
#include "messageHandlerTest.h"*/
#include "gtest/gtest.h"
#include "../../libs/loger/easylogging++.h"
#include "../ServerUtils.h"

INITIALIZE_EASYLOGGINGPP


int main(int argc, char **argv) {
  string port, shared, logCnfg;
  configure(argc,argv,port,shared,logCnfg);
  el::Configurations conf(logCnfg);
  el::Loggers::reconfigureAllLoggers(conf);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  LOGG(INFO) << "Port: " << port;
  LOGG(INFO) << "Shared: " << shared;
  LOGG(INFO) << "Loger configuration: " << logCnfg;
  LOGG(DEBUG) << "Corro test";
  ::testing::InitGoogleTest(&argc, argv);
  //RestClient::init();
  int result = RUN_ALL_TESTS();
  //RestClient::disable();
  LOGG(DEBUG) << "End test";
  return result;
}

