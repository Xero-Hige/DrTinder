#include "MessageHandler.h"
#include "Tokenizer.h"

using std::string;

MessageHandler::MessageHandler(DatabaseManager *pDatabase, string name, string pass) :
		usersDB(pDatabase), username(name) {
	handlers.push_back((Handler *)new userHandler());
	handlers.push_back((Handler *) new usersHandler());
	handlers.push_back((Handler *) new friendsHandler());
	handlers.push_back((Handler *) new interestHandler());
	handlers.push_back((Handler *) new chatHandler());
}

MessageHandler::~MessageHandler() {
	std::vector<Handler*>::iterator it;
	for ( it = handlers.begin(); it != handlers.end(); ){
	      delete * it;
	      it = handlers.erase(it);
	}
}

string MessageHandler::divideMessage(string& message) {
	size_t pos = message.find(SEPARATOR);
	if (pos == std::string::npos) {
		pos = message.size();
	}
	string subMsg = message.substr(0, pos);
	message.erase(0, pos + 1);
	return subMsg;
}

HttpResponse MessageHandler::parse(string message) {
	Parser parser;
	HttpResponse resp;
	struct http_message hm;

	int err = mg_parse_http( message.c_str(), message.length(), &hm, 1);
	if (err <= 0){
		resp.turnToBadRequest("NO HTTP REQUEST");
	}else{
		std::string uri_start = parser.getUriStart(&hm);

		for (std::vector<Handler*>::iterator it = handlers.begin() ; it != handlers.end(); ++it){
			if ((*it)->manages(uri_start)){
				return (*it)->httpRequest(&hm);
			}
		}

		resp.turnToBadRequest("No existe handler");
	}
	return resp;

}

void MessageHandler::setUsersDB(DatabaseManager * usersDB) {
	this->usersDB = usersDB;
}

bool MessageHandler::getUsers(std::string& resultMsg) {
	ssClient.getUsers(&resultMsg);

	//	TODO: servicio para filtrar usuarios
	return true;
}

bool MessageHandler::authenticate(string username, string password) {
	return usersDB->correctEntry(username, password);
}

void MessageHandler::createUser(struct mg_str* user_data) {
	char name[20], pass[20];
	mg_get_http_var(user_data, "name", name, sizeof(name));
	mg_get_http_var(user_data, "pass", pass, sizeof(pass));
	//TODO: agregar otros parametros
	usersDB->addEntry(name, pass);
	//TODO: enviar datos a shared
}

void MessageHandler::updateUser(struct mg_str* user_data) {
	//TODO: cambiar datos en shared
}

void MessageHandler::deleteUser() {
	usersDB->deleteEntry(username);
	//TODO: quitar de shared
}

void MessageHandler::getInterest(std::string photo_64, std::string id_interest) {
	//TODO: obtener foto interes del shared y guardarlo en photo_64
}

void MessageHandler::getChat(string chat_history) {
	//TODO: get chat
}

void MessageHandler::getPhoto(string photo_64) {
	//TODO: obtener foto usuario del shared y guardarlo en photo_64
}

void MessageHandler::postPhoto(string photo_64) {
	//TODO: enviar photo_64 al shared
}

bool MessageHandler::validateToken(std::string user_token) {
	return (token.compare(user_token) == 0);
}

void MessageHandler::getMatches(std::string id) {
	//TODO: calcular matches
}

string MessageHandler::getToken() {
	return token;
}






















