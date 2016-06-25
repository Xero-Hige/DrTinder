#include "ServerUtils.h"

#define DEFAULT_CONFG "logCnfgs/default.conf"
#define DEBUG_CONFG "logCnfgs/debug.conf"
#define DEFAULT_PORT "8000"
#define DEFAULT_LOG "default"
#define DEBUG_LOG "debug"
#define DEFAULT_SHARED "http://dr-tinder.herokuapp.com"

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

void configure(int argc, char **argv, string &port, string &shared, string &logCnf) {
	string _port="", _shared="", _logCnf="";
	for (int i = 1; i + 1 < argc; i++) {
		string flag = string(argv[i]);
		if (equals(flag,"-p") || equals(flag,"-p")) {
			_port = argv[i + 1];
		} else if (equals(flag,"-s") || equals(flag,"--shared") ) {
			_shared = argv[i + 1];
		} else if (equals(flag,"-l") || equals(flag,"--log") ) {
			_logCnf = argv[i + 1];
		}
	}

	if (_port.empty()) _port = DEFAULT_PORT;
	if (_shared.empty()) _shared = DEFAULT_SHARED;
	if (_logCnf.empty()) _logCnf = DEFAULT_LOG;

	port = _port;
	shared = _shared;

	if ( equals(_logCnf,DEBUG_LOG)){
		logCnf = DEBUG_CONFG;
	}else{
		logCnf = DEFAULT_CONFG;
	}

}
