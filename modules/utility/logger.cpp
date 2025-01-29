#pragma once

#include <iostream>
#include <windows.h>
#include "utility.h"




using namespace std;
const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

const WORD STANDART_STYLE = 15;
const WORD ERROR_STYLE = 12; 
const WORD WARNING_STYLE = 14; 
const WORD INFO_STYLE = 11; 
const WORD POSITIVE_STYLE = 10; 
const WORD STRANGE_STYLE = 13; 
const WORD USUAL_STYLE = 9; 

WORD make_style(const int& foreground, const int& background=0) { return ((background << 4) | foreground); }


void print(const string& text, const WORD& settings=STANDART_STYLE) {
    cout << text;
}
/*
void print(const string& text, const WORD& settings=STANDART_STYLE) {

    if (settings != STANDART_STYLE) 
        SetConsoleTextAttribute(h, settings);

    for (const char& letter: text) {
        if (letter == '\n') {
            SetConsoleTextAttribute(h, make_style(0, 0));
            cout << '\31' << endl; // cus 31 is not shown by console but fixes a bug
            SetConsoleTextAttribute(h, settings);
            continue; 
        } cout << letter; 

    } SetConsoleTextAttribute(h, STANDART_STYLE);
    cout << '\31';
    
}*/

void println(const string& text, const WORD& settings=STANDART_STYLE) {
    print(text + '\n', settings);
}

/*
int main() {
    print(("hello_world and something else\n"), make_style(9));
    print(("hello_world and something else\n"), make_style(10));
    print(("hello_world and something else\n"), make_style(11));
    print(("hello_world and something else\n"), make_style(12));
    print(("hello_world and something else\n"), make_style(13));
    print(("hello_world and something else\n"), make_style(14));
    print(("hello_world and something else\n"));

    cout << make_style(9) << '|' 
        << make_style(10) << '|'
        << make_style(11) << '|'
        << make_style(12) << '|'
        << make_style(13) << '|'
        << make_style(14) << '|'
        << make_style(15) << '|' 
    << endl;
    cin.get();
    return 0;
}*/

