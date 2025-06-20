

#include <iostream>
#include "file_manager.cpp"
#include "server_utilities.cpp"
#include "..\utility\logger.cpp"
#include "..\utility\utility.h"


using namespace std;


string get_req_type(const string& request) {
    string req_type = {};
    long long unsigned int idx = 0;

    while (request[idx] != ' ' && idx < request.size()) { 
        req_type += request[idx];
        idx ++;
    }

    if (!server_values::REQUEST_TYPES.count(req_type)) {
        throw string {"request type is not maintained"};
    }

    return req_type;
}


answer_format read_request(const SOCKET& socket, const int& BUFFER_SIZE) {
    char buff[BUFFER_SIZE] = {0};
    int bytes = recv(socket, buff, BUFFER_SIZE, 0);

    if (bytes < 0) {
        println("can't read client request", ERROR_STYLE);
        return {"500", "Internal Server Error: Failed to read request"};
    }

    
    try {
        string request_type = get_req_type(buff);
        cout << request_type << endl;

        if (request_type == "GET") {
            return get_file(buff);
        }

        if (request_type == "POST") {
            return server_answer(buff);
        }


    } catch (const string& e) {
        if (e == "can't read file" || e == "problems with path" ) {
            return answer_format(read_file("errors/404/index.html"), "html");
        } else if (e == "no permission") {
            return answer_format("403", "Internal Server Error: NO ACCESS", "ERROR");
        }
    }

    return answer_format(read_file("errors/404/index.html"), "html");
}

string make_server_msg(const answer_format& answer_body) {
    string serverMessage;

    if (answer_body.request_type == "GET") {
        string filetype = (answer_body.answer_type != "png" ? "text" : "image");
        cout << "|" << "GET" << endl; 
        serverMessage = "HTTP/1.1 200 OK\nContent-Type: " 
            + filetype
            + "/" 
            + answer_body.answer_type
            + "\nContent-Length: "
            + to_string(answer_body.answer_data.size())
            + "\nAccess-Control-Allow-Origin: *"
            + "\nAccess-Control-Allow-Methods: POST, GET, OPTIONS"
            + "\nAccess-Control-Allow-Headers: Content-Type"
            + "\n\n"
            + answer_body.answer_data;     
    } 
    else if (answer_body.request_type == "POST") {
        cout << "|" << "POST" << endl; 
        serverMessage = "HTTP/1.1 " 
        + answer_body.answer_type 
        + "\nContent-Type: application/json" 
        + "\nAccess-Control-Allow-Origin: *"
        + "\nAccess-Control-Allow-Methods: POST, GET, OPTIONS"
        + "\nAccess-Control-Allow-Headers: Content-Type"
        + "\n\n";
    }
    else if (answer_body.request_type == "ERROR") {
        serverMessage = "HTTP/1.1 " 
        + answer_body.answer_data + answer_body.answer_type 
        + "\nContent-Type: application/json" 
        + "\nLocation: " 
        + "\nAccess-Control-Allow-Origin: *"
        + "\nAccess-Control-Allow-Methods: POST, GET, OPTIONS"
        + "\nAccess-Control-Allow-Headers: Content-Type";
    }

    
    if (!answer_body.additional_info.empty()) {
        serverMessage +=  "\n";
        serverMessage +=  "{";
        for (pair<string, string> additional_par : answer_body.additional_info) {
            serverMessage +=  "\"" + additional_par.first + "\":\"" + additional_par.second + "\",";
        }
        serverMessage.pop_back();
        serverMessage +=  "}";
    }
    
    //cout << serverMessage << endl;
    return serverMessage;
}

void answer_request(const SOCKET& socket, const string& serverMessage) {
    int bytesSent = 0;
    int totalBytesSent = 0;

    while (totalBytesSent < serverMessage.size()) {
        bytesSent = send(socket, serverMessage.c_str(), serverMessage.size(), 0);
        if (bytesSent < 0) {
            println("can't send response", ERROR_STYLE);
            break;
        }
        totalBytesSent += bytesSent;
    }

    println("response sent", WARNING_STYLE);
}

void process_request(SOCKET socket, const int& BUFFER_SIZE) {
    if (socket == INVALID_SOCKET) {
        println("can't accept", ERROR_STYLE);
        return;
    }

    try {
        answer_request(socket, make_server_msg(read_request(socket, BUFFER_SIZE)));
    } catch (const exception& e) {
        println("Unexpected error: " + string(e.what()), ERROR_STYLE);

    }
    closesocket(socket);
}