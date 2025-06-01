
#include <iostream>
#include "../include/Tokenizer.h"
#include "../include/RPN.h"
#include "RPN.cpp"
#include "Token.cpp"
#include "Tokenizer.cpp"

using namespace std;

// fxgd
double Calculate(const string& expr="0") {
    vector<Token> tokensInfix, tokensRPN;
    
    try {
        if (!expr.empty()) {
            tokenize(expr, tokensInfix);
            for(auto& i : tokensInfix) {
                string type, asc;
                switch(i.getType()) {
                    case Token::OPERATOR:
                        type = "OPERATOR";
                        break;
                    case Token::L_PARANTHESIS:
                        type = "L_PARANTHESIS";
                        break;
                    case Token::R_PARANTHESIS:
                        type = "R_PARANTHESIS";
                        break;
                    case Token::INT_LITERAL:
                        type = "INT_LITERAL";
                        break;
                    case Token::FLOAT_LITERAL:
                        type = "FLOAT_LITERAL";
                        break;
                    case Token::FUNCTION:
                        type = "FUNCTION";
                        break;
                    case Token::SEPARATOR:
                        type = "SEPARATOR";
                        break;
                }

                switch(i.getAsc()) {
                    case Token::NONE:
                        asc = "NONE";
                        break;
                    case Token::RIGHT:
                        asc = "RIGHT";
                        break;
                    case Token::LEFT:
                        asc = "LEFT";
                        break;
                }
            }
        }
        shuntingYard(tokensInfix, tokensRPN);
        return countRPN(tokensRPN);
    } catch(...) {
        throw string("");
    }

    return 0.0;
}


// int main() {
//     vector<Token> tokensInfix, tokensRPN;
//     string expr = "";
    


//     while (true) {
//         cin >> expr;
//         if (expr == "") {
//             break;
//         }
//         cout << "Answer" << Calculate(expr) << endl;
//     }
//     return 0;
// }