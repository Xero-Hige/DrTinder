#ifndef SOURCE_API_CONSTANTS_SERVER
#define SOURCE_API_CONSTANTS_SERVER

#define DB_SEPARATOR ","

#define CHAT_URI "/chats"
#define USER_URI "/user"
#define USER_ID_URI "/user/token/id"
#define USERS_URI "/users"
#define USERS_PHOTO_URI "/users/photo"
#define INTEREST_URI "/interest"
#define FRIENDS_URI "/friends"



#define TOKEN_VARIABLE_NAME "token"
#define AUTHORIZATION_HEADER "Authorization"
#define AUTHORIZATION_HEADER_USER "username"
#define AUTHORIZATION_HEADER_PASS "pass"
#define QUERY_STRING_USER "user"
#define QUERY_STRING_RESOURCE_ID "res_id"
#define BODY_USER "User"
#define USER_LOCATION_TOKEN "localization"
#define CONTENT_TYPE_HEADER_CSV "text/csv"
#define CONTENT_TYPE_HEADER_IMAGE "image"

#define FIREBASE_SERVER_SEND_URL "fcm-xmpp.googleapis.com"
#define FIREBASE_SERVER_PORT 5236
#define FIREBASE_SERVER_KEY "AIzaSyB_kX1_V5cHcD3zbWusTNNm3Q_vXkgPXJw"

#define LIKE_USER "user"
#define LIKE_CANDIDATE "candidate"
#define LIKE_TOKEN "liked"
#define LIKED_TOKEN "yes"
#define DISLIKED_TOKEN "no"

#define CHAT_MESSAGE_TOKEN "message"
#define CHAT_SENT_FROM_TOKEN "senderID"
#define CHAT_RECEIVER_TOKEN "receiver"
#define CHAT_SENDER_TOKEN "sender"

#define GET_S "GET"
#define POST_S "POST"
#define DELETE_S "DELETE"
#define PUT_S "PUT"
#define GET 1
#define POST 2
#define DELETE 3
#define PUT 4
#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define AUTHENTICATION_ERROR 403
#define STATUS_OK 200
#define CREATED 201
#define INVALID_TOKEN 498
#define NOT_IMPLEMENTED 501

#endif //SOURCE_API_CONSTANTS_SERVER
