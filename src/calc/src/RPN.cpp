
#include <iostream>
#include <stack>
#include <cmath>
// #include <tuple>getPrecendance
#include <format>
#include "../include/RPN.h"


using namespace std;

void shuntingYard(const vector<Token> &expr, vector<Token> &outQueue)
{
    stack<Token> stack;
    auto fromStackToQueue = [&]() { outQueue.push_back(stack.top()); stack.pop(); };
    for(const auto& token : expr)
    {
        switch(token.getType())
        {
        case Token::INT_LITERAL:
        case Token::FLOAT_LITERAL:
            outQueue.push_back(token);
            break;
        case Token::L_PARANTHESIS:
        case Token::FUNCTION:
            stack.push(token);
            break;
        case Token::OPERATOR:
            if(!stack.empty())
            {
                while(stack.top().getType() == Token::OPERATOR && ((stack.top().getPrecendance() > token.getPrecendance())
                        || (stack.top().getPrecendance() == token.getPrecendance() && token.getAsc() == Token::LEFT)))
                {
                    fromStackToQueue();
                    if(stack.empty()) 
                        break;
                }
            }
            stack.push(token);
            break;

        case Token::R_PARANTHESIS:
            if(stack.empty())
                throw Error("Non-balanced on paranthesis expression!", Error::Syntax);
            while (stack.top().getType() != Token::L_PARANTHESIS)
            {
                fromStackToQueue();
                if (stack.empty())
                    throw Error("Non-balanced on paranthesis expression!", Error::Syntax);
            }
            stack.pop();
            if(!stack.empty() && stack.top().getType() == Token::FUNCTION)
                fromStackToQueue();
            break;

        case Token::SEPARATOR:
            if(stack.empty())
                throw Error("Paranthesis or separator missed!", Error::Syntax);
            while(stack.top().getType() != Token::L_PARANTHESIS)
            {
                fromStackToQueue();
                if(stack.empty())
                    throw Error("Paranthesis-unbalanced expression!", Error::Syntax);
            }
            break;
        }
    }
    while(!stack.empty())
    {
        if(stack.top().getType() == Token::L_PARANTHESIS)
            throw Error("Paranthesis-unbalanced expression!", Error::Syntax);
        else
            fromStackToQueue();
    } 
}

double countRPN(const vector<Token> &expr)
{
    stack<double> stack;
    auto getOneToken = [&]() 
    {
        if(stack.empty()) throw Error("Not enough arguments in function!", Error::Syntax);
        double x = stack.top(); 
        stack.pop(); 
        return x; 
    };
    auto getTwoTokens = [&]()
        { double x = getOneToken(), y = getOneToken(); return tuple{y,x}; };
    auto checkedDivision = [&](double a, double b)
        { if(b == 0.f) throw Error("Division by zero", Error::Math); return a / b; };

    double res;
    for (auto &token : expr)
    {
        const string &str = token.getStr();
        switch(token.getType())
        {
        case Token::INT_LITERAL:
            stack.push(stof(str));
            break;
        case Token::FLOAT_LITERAL:
            stack.push(stof(str));
            break;
        case Token::OPERATOR:
            switch(token.getAsc())
            {
            case Token::LEFT:
            {
                auto [a,b] = getTwoTokens(); 
                if      (str == "+") res = a + b;
                else if (str == "-") res = a - b;
                else if (str == "*") res = a * b;
                else if (str == "/") res = checkedDivision(a, b);
                else if (str == "^") res = pow(a,b);
                else    throw Error("Unknown operator!", Error::Syntax);
                break;
            }
            case Token::RIGHT:
            {
                auto a = getOneToken();
                if   (str == "-") res = -a;
                else throw Error("Unknown operator!", Error::Syntax);
                break;
            }
            case Token::NONE:
                throw logic_error("Operator must have associativity!");
                break;
            }
            stack.push(res);
            break;
        case Token::FUNCTION:
            if(str == "log") 
            {
                auto [a,b] = getTwoTokens();
                if(a <= 0.f || a == 1.0f) throw Error(format("log(a,x): not defined for a = {}", a), Error::Math);
                if(b <= 0.f) throw Error("log(a,x): out of function's domain", Error::Math);
                res = log(b) / log(a);
            }
            else if (str == "log2")
            {
                auto a = getOneToken();
                if(a <= 0.f) throw Error("log2(x): out of function's domain", Error::Math);
                res = log2(a);
            }
            else if(str == "ln")
            {
                auto a = getOneToken();
                if(a <= 0.f) throw Error("ln(x): out of function's domain", Error::Math);
                res = log(a);
            }
            else if(str == "lg")
            {
                auto a = getOneToken();
                if(a <= 0.f) throw Error("lg(x): out of function's domain", Error::Math);
                res = log10(a);
            }
            else if(str == "max")
            {
                auto[a,b] = getTwoTokens();
                res = a > b ? a : b;
            }
            else if(str == "min")
            {
                auto[a,b] = getTwoTokens();
                res = a < b ? a : b;
            }
            else if(str == "sqrt")
            {
                auto a = getOneToken();
                res = sqrt(a);
            }
            else if(str == "sin")
            {
                auto a = getOneToken();
                res = sin(a);
            }
            else if(str == "cos")
            {
                auto a = getOneToken();
                res = cos(a);
            }
            else if (str == "tg")
            {
                auto a = getOneToken();
                res = tan(a);
            }
            else if (str == "ctg")
            {
                auto a = getOneToken();
                res = 1 / tan(a);
            }
            else
                throw Error("Unknown function!", Error::Syntax);
            stack.push(res);
            break;
        default:
            break;
        } 
    }
    return stack.top();
}
