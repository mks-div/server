

#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept> 
#include <unordered_map>

//#include "../OpenSSL-Win64/include/openssl/rand.h"
#include "../../includes/OpenSSL-Win64/include/openssl/rand.h"
#include "../utility/utility.h" 
#include "../calc/src/calculator.cpp"

using namespace std;

string get_date() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    char buffer[11];
    
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", now);
    return buffer;
}

string generateSecureToken(size_t length = 32) {
    vector<unsigned char> buffer(length);
    if (RAND_bytes(buffer.data(), static_cast<int>(buffer.size())) != 1) {
        throw runtime_error("Failed to generate random bytes");
    }
    ostringstream oss;
    oss << hex << setfill('0');
    for (unsigned char byte : buffer) {
        oss << setw(2) << static_cast<int>(byte);
    }
    return oss.str();
}

pair<string, string> Tokenizer() {
    return make_pair(generateSecureToken(), get_date());
}

user Authorize(const string& name, const pair<bool, string>& password, Database* DB) { 
    user answ;
    try {
        if (!password.first) { //token
            answ = DB->Authorize(name, password.second);
        }
        else {
            answ = DB->Authorize("", "", password.second);
        }
    } catch(string err) {
        cout << err;
        return user();
    }
    return answ;
}

user Register(const string& name, const string& password, const pair<string, string>& token, Database* DB) {
    try {
        DB->Insert(name, password, token.first, token.second);
        return Authorize(name, make_pair(true, token.first), DB);
    } catch(string err) {
        throw string {"can't register"};
    }
    return user();
}

Database create_db() {
    Database database;
    try { 
        database.Create_Connection();
    }
    catch(string err) {
        cout << err << endl;
    } 
    return database;
}


answer_format server_answer(const string& request) {
    vector<pair<string, string>> additional_info = {};
    pair<string, string> buf = {"", ""};
    Database DB = create_db();


    bool is_not_empty = false;
    for (int i = request.size(); (request[i] != '{' || i == 0); i--) {
        if (!is_not_empty) {
            if (request[i] == '}') is_not_empty = true;
            continue;
        }
        if (request[i] == ':') {
            buf.first = buf.second;
            buf.second = "";    
            continue;
        }
        else if (request[i] == ',') {
            additional_info.emplace_back(buf);
            //cout << buf.first << "|" << buf.second << endl;
            buf = {"", ""};
            continue;
        }
        else if (request[i] != '"') {
            buf.second = request[i] + buf.second;
        }
    } additional_info.emplace_back(buf); buf = {};
    

    cout << endl;
    
    unordered_map<string, string> fields;
    string Token_to_return;
    user us;

    for (pair<string, string> value: additional_info) { 
        fields.insert({value.second, value.first});
    } 

    if (fields.count("Expression")) {
        if (!Authorize("", make_pair(true, fields.at("Token")),  &DB).empty()) {
            cout << "ddf" << fields.at("Expression") << endl;

            try {

                if (!fields.at("Expression").empty()) {
                    string answ = to_string(Calculate(fields.at("Expression")));
                    cout << "@#$" << answ;
                    return answer_format("", "200", "POST", {{"answer", answ}});
                }

            } catch(...) {
                cout << "err" << endl;
            }

        }
        else {
            cout << "$Op";
            throw string("no permission");
        }
    }

    if (fields.count("ISREG") && !fields.at("ISREG").empty()) {
        us = Register(fields.at("Username"), fields.at("Password"), Tokenizer(), &DB);
    }

    else if(fields.count("ISAUT") && !fields.at("ISAUT").empty()) {
        us = Authorize(fields.at("Username"), (fields.count("Token") ? make_pair(true, fields.at("Token")) : make_pair(false, fields.at("Password")) ),  &DB);
    }

    Token_to_return = (!us.token.empty() ? us.token : "");
    cout << "@#$" << Token_to_return << endl;
    if (us.empty()) {
        throw string("no permission");
    }
        
    string response_body = "http://localhost:8080/extended_calculator.html";
    return answer_format("", "200", "POST", {{"token", Token_to_return}, {"redirect", response_body}});
}