#include <iostream>
#include <future>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include "file_manager.cpp"
#include "logger.cpp"

using namespace std;
//hello world
void new_one(){

}

/*
string make_500_response(const string& error_message) {
    return "HTTP/1.1 500 Internal Server Error\r\n"
           "Content-Type: text/plain\r\n"
           "Content-Length: " + to_string(error_message.size()) + "\r\n"
           "\r\n"
           + error_message;
}*/

pair<string, string> read_request(const SOCKET& socket, const int& BUFFER_SIZE) {
    char buff[BUFFER_SIZE] = {0};
    int bytes = recv(socket, buff, BUFFER_SIZE, 0);
    if (bytes < 0) {
        println("can't read client request", ERROR_STYLE);
        return {"500", "Internal Server Error: Failed to read request"};
    }

    try {
        return get_file(buff); // Assume get_file may throw exceptions

    } catch (const string& e) {
        if (e == "can't read file" || "problems with path" ) {
            return {read_file("errors/404/index.html"), "html"};

        } else if (e == "no permission") {
            return {"500", "Internal Server Error: NO ACCESS"};
        }

        // 2-?
        println("Error processing request: " + string(e), ERROR_STYLE);
        return {"500", "Internal Server Error: " + string(e)};

    }
}

string make_server_msg(const pair<string, string>& answer_body) {

    string filetype = (answer_body.second != "png" ? "text" : "image");

    string serverMessage = "HTTP/1.1 200 OK\nContent-Type: " 
        + filetype
        + "/" 
        + answer_body.second 
        + "\nContent-Length: "
        + to_string(answer_body.first.size())
        + "\n\n"
        + answer_body.first;

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
        //answer_request(socket, make_500_response("Internal Server Error: " + string(e.what())));
    }

    closesocket(socket);
}

int main() {
    println("creating a server", USUAL_STYLE);

    SOCKET wsocket;
    WSADATA wsaData;
    int server_len;
    int BUFFER_SIZE = 30720;
    struct sockaddr_in server;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        println("can't initialize", ERROR_STYLE);
        return 1;
    }

    // Create TCP socket
    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (wsocket == INVALID_SOCKET) {
        println("can't create socket", ERROR_STYLE);
        WSACleanup();
        return 1;
    }

    // Bind socket to address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);
    server_len = sizeof(server);

    if (bind(wsocket, (SOCKADDR*)&server, server_len) != 0) {
        println("can't bind", ERROR_STYLE);
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    // Listen for connections
    if (listen(wsocket, 20) != 0) {
        println("can't listen", ERROR_STYLE);
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    println("server started", POSITIVE_STYLE);

    vector<future<void>> storage;

    while (true) {
        // Accept client request
        SOCKET new_wsocket = accept(wsocket, (SOCKADDR*)&server, &server_len);
        if (new_wsocket == INVALID_SOCKET) {
            println("can't accept", ERROR_STYLE);
            continue;
        }

        // Process request asynchronously
        storage.emplace_back(async(launch::async, process_request, new_wsocket, BUFFER_SIZE));
    }

    closesocket(wsocket);
    WSACleanup();
    return 0;
}
