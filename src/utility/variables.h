#pragma once
#include <iostream>
#include <unordered_set>
#include <cstdlib>
using namespace std;


namespace server_values {
    const string FILES_PATH = string(getenv("SERVER_FILES"));

    //const string SRC_PATH = PATH_TO_DIR + "/src";
    //const string INCLUDES_PATH = PATH_TO_DIR + "/includes";
    //const string FILES_PATH = PATH_TO_DIR + "/files";

    const string WEB_PATH = FILES_PATH + "/web";
    const string S_DB_PATH = FILES_PATH + "/db/users.db";

    
    const char * DB_PATH = S_DB_PATH.c_str();

    const unordered_set<string> REQUEST_TYPES = {
        "GET",
        "POST"
    };

    const char* IP_ADDRESS = "127.0.0.1";
    const int PORT = 8080;
    const int BUFFER_SIZE = 30720;
}