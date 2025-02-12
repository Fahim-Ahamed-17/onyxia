#include <iostream>
#include <cstdlib>
#include <fstream>
#include "ast.h"
#include "parser.h"
#include "../runtime/values.h"
#include "../runtime/interpreter_part.h"
#include "../runtime/environment.h"
#define MK_NUMBER(i) (new values::NumberValue(i))

using namespace std;

// void printAST(ast_types::Program program){
//     vector<ast_types::Statement> statements = program.body;
//     for(int i = 0;i < statements.size();i++){
//     }
// }

int repl();
int fullVersion();


int main (int argc, char *argv[]) {
  int outputCode = fullVersion();
  return 0;
}


int fullVersion() {
  ifstream source_file("hello.nx");
  string text;
  string source_code ="";
  values::RuntimeValue* result;
  Parser parser = Parser();
  ast_types::Program program ;
  environment::Environment* env = new environment::Environment();
    while (true)
    {
        std::getline(source_file,text);
        if(source_file.eof()){
          break;
        }
        source_code.append(text);
    }  
  program = parser.produceAST(source_code);
  for(int i = 0;i<program.body.size();i++){
    program.body[i]->printNode(1);
  }
  result = (&program)->evaluate_node(env);
  result->printValue();
  source_file.close();
  return 0;
}


int repl(){
    string text;
    Parser parser = Parser();
    ast_types::Program program ;
    environment::Environment* env = new environment::Environment();
    system("clear");
    cout << "Onyxia Interpreter " <<endl;
    cout << "Repl v1.0" << endl;
    while (true)
    {
        cout << ">";
        std::getline(std::cin,text);
        if(text == "exit") {
            system("clear");
            break;
        }
        parser.first_pass();
        program = parser.produceAST(text);
        program.printNode(0);
        values::RuntimeValue* result = (&program)->evaluate_node(env);
        result->printValue();
    }
    
  
    return 0;
}

//g++ frontend\test.cpp frontend\parser.cpp frontend\lexer.cpp frontend\ast.cpp runtime\interpreter_part.cpp runtime\values.cpp

//   g++ frontend/test.cpp frontend/parser.cpp frontend/lexer.cpp frontend/ast.cpp runtime/interpreter_part.cpp runtime/values.cpp runtime/environment.cpp  
