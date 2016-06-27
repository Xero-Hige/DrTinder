#include "ServerUtils.h"

#define DEFAULT_LOG_CONFG "logCnfgs/default.conf"
#define DEBUG_LOG_CONFG "logCnfgs/debug.conf"
#define DEFAULT_CONFG "server.conf"
#define DEFAULT_PORT "8000"
#define DEFAULT_LOG "default"
#define DEBUG_LOG "debug"
#define DEFAULT_SHARED "http://dr-tinder.herokuapp.com"
#define CONFG_PORT_KEY "port"
#define CONFG_SHARED_KEY "shared"
#define CONFG_LOGFILE_KEY "logfile"

using namespace std;

void intToString(int number, std::string& result) {
    std::ostringstream convert;
    convert << number;
    result = convert.str();
}

int is_equal(const struct mg_str *s1, const std::string s2) {
    return mg_vcmp(s1, s2.c_str()) == 0;
}

bool equals(string s1,string s2){
	return s1.compare(s2) == 0;
}

bool isInteger(std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

bool isFloat( string myString ) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

void setCnfgValue(JsonParser* parser, string key, string &value_to_set){
	Json::Value value;
	value = parser->getValue(key);
	if (!value.empty()) {
		value_to_set = value.asString();
	}
}

void readConfigFile(string &port, string &shared, string &logCnfFile){
	ifstream config(DEFAULT_CONFG);
	if (config.is_open()){
		JsonParser parser;
		string line, buff="";
		while ( getline (config,line) ){
			buff += line;
		}
		if (parser.parsing(buff)){
			setCnfgValue(&parser, CONFG_PORT_KEY,port);
			setCnfgValue(&parser, CONFG_SHARED_KEY,shared);
			setCnfgValue(&parser, CONFG_LOGFILE_KEY,logCnfFile);
		}
		config.close();
	}
}

void configure(int argc, char **argv, string &port, string &shared) {
	string _port="", _shared="", _logCnf="", logCnfFile="";

	//primero lee .conf
	readConfigFile(_port,_shared,logCnfFile);

	//sobreescribe con comandos
	for (int i = 1; i + 1 < argc; i++) {
		string flag = string(argv[i]);
		if (equals(flag,"-p") || equals(flag,"--port")) {
			_port = argv[i + 1];
		} else if (equals(flag,"-s") || equals(flag,"--shared") ) {
			_shared = argv[i + 1];
		} else if (equals(flag,"-l") || equals(flag,"--log") ) {
			_logCnf = argv[i + 1];
		}else if (equals(flag,"-lf") || equals(flag,"--logfile")){
			logCnfFile = argv[i + 1];
		}
	}

	//set defaults
	if (_logCnf.empty()){
		_logCnf = DEFAULT_LOG;
	}

	if ( logCnfFile.empty() ){
		if (equals(_logCnf, DEBUG_LOG)) {
			logCnfFile = DEBUG_LOG_CONFG;
		} else {
			logCnfFile = DEFAULT_LOG_CONFG;
		}
	}

	//set logger
	el::Configurations conf(logCnfFile);
	el::Loggers::reconfigureAllLoggers(conf);
	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

	LOGG(INFO)<< "Logger took cnfg from: " << logCnfFile;

	if (_port.empty() || ! isInteger(_port)){
		_port = DEFAULT_PORT;
		LOGG(INFO) << "Port set to default";
	}else{
		LOGG(INFO) << "Server port set to: " << _port;
	}

	if (_shared.empty()){
		_shared = DEFAULT_SHARED;
		LOGG(INFO) << "Shared server set to defualt";
	}else{
		LOGG(INFO) << "Shared server link to: " << _shared;
	}

	port = _port;
	shared = _shared;

}
