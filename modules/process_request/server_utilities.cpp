

#include "..\utility\utility.h" 
#include <iostream>
#include <vector>

using namespace std;

answer_format server_answer(const string& request) {
    string answ = "";
    vector<pair<string, string>> additional_info = {};
    pair<string, string> buf = {};

    
    cout << request;

    for (int i = request.size(); (request[i] != '{' || i == 0); i--) {
        if (request[i] == ':') {
            buf.first = buf.second;
            buf.second = {};
            continue;
        }
        else if (request[i] == ',') {
            additional_info.emplace_back(buf);
            cout << buf.first << "|" << buf.second << endl;
            buf = {};
            continue;
        }
        else if (request[i] != '"' && request[i] != '}') {
            buf.second = request[i] + buf.second;
        }
    } additional_info.emplace_back(buf); buf = {};
    

    cout << endl;
    for (pair<string, string> value: additional_info) {
        cout << value.first << "|" << value.second << endl;
    }


    

    string response_body = "http://localhost:8080/";
    
    
        
        
    return answer_format(response_body, "301", "POST");
}