cmake_minimum_required(VERSION 2.8)

PROJECT (Servidor)

SET( CMAKE_VERBOSE_MAKEFILE on )

INCLUDE_DIRECTORIES(../libs/mongoose)
INCLUDE_DIRECTORIES(Handlers)
INCLUDE_DIRECTORIES(Parsers)
INCLUDE_DIRECTORIES(../libs/jsoncpp/dist/json)
INCLUDE_DIRECTORIES(../libs/rocksdb/include/rocksdb/)
INCLUDE_DIRECTORIES(../libs/rocksdb/include/rocksdb/utilities/)
INCLUDE_DIRECTORIES(../libs/swift/Swiften)
INCLUDE_DIRECTORIES(../libs/googletest)
INCLUDE_DIRECTORIES(/usr/lib/x86_64-linux-gnu/)

LINK_DIRECTORIES(/usr/local/lib/ /usr/lib /usr/lib/x86_64-linux-gnu/ )
SET(SERVER_LIBS rt libidn.so libresolv.so expat pthread rocksdb z bz2 snappy curl restclient-cpp xml2 gcov)
SET(TEST_LIBS ${SERVER_LIBS} gtest)

SET(SSL_LIBS ssl crypto)
SET(COMMON_SRCS ../libs/mongoose/mongoose.c
    Server.cpp
	 ServerUtils.cpp
	 RequestHandler.cpp
    DatabaseManager.cpp
    MessageHandler.cpp
    SharedServerClient.cpp
	 ../libs/jsoncpp/dist/jsoncpp.cpp
	 User.cpp
	 Interests.cpp
	 Tokenizer.cpp
	 matches/UserMatcher.cpp
	 Parsers/CsvParser.cpp
	 Parsers/JsonParser.cpp
	 Parsers/UserParser.cpp
	 ChatDatabaseManager.cpp
	 LikesDatabaseManager.cpp
)
SET (Servidor_SRCS main.cpp ${COMMON_SRCS})

SET (TEST_SRCS tests/main.cpp)
SET (COMMON_FLAGS "-g -W -Wall -Wno-unused-function")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMMON_FLAGS} -fprofile-arcs -ftest-coverage --coverage")
SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMMON_FLAGS}")

ADD_DEFINITIONS(-std=c++11)
ADD_EXECUTABLE (server ${Servidor_SRCS})

ADD_DEFINITIONS(-DMG_ENABLE_SSL)
ADD_DEFINITIONS(-DMG_ENABLE_THREADS)
TARGET_LINK_LIBRARIES(server ${SERVER_LIBS})
TARGET_LINK_LIBRARIES(server ${SSL_LIBS})

ADD_EXECUTABLE (Test tests/main.cpp ${COMMON_SRCS})

TARGET_LINK_LIBRARIES(Test ${SSL_LIBS})
TARGET_LINK_LIBRARIES(Test ${TEST_LIBS})


