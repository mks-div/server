#pragma once
#include <map>
#include <stdexcept>
#include <format>
#include "../include/Token.h"
#include "../include/Error.h"

Token::Token(string token, Type type, OperatorAssociativity asc) : type{type}, str{token} {
    // если токен - оператор, но ассоциативность не задана - ошибка
    if(type == OPERATOR && asc == NONE)
        throw string("Associativity required!");

    // если токен - НЕ оператор, но ассоциативность задана - ошибка
    else if(type != OPERATOR && asc != NONE)
        throw string("Non-operator token can't have an associativity!");

    opAsc = asc;
}

int Token::getPrecendance() const
{
    static map<string, int> op_leftassociative = 
    {
        {"+", 2},
        {"-", 2},
        {"/", 3},
        {"*", 3},
        {"^", 5}
    };


    static map<string, int> op_rightassociative = 
    {
        {"-", 4} // унарное отрицание
    };

    switch(opAsc)
    {
    case LEFT:
        if(op_leftassociative.contains(str)) return op_leftassociative[str];
        else throw string("Unknown Operator!");
        break;
    case RIGHT:
        if(op_rightassociative.contains(str)) return op_rightassociative[str];
        else throw string("Unknown Operator!");
        break;
    case NONE:
        throw logic_error(
          format("Token \"{}\" is not an operatator, impossible.", str)
        );
        break;
    }
    return -1;
}
