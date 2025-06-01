#pragma once
#include <iostream>
#include <unordered_set>
#include "../db/db.h"
#include "variables.h"
using namespace std;


struct answer_format {
    string answer_data;
    string answer_type;
    string request_type;
    vector<pair<string, string>> additional_info;

    answer_format(string ans_data, string ans_type="text", string req_type="GET", vector<pair<string, string>> add_info={}) 
        : answer_data(ans_data), answer_type(ans_type), request_type(req_type), additional_info(add_info) {}
};


