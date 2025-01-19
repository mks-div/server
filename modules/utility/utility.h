#pragma once
#include <iostream>
#include <unordered_set>
using namespace std;

namespace server_values {
    const string path_to_dir = "C:/projects/server";
    const unordered_set<string> REQUEST_TYPES = {
        "GET",
        "POST"
    };

    const char* IP_ADDRESS = "127.0.0.1";
    const int PORT = 8080;
    const int BUFFER_SIZE = 30720;

}

struct answer_format {
    string answer_data;
    string answer_type;
    string request_type;
    
    answer_format(string ans_data, string ans_type="text", string req_type="GET") 
        : answer_data(ans_data), answer_type(ans_type), request_type(req_type) {}
};


