//
// Created by 16672 on 2024/5/9.
//

#ifndef PROJECT2_TOKEN_H
#define PROJECT2_TOKEN_H

#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <map>

using namespace std;

enum TokenType {
    HASH = 0, //#
    BEGIN = 1,
    IF = 2,
    THEN =3,
    WHILE = 4,
    DO = 5,
    END = 6,
    IDENTIFIER = 10,    //标识符类型
    NUM = 11,   //数字
    PLUS = 13,
    MINUS = 14,
    MULTIPLY = 15,
    DIVIDE = 16,
    COLON = 17,
    ASSIGN = 18,
    LT = 20,
    NE = 21,
    LE = 22,
    GT = 23,
    GE = 24,
    EQUAL = 25,
    SEMICOLON = 26,
    LPAREN = 27,
    RPAREN = 28,
    UNKNOWN = 99
};

struct Token {
    TokenType type;
    string value;
};

//运算符和种别码的映射
map<string, TokenType> tokenMap = {
        {"#", HASH},    //0
        {"begin", BEGIN},
        {"if", IF},
        {"then", THEN},
        {"while", WHILE},
        {"do", DO},
        {"end", END},
        {"+", PLUS},    //13
        {"-", MINUS},
        {"*", MULTIPLY},
        {"/", DIVIDE},
        {":", COLON},
        {":=", ASSIGN},
        {"<",LT},
        {"<>", NE},
        {"<=", LE},
        {">", GT},
        {">=", GE},
        {"=", EQUAL},
        {";", SEMICOLON},
        {"(", LPAREN},
        {")", RPAREN},
        {"#", HASH}
};

vector<Token> lex(string input) {
    vector<Token> tokens;
    string buffer;  //临时存储从 input 中读取的字符。
    for (int i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (isspace(c)) {   //检查空白字符（如空格、制表符、换行符等）
            continue;
        }

        buffer.push_back(c);    //字符串的末尾添加一个字符
        //检查是否是标识符或界符
        if (c == ':') {
            if (i + 1 < input.size() && input[i + 1] == '=') {
                buffer.push_back('=');
                ++i;
            }
        } else if (c == '<') {
            if (i + 1 < input.size() && (input[i + 1] == '=' || input[i + 1] == '>')) {
                buffer.push_back(input[i + 1]);
                ++i;
            }
        } else if (c == '>') {
            if (i + 1 < input.size() && input[i + 1] == '=') {
                buffer.push_back('=');
                ++i;
            }
        }

        //在map查找到元素 是否是特殊标识符或边界符
        if (tokenMap.find(buffer) != tokenMap.end()) {
            tokens.push_back({tokenMap[buffer], buffer});
            buffer.clear();
        }
            //buffer 的第一个字符是否是字母
        else if (isalpha(buffer[0])) { // identifier
            //检查是否不是字母或数字
            if (i + 1 == input.size() || !isalnum(input[i + 1])) {
                tokens.push_back({IDENTIFIER, buffer});
                buffer.clear();
            }
        }
        else if (isdigit(buffer[0])) { // number
            //检查是否不是数字
            if (i + 1 == input.size() || !isdigit(input[i + 1])) {
                tokens.push_back({NUM, buffer});
                buffer.clear();
            }
        }
            //不是运算符和界符，不是标识符和数字
        else {
            tokens.push_back({UNKNOWN, buffer});
            buffer.clear();
        }
    }

    return tokens;
}

#endif //PROJECT2_TOKEN_H
