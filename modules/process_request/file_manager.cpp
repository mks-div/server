
#include <iostream>
#include <vector>

#include <fstream>
#include <sstream>

#include "C:/projects/server/modules/utility/utility.h"
#include "C:/projects/server/modules/utility/logger.cpp"

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

    if (path.empty()) {
        throw string{"no path"};
    }

    string file_extension = {};
    long long unsigned int idx = (!path.size() ? 0 : path.size() - 1); 

    while (path[idx] != '.') {  
        
        file_extension = path[idx] + file_extension;
        
        if (!idx) { // it's better not to write it in while conditions
            throw string {"incorrect path"};
        }
        
        idx --;
    } 
    return file_extension;
}


// text reader
string read_file(const string& path) { 
    ifstream file;
    stringstream buf;
    cout << "path:" << path << endl;
    file.open("C:/projects/server/files/web/" + path);

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

    ifstream stream("C:/projects/server/files/web/" + path, ios::in | ios::binary);

    if (stream.fail()) {
        println("can't read image file", ERROR_STYLE);
        throw string("can't read file");
    }

    vector<char> contents((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());


    return string(contents.begin(), contents.end());
}


answer_format get_file(const string& request) { 
    // for writing server message

    if (!give_access(request)) {
        println("no permission", ERROR_STYLE);
        throw string{"no permission"};
    } cout << request;

    string path_to_file = {};
    string file_extension = {};


    try {
        path_to_file = get_path(request);
        file_extension = get_file_extension(path_to_file);
    } catch(string error_message) {
        if (
            error_message == "no path" ||
            error_message == "incorrect path"
        ) {
            throw string {"problems with path"};
        }
    }

    // read file
    try {
        if (file_extension != "png") // need to check with map or set
            return answer_format(read_file(path_to_file), file_extension, "GET");
        else
            return answer_format(read_image(path_to_file), file_extension, "GET");
        
    } catch (string error_message) {
        throw error_message;
    }

    throw string {"unknown error occured"};
}