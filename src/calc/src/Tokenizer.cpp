#include "../include/Tokenizer.h"
#include <iostream>
#include <format>
#include <map>
#include <string>
#include "../include/Error.h"
#include "Token.cpp"

using namespace std;
void tokenize(const string &expr, vector<Token> &tokens)
{
    State state = S0;

    string validOperators = "+-*^/";

    bool isDigit, isLetter, isOp, isParanth, isPoint, isSep, isLParanth, isRParanth;

    string buffer;
    Token::Type bufferTokenType = Token::INT_LITERAL;

    for(auto& s : expr) {
        // Определяем тип символа
        isDigit = isdigit(s);
        isLetter = isalpha(s);
        isLParanth = s == '(';
        isRParanth = s == ')';
        isParanth = isLParanth || isRParanth;
        isPoint = s == '.';
        isSep = s == ',';
        isOp = validOperators.find(s) != validOperators.npos;

        // Если тип символа неопределен, значит ошибка в синтаксисе
        if(!(isDigit || isLetter || isParanth || isPoint || isSep || isOp))
            throw string(format("Unknown symbol: {}", s));

        // Смена состояния
        switch(state)
        {
        case S0:
            if (isOp || isParanth)
                state = S1;
            else if (isDigit)
                state = S2;
            else if (isLetter)
                state = S4;
            else if (isPoint || isSep)
                throw string(format("Unexpected symbol: \"{}\"", s));
            break;
        case S1:
            if (isDigit)
                state = S2;
            else if (isLetter)
                state = S4;
            else if (isPoint || isSep)
                throw string(format("Unexpected symbol: \"{}\"", s));
            break;
        case S2:
            bufferTokenType = Token::INT_LITERAL;
            if (isPoint)
                state = S3;
            else if (isParanth || isOp || isSep)
                state = S5;
            else if (isLetter)
                throw string(format("Unexpected symbol: \"{}\"", s));
            break;
        case S3:
            bufferTokenType = Token::FLOAT_LITERAL;
            if (isParanth || isOp || isSep)
                state = S5;
            else if (isPoint)
                throw string(format("Unexpected symbol: \"{}\"", s));
            break;
        case S4:
            bufferTokenType = Token::FUNCTION;
            if(isLParanth)
                state = S5;
            else if(isOp || isRParanth || isSep)
                throw string(format("Unexpected symbol \"{}\"", s));
            break;
        case S5:
            if (isParanth || isOp)
                state = S1;
            else if (isDigit)
                state = S2;
            else if (isLetter)
                state = S4;
            else if (isPoint || isSep)
                throw string(format("Unexpected symbol: \"{}\"", s));
            break;
        default:
            break;
        }

        auto tokenize_Op_Paranth_Sep = [&]() 
        {
            if(isOp)
            {
                // обработка unary negation
                if(tokens.size() == 0 || tokens[tokens.size()-1].getType() == Token::L_PARANTHESIS)
                    tokens.push_back({string{s}, Token::OPERATOR, Token::RIGHT});
                else
                    tokens.push_back({string{s}, Token::OPERATOR, Token::LEFT});
            }
            else if(isParanth)
            {
                tokens.push_back({string{s}, isRParanth ? Token::R_PARANTHESIS : Token::L_PARANTHESIS});
            }
            else if(isSep)
            {
                tokens.push_back({string{s}, Token::SEPARATOR});
            }
        };

        // Действия
        switch(state)
        {
        case S1:
            tokenize_Op_Paranth_Sep();
            break;
        case S2: case S3: case S4:
            buffer.push_back(s);
            break;
        case S5:
            tokens.push_back({buffer, bufferTokenType});
            buffer.clear();
            tokenize_Op_Paranth_Sep();
            break;
        }   
    }
    if(!buffer.empty())
        tokens.push_back({buffer, bufferTokenType});
}

