cmake_minimum_required(VERSION 2.8)

PROJECT (Servidor)

SET( CMAKE_VERBOSE_MAKEFILE on )

INCLUDE_DIRECTORIES(../libs/mongoose)
INCLUDE_DIRECTORIES(Handlers)
INCLUDE_DIRECTORIES(Parsers)
INCLUDE_DIRECTORIES(../libs/jsoncpp/dist/json)
INCLUDE_DIRECTORIES(../libs/rocksdb/include/rocksdb/)
INCLUDE_DIRECTORIES(../libs/rocksdb/include/rocksdb/utilities/)

LINK_DIRECTORIES(/usr/local/lib/ /usr/lib /usr/lib/x86_64-linux-gnu/ )
SET(SERVER_LIBS pthread rocksdb z bz2 snappy)
SET(TEST_LIBS ${SERVER_LIBS} gtest)
SET(SSL_LIBS ssl crypto)
SET(COMMON_SRCS ../libs/mongoose/mongoose.c
    Server.cpp
    DatabaseManager.cpp
    MessageHandler.cpp
    Parser.cpp
    SharedServerClient.cpp
	 HttpResponse.cpp
	 Handlers/Handler.cpp
	 Handlers/userHandler.cpp
	 Handlers/chatHandler.cpp
	 Handlers/usersHandler.cpp
	 Handlers/friendsHandler.cpp
	 Handlers/interestHandler.cpp
	 ../libs/jsoncpp/dist/jsoncpp.cpp
	 User.cpp
	 Interests.cpp
	 Tokenizer.cpp
	 Parsers/CsvParser.cpp
	 Parsers/JsonParser.cpp
)
SET (Servidor_SRCS main.cpp ${COMMON_SRCS})

SET (TEST_SRCS tests/main.cpp ${COMMON_SRCS})
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Wno-unused-function")

ADD_DEFINITIONS(-std=c++11)

ADD_EXECUTABLE (server ${Servidor_SRCS})
ADD_EXECUTABLE (unit_test ${TEST_SRCS})

ADD_DEFINITIONS(-DMG_ENABLE_THREADS)

TARGET_LINK_LIBRARIES(server ${SERVER_LIBS})

ADD_DEFINITIONS(-DMG_ENABLE_SSL)

TARGET_LINK_LIBRARIES(server ${SSL_LIBS})
TARGET_LINK_LIBRARIES(unit_test ${TEST_LIBS})
TARGET_LINK_LIBRARIES(unit_test ${SSL_LIBS})
