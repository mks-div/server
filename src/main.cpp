#include <iostream>
#include <future>

#include <vector>

#include <fstream>
#include <sstream>

#include <winsock2.h>
#include <Ws2tcpip.h>

#include "utility/utility.h"
#include "utility/logger.cpp"
#include "process_request/process_request.cpp"

#include <typeinfo>

using namespace std;
// notes:
// 2 solve getting file security issues


int main() {
    println("creating a server", USUAL_STYLE);
    SOCKET wsocket;
    WSADATA wsaData;
    int server_len;
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

    // it may be possible to apply values to server in one stroke
    // Bind socket to address 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_values::IP_ADDRESS);
    server.sin_port = htons(server_values::PORT);
    server_len = sizeof(server);
    println("1");
    //cout << type_info(socket_answ);
    if (bind(wsocket, (SOCKADDR*)&server, server_len) == SOCKET_ERROR) {
        println("can't bind: " + to_string(WSAGetLastError()), ERROR_STYLE);
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }
    println("2");
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
        storage.emplace_back(async(launch::async, process_request, new_wsocket, server_values::BUFFER_SIZE));
    }

    closesocket(wsocket);
    WSACleanup();
    return 0;
}
