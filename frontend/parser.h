#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include <iostream>
#include <vector>
#include "lexer.cpp"

class Parser{
    private :
        vector<Token> tokens;
    public :
        ast_types::Program produceAST(string source); 
        bool isEOF();
        Token at();
        Token at(int i);
        Token eat();
        Token eat(string expected,string error);
        Token eat(TokenType tk,string error);
        void first_pass();
        ast_types::Statement* parse_statement();
        ast_types::Statement* parse_variable_declaration();
        ast_types::Statement* parse_variable_assignment();
        ast_types::Statement* parse_if_statement(bool isElse = false);
        ast_types::Statement* parse_while_loop();
        ast_types::Statement* parse_break_keyword(); 

        ast_types::Expression* parse_expression();
        ast_types::Expression* parse_primary_expression();
        ast_types::Expression* parse_additive_expression();
        ast_types::Expression* parse_multiplicative_expression();
        ast_types::Expression* parse_logical_expression();        
};

#endif 
