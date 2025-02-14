#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <stack>
#include "ast.h"
#include "parser.h"

using namespace std;
using namespace ast_types;


Program Parser::produceAST(string source){
    vector<Token>* pTokens = Token::Tokenize(source);
    tokens = std::move(*pTokens);
    // cout << "tokens size : " << (tokens).size()<< endl;
    Program program = Program();
    // cout << "tokens generated "<<endl;
    first_pass();
    while(!isEOF()){
        program.body.push_back(parse_statement());
    }

    return program;
}

bool Parser::isEOF(){
    // cout << "is eof checked "<< tokens[0].value << endl;
    // cout << "first token : "<< Token::toString(tokens[0].type) << " value : " << tokens[0].value<<endl;
    Token x = tokens[0];
    if(x.type == TokenType::End_of_file){
        return true;
    }else{
        return false;
    }
}

Token Parser::at(){
    return tokens[0];
}

Token Parser::eat(){
    Token x = at();
    tokens.erase(tokens.begin());
    return x; {
      
    }

}

Token Parser::eat(TokenType tk,string error_msg = ""){
    Token x = at();
    tokens.erase(tokens.begin());
    if(x.type != tk){ 
        if(error_msg == "") throw runtime_error("expected '" + Token::toString(tk) + "'but got '" + x.value +"'");
        throw runtime_error(error_msg);
    }
    return x;
}


Token Parser::at(int i){
  return tokens[i];
}

void Parser::first_pass(){
  stack<string> stackVar = stack<string>();
  Token current;
  int len = tokens.size();
  int index = 0;
  while(index < len){
      current = at(index);
      string value = current.value;
      if((value == "{" || value == "[") || value == "("){
          stackVar.push(value);
      }
      else if((value == "}" && stackVar.top()=="{" || value == "]" && stackVar.top() == "[") || value == ")" && stackVar.top() == "("){
          stackVar.pop(); 
      }

      else if(((value == "}" || value == "]") || value == ")") && stackVar.empty()){
              throw runtime_error("invalid token 1 '" + value +"'");
      }
      else if((value == "}" || value == "]") || value == ")"){
          if((value == "}" && stackVar.top() != "{" || value == "]" && stackVar.top() != "[") || value == ")" && stackVar.top() != "("){
              throw runtime_error("invalid token 2 found '" + value +"' but needed "+stackVar.top());
          }
      }  index++;
  }
}

Statement*  Parser::parse_statement(){
    // cout << "statement parsed" << endl;
    TokenType x = at().type;
    if(x == TokenType::KeyWords){
        Token kw = eat();
        if(kw.value == "var"){
            return parse_variable_declaration();
        }else if(kw.value == "if"){
            return parse_if_statement();
        }else if(kw.value == "while"){
            return parse_while_loop();
        }else if(kw.value == "break"){
            return parse_break_keyword();  
        }
        else{
            throw std::runtime_error("Keyword hasnt been implemented");
        }
    }else if(x == TokenType::Identifiers){
        if(tokens[1].type == TokenType::Equals)return parse_variable_assignment();
    }
    return parse_expression();
}

Statement* Parser::parse_variable_declaration(){
    Token identifier = eat(TokenType::Identifiers,"dei problem");
    eat(TokenType::Colon);
    Token variable_data_type = eat(TokenType::DataType);
    bool isNullable = false;
    if(at().type == TokenType::QuestionMark){
        eat();
        isNullable = true;
    }
    Token equals = eat(TokenType::Equals);
    Expression* right_expression = parse_expression(); // expression followed by the equals sign
    eat(TokenType::SemiColon);

    Identifier* identifier_expr = new Identifier(identifier.value);
    DataTypes data_type;
    if(variable_data_type.value == "Number"){
        data_type = DataTypes::Number;
    }else if(variable_data_type.value == "Boolean"){
        data_type = DataTypes::Bool;
    }else if(variable_data_type.value == "String"){
        data_type = DataTypes::String;
    }else{
        throw runtime_error("unknown datatype encountered");
    }
    // return new Variable_Declaration(); 
    return new Variable_Declaration(identifier_expr,right_expression,data_type,isNullable);
}

Statement* Parser::parse_variable_assignment(){
    Token identifier = eat(TokenType::Identifiers);
    Token equals = eat(TokenType::Equals);
    Expression* right_expression = parse_expression(); // expression followed by the equals sign
    eat(TokenType::SemiColon,"parse variable assignment");

    Identifier* identifier_expr = new Identifier(identifier.value);
    DataTypes data_type;
    // return new Variable_Declaration(); 
    return new Variable_Assignment(identifier_expr,right_expression,data_type);
}

Statement* Parser::parse_if_statement(bool isElse){
  vector<Statement*> body_if = vector<Statement*>();
  Expression* condition_expr = nullptr;
  if(!isElse){
      eat(TokenType::Open_paren);
      condition_expr = parse_expression();
      eat(TokenType::Close_paren);
  }
  eat(TokenType::OpenBrace);
  Statement* elseBody = nullptr;
  while(true){
      Statement* current_expr = parse_statement();
      body_if.push_back(current_expr);
      if(at().type == TokenType::CloseBrace){
      eat();
      break;
    }
  }
  if(!isElse && at().value == "else"){
      if(tokens[1].value == "if"){
          eat();
          eat();
          cout << "hello" << endl;
          elseBody = parse_if_statement(false);
      }else if (tokens[1].value == "{"){
          eat();
          elseBody = parse_if_statement(true);
      }
  }
  Statement* if_Node = new If_Statement(condition_expr,body_if,elseBody);
  return if_Node; 
}

Statement* Parser::parse_while_loop(){
    eat(TokenType::Open_paren);
    Expression* condition = parse_expression();
    eat(TokenType::Close_paren);
    eat(TokenType::OpenBrace);
    vector<Statement*> body = vector<Statement*>();

    while(true){
        Statement* current_expr = parse_statement();
        body.push_back(current_expr);
        if(at().type == TokenType::CloseBrace){
          eat();
          break;
        }
    }
    
    return new While_Loop(condition,body);

}

Statement* Parser::parse_for_loop(){
  Statement* indexing_variable_expression;
  Expression* condition_expr;
  Statement* increment_expr;

  eat(TokenType::Open_paren);
  if(at().value == "var") indexing_variable_expression = parse_variable_assignment();
  else if(tokens[1].type == TokenType::Equals) indexing_variable_expression = parse_variable_assignment();
  else if(at().type == TokenType::Identifiers && tokens[1].type == TokenType::SemiColon) {
    indexing_variable_expression = new Identifier(eat().value);
    eat(TokenType::SemiColon);
  }else if(at().type == TokenType::SemiColon) indexing_variable_expression = nullptr; 
  else throw runtime_error("Parse for Loop cannot identify the indexing variable");

  if(at().type == TokenType::SemiColon) condition_expr = nullptr;
  else condition_expr = parse_expression();

  if(at().type == TokenType::Close_paren) increment_expr = nullptr;
  else increment_expr = parse_statement();

  bool isValidIncrement = true;
  NodeType expr_type = increment_expr->type;



}


Statement* Parser::parse_break_keyword(){
  eat(TokenType::SemiColon);
  return new Break_keyWord();
}  


// -----------------------------------------------------------------
// expression

Expression* Parser::parse_expression(){
    // cout << "expr parsed" << endl;
    return this->parse_logical_expression();
}

Expression* Parser::parse_logical_expression(){
  Expression* left = parse_conditional_expression();
  while(at().value=="&&" || at().value=="||"){
    string logical_operator = eat().value;
    Expression* right = parse_conditional_expression();
    left = new LogicalExpression(left,right,logical_operator);
  }
  return left;
}

Expression* Parser::parse_conditional_expression(){
    Expression* left = parse_additive_expression();
    while(at().value=="<"||at().value=="<="||at().value==">"||at().value==">="||at().value=="=="){
        string logical_operator = eat().value;
        Expression* right = parse_additive_expression();
        left = new ConditionalExpression(left,right,logical_operator);
    }
    return left;
}

Expression* Parser::parse_multiplicative_expression(){
    Expression* left = parse_primary_expression();
    BinaryExpression* binary_expression;
    while(at().value == "/" || at().value == "*" || at().value == "%"){
        string binary_operator = eat().value;
        Expression* right = parse_primary_expression();
        left = new BinaryExpression(left,right,binary_operator);
    }
    return left;
}

Expression* Parser::parse_additive_expression(){
    Expression* left = parse_multiplicative_expression();
    BinaryExpression* binary_expression;
    while(at().value == "+" || at().value == "-"){
        string binary_operator = eat().value;
        Expression* right = parse_multiplicative_expression();
        left = new BinaryExpression(left,right,binary_operator);
    }
    return left;
}

Expression* Parser::parse_primary_expression(){
    TokenType current = at().type;
    Expression* expr;
    // cout << "primary expr parsed" << endl;
    switch (current)
    {
    case TokenType::Identifiers :
        return new Identifier(eat().value);
    case TokenType::Number:
        return new NumericLiteral(eat().value);
    case TokenType::Boolean:
        return new BooleanLiteral(eat().value);
    case TokenType::String:
        return new StringLiteral(eat().value);
    case TokenType::Open_paren:
        eat();
        expr = parse_expression();
        eat(TokenType::Close_paren);
        return expr;
    
    default:
        throw std::runtime_error("unknown token reached during parsing" + at().value);

    }
} 
