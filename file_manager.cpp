
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include"logger.cpp"

using namespace std;


bool give_access(const string& request) {
    return true;
}

string get_path(const string& request) {
    string path = {};

    long long unsigned int idx = 5;

    while (request[idx] != ' ' && idx < request.size()) { 
        path += request[idx];
        idx ++;
    }

    if (!path.size()) {
        return "index.html";
    } 

    return path;
}

string get_file_extension(const string& path) {
    string file_extension = {};
    unsigned int idx = path.size() - 1;

    while (path[idx] != '.' && idx >= 0) {
        file_extension = path[idx] + file_extension;
        idx --;
    } 
    return file_extension;
}


// text reader
string read_file(const string& path) { 
    ifstream file;
    stringstream buf;

    file.open("files/web/" + path);

    if (file.fail()){
        println("can't read text file", ERROR_STYLE);
        throw string{"can't read file"}; 
    } 
        
    if (file.is_open()) 
        buf << file.rdbuf();  
    
    file.close();

    return buf.str();
}


// image reader
string read_image(const string& path) {

    ifstream stream("files/web/" + path, ios::in | ios::binary);

    if (stream.fail()) {
        println("can't read image file", ERROR_STYLE);
        throw string("can't read file");
    }

    vector<char> contents((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());


    return string(contents.begin(), contents.end());
}

pair<string, string> get_file(const string& request) { 
    // for writing server message
    
    if (!give_access(request)) {
        println("no permission", ERROR_STYLE);
        throw string{"no permission"};
    } print(request);

    string path_to_file = get_path(request);
    string file_extension = get_file_extension(path_to_file);


    try {
        if (file_extension != "png") // need to check with map or set
            return {read_file(path_to_file), file_extension};
        else
            return {read_image(path_to_file), file_extension};
        
    } catch (string error_message) {
        throw error_message;
    }

    throw string {"unknown error occured"};
}

