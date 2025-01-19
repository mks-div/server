

#include "C:\projects\server\modules\utility\utility.h" 
#include <iostream>

using namespace std;

answer_format server_answer(const string& request) {

    cout << '|';
    cout << request;
    string response_body = "http://localhost:8080/index.html";
    
    
        
        
    return answer_format(response_body, "301", "POST");
}