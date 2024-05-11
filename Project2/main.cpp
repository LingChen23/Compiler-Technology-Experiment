//
// Created by 16672 on 2024/5/9.
//

#include <iostream>
#include <vector>
#include <stdexcept>
#include "token.h"

using namespace std;

class Parser {
    vector<Token> tokens;   //存储了从词法分析器得到的一系列的标记
    int currentToken;       //指向当前正在处理的标记
public:
    Parser(vector<Token> tokens) : tokens(tokens), currentToken(0) {}

    void parse() {
        program();
        if (currentToken != tokens.size()) {
            throw runtime_error("Extra tokens found");
        }
        cout << "success" << endl;
    }

private:
    Token peek() {
        return tokens[currentToken];
    }

    Token consume(TokenType type) {
        if (peek().type == type) {  //获取当前处理的标记 检查是否是应处理的标记
            return tokens[currentToken++];
        } else {
            throw runtime_error("Unexpected token: " + peek().value);
        }
    }

    void program() {
        consume(BEGIN); //读取（或消耗）输入流的一部分后，移动输入流的当前位置
        statement_string();
        consume(END);
        consume(HASH);
    }

    void statement_string() {   //一个或多个通过分号分隔的语句
        statement();
        while (peek().type == SEMICOLON) {
            consume(SEMICOLON);
            statement();
        }
    }

    void statement() {  //一个赋值语句
        if (peek().type == IF) {
            if_statement();
        } else {
            assignment_statement();
        }
    }

    void if_statement() {
        consume(IF);
        expression();
        consume(THEN);
        statement_string();
//        if (peek().type == ELSE) {
//            consume(ELSE);
//            statement_string();
//        }
        consume(END);
    }

    void assignment_statement() {
        consume(IDENTIFIER);    //消耗一个标识符
        consume(ASSIGN);        //消耗赋值运算符
        expression();           //消耗表达式
    }

    void expression() {
        arithmetic_expression();
        while (peek().type == LT || peek().type == GT || peek().type == LE || peek().type == GE || peek().type == EQUAL || peek().type == NE) {
            consume(peek().type);
            arithmetic_expression();
        }
    }

    void arithmetic_expression() {
        term();
        while (peek().type == PLUS || peek().type == MINUS) {
            consume(peek().type);
            term();
        }
    }


    void term() {
        factor();
        while (peek().type == MULTIPLY || peek().type == DIVIDE) {
            consume(peek().type);
            factor();
        }
    }

    void factor() {
        if (peek().type == IDENTIFIER || peek().type == NUM) {  //消耗标识符或数字
            consume(peek().type);
        } else if (peek().type == LPAREN) { //左括号，然后解析一个表达式，最后消耗一个右括号。
            consume(LPAREN);
            expression();
            consume(RPAREN);
        } else {
            throw runtime_error("Unexpected token in factor: " + peek().value);
        }
    }
};

int main() {
    vector<string> tests = {
            "begin a:=9; x:=2*3; b:=a+x end#",  // 基本的赋值和数学运算
            "begin x:=1; if x>0 then y:=2 end end#",
            "x:=a+b*c end#",
            "begin x:=1; y:=2; z:=x+y end#",
            "x:=5+3*2 end#",
            "x:=a+(b+c)*d end#",
            "begin x:=1; y:=2; z:=3; w:=x+y+z end#",
            "begin x:=1; y:=2; z:=(x+y*)3 end#",
            "x:=a end#"
    };


    for (const auto& test : tests) {
        auto tokens = lex(test);    //获取标记
        Parser parser(tokens);
        cout<<test<<endl;
        try {
            parser.parse();
        } catch (exception&) {
            cout << "error" << endl;
        }
    }

    return 0;
}
