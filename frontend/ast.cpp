#include <iostream>
#include <stdexcept>
#include <system_error>
#include <vector>
#include <iomanip>
#include "ast.h"
#include "../runtime/interpreter_part.h"

using namespace std;

namespace ast_types{
    

  string toString(NodeType a){
    switch (a){
      case NodeType::Program:
        return "Program";
      case NodeType::Break_Interrupt:
        return "Break Statement";
      default:
        return "Invalid";
    }
  }

  void Statement::setNodeType(NodeType NodeKind){type  = NodeKind;}
  NodeType Statement::getNodeType(){return type;}


  Program::Program(){
    type = NodeType::Program;
  }

  void Program::printNode(int i = 0) {
    cout << "Program : " << endl;
    for(int i = 0;i < body.size();i++){
      body[i]->printNode(i+1);
    }   
  }  

  values::RuntimeValue* Program::evaluate_node(environment::Environment* env){
    return interpreter_functions::evaluate_program(this,env);
  }

  BinaryExpression::BinaryExpression(
    Expression* left_exp,
    Expression* right_exp,
    string bin_op
  ){
    left = left_exp;
    right = right_exp;
    binary_operator = bin_op;
    setNodeType(NodeType::Binary_Expression);
  }

  void BinaryExpression::printNode(int i){
    cout << "Binary Expression : " <<endl;
    Statement::indent(i);
    cout << "Left  : " ;left->printNode(i+1);
    Statement::indent(i);
    cout << "Right : " ;right->printNode(i+1);
    Statement::indent(i);
    cout << "operator : " << binary_operator << endl; 
    }

  values::RuntimeValue* BinaryExpression::evaluate_node(environment::Environment* env){
    return interpreter_functions::eval_binary_expr(this, env);
  }
        
  ConditionalExpression::ConditionalExpression(
    Expression* left_exp,
    Expression* right_exp,
    string conditional_op
  ){
    left = left_exp;
    right = right_exp;
    conditional_operator = conditional_op;
    setNodeType(NodeType::Conditional_Expression);
    }

  void ConditionalExpression::printNode(int i){
    cout << "Conditional Expression : " <<endl;
    Statement::indent(i);
    cout << "Left  : " ;left->printNode(i+1);
    Statement::indent(i);
    cout << "Right : " ;right->printNode(i+1);
    Statement::indent(i);
    cout << "operator : " << conditional_operator << endl; 
  }

  values::RuntimeValue* ConditionalExpression::evaluate_node(environment::Environment* env){
    return interpreter_functions::eval_conditional_expr(this,env);
  }


  LogicalExpression::LogicalExpression(
    Expression* left_exp,
    Expression* right_exp,
    string logical_op
  ){
    left = left_exp;
    right = right_exp;
    logical_operator = logical_op;
    setNodeType(NodeType::Logical_Expression);
    }

  void LogicalExpression::printNode(int i){
    cout << "Logical Expression : " <<endl;
    Statement::indent(i);
    cout << "Left  : " ;left->printNode(i+1);
    Statement::indent(i);
    cout << "Right : " ;right->printNode(i+1);
    Statement::indent(i);
    cout << "operator : " << logical_operator << endl; 
  }

  values::RuntimeValue* LogicalExpression::evaluate_node(environment::Environment* env){
    return interpreter_functions::eval_logical_expr(this, env);
  }
  NumericLiteral::NumericLiteral(string num){
    value = stoi(num);
    setNodeType(NodeType::Logical_Expression);
  }

  void NumericLiteral::printNode(int i) {
    cout << "NumericLiteral : " << endl;
    Statement::indent(i);
    cout << "value : " << fixed << setprecision(3) << value << endl;
  }

  values::RuntimeValue* NumericLiteral::evaluate_node(environment::Environment* env){
    ast_types::NumericLiteral* numeric_literal_ptr= dynamic_cast<NumericLiteral*>(this);
    int int_value = numeric_literal_ptr->value;
    return new values::NumberValue(int_value);
  }

  BooleanLiteral::BooleanLiteral(string data){
    if(data == "true") value = true;
    else value = false;
    setNodeType(NodeType::BooleanLiteral);
  }

  void BooleanLiteral::printNode(int i) {
    cout << "BooleanLiteral : " << endl;
    Statement::indent(i);
    cout << "flag : " << value << endl;
  }

  values::RuntimeValue* BooleanLiteral::evaluate_node(environment::Environment* env){
    BooleanLiteral* boolean_literal_ptr= dynamic_cast<BooleanLiteral*>(this);
    bool bool_value = boolean_literal_ptr->value;
    return new values::BooleanValue(bool_value);
  }
    
  StringLiteral::StringLiteral(string word){
    value = word;
    setNodeType(NodeType::StringLiteral);
  }

  void StringLiteral::printNode(int i) {
    cout << "StringLiteral : " << endl;
    Statement::indent(i);
    cout << "value : " << fixed << setprecision(3) << value << endl;
  }

  values::RuntimeValue* StringLiteral::evaluate_node(environment::Environment* env){
    StringLiteral* string_literal_ptr= dynamic_cast<StringLiteral*>(this);
    string string_value = string_literal_ptr->value;
    return new values::StringValue(string_value);
  }
    
  Identifier::Identifier(string value){
    symbol = value;
    setNodeType(NodeType::Identifier);
  }

  void Identifier::printNode(int i) {
    cout << "Identifier : " << endl;
    Statement::indent(i);
    cout << "value : " << symbol << endl;
  }
  
  values::RuntimeValue* Identifier::evaluate_node(environment::Environment* env){
    Identifier* Identifier_ptr = dynamic_cast<Identifier*>(this);
    if(Identifier_ptr->symbol == "null"){return new values::NullValue();}
    return interpreter_functions::evaluate_identifier(this,env);
  }
  
  void yoyo(){
    cout << "heloo dude"<< endl;
  }

  Variable_Declaration::Variable_Declaration(Identifier* name,Expression* expr,DataTypes type,bool isNullable){
    varName = name;
    expression = expr;
    datatype = type;
    nullable = isNullable;
    if(datatype == DataTypes::Number){
    // runtime_values hepls to find what valuetype it will evaluate to 
      runtime_value = values::ValueType::Number;
      }else if(datatype == DataTypes::Bool){
        runtime_value = values::ValueType::Boolean;
      }else if(datatype == DataTypes::String){
        runtime_value = values::ValueType::String;
      }else{
        throw runtime_error("from ast.cpp variabledec :: unknown datatype");
      }
      setNodeType(NodeType::Variable_Declaration);
    }

  void Variable_Declaration::printNode(int i) {
    cout << "Variable_Declaration : " << endl;
    Statement::indent(i);
    cout << "variable Name : ";
    varName->printNode(i+1);
    Statement::indent(i);
    cout << "Expr : ";
    expression->printNode(i+1);
    Statement::indent(i);
    cout << "type : " + values::tostring(runtime_value)<<endl;    
  }

  values::RuntimeValue* Variable_Declaration::evaluate_node(environment::Environment* env){
    return interpreter_functions::eval_variable_declaration(this, env);
  }

  Variable_Assignment::Variable_Assignment(Identifier* name,Expression* expr,DataTypes type){
    varName = name;
    expression = expr;
    setNodeType(NodeType::Variable_Assignment);
  }

  void Variable_Assignment::printNode(int i) {
    cout << "Variable_Assignment : " << endl;
    Statement::indent(i);
    cout << "Identifier : ";
    varName->printNode(i+1);
    Statement::indent(i);
    cout << "Expr : ";
    expression->printNode(i+1);
  }

  values::RuntimeValue* Variable_Assignment::evaluate_node(environment::Environment* env){
    return interpreter_functions::eval_variable_assignment(this, env);
  }
     
  If_Statement::If_Statement(Expression* expr,vector<Statement*> body,Statement* other_if_ptr){
    condition_expr = expr;
    if_body = body;
    other_if = other_if_ptr;
    setNodeType(NodeType::If_Statement);
  }

  void If_Statement::printNode(int i){
    cout << "If Statement : " << endl;
    if(condition_expr != nullptr){
      Statement::indent(i+1);
      cout <<"Condition : ";
      condition_expr->printNode(i+2);
    }
    Statement::indent(i+1);
    cout << "Body : " << endl;
    for (int j =0;j < if_body.size();j++){
      Statement::indent(i+2);
      if_body[j]->printNode(i+3);
    }
    if(other_if != nullptr){
      Statement::indent(i+1);
      cout << "Else Body : ";
      other_if->printNode(i+2);
    } 
  }
    
  values::RuntimeValue* If_Statement::evaluate_node(environment::Environment* env){
    return interpreter_functions::eval_if_statement(this, env);
  }
    
  While_Loop::While_Loop(Expression* expr,vector<Statement*> body){
    condition_expr = expr;
    while_loop_body = body;
    setNodeType(NodeType::While_Loop);
  }

  void While_Loop::printNode(int i){
    cout << "While Loop : " << endl;
    Statement::indent(i);
    cout << "Condition : ";
    condition_expr->printNode(i+1);
    Statement::indent(i);
    cout << "Body : " << endl;
    for (int j =0;j < while_loop_body.size();j++){
      Statement::indent(i+2);
      while_loop_body[j]->printNode(i+3);
    }
  }

  values::RuntimeValue* While_Loop::evaluate_node(environment::Environment* env){
    interpreter_functions::eval_while_loop(this, env);
    return new values::Null_Interrupt_Value();
  }


  For_Loop::For_Loop(Expression* condition_expr,Expression* increment_expr,Statement* variable_bind,vector<Statement*> body){
    this->condition_expr = condition_expr;
    this->index = variable_bind;
    this->increment_expr = increment_expr;
    this->for_loop_body = body;
}

  void For_Loop::printNode(int i){
    cout << "For Loop : " << endl;
    Statement::indent(i);
    cout << "Iteration Counter : ";
    index->printNode(i+1);
    Statement::indent(i);
    cout << "Condition Expression : ";
    condition_expr->printNode(i+1);
    cout << "Increment Expression : ";
    increment_expr->printNode(i+1);
    cout << "Body : " << endl;
    for (int j =0;j < for_loop_body.size();j++){
      Statement::indent(i+2);
      for_loop_body[j]->printNode(i+3);
    }
  }

  values::RuntimeValue* For_Loop::evaluate_node(environment::Environment* env){
    return new values::NullValue();
  }

  Null_Interrupt::Null_Interrupt(){
    setNodeType(NodeType::Null_Interrupt);
  }
    
  void Null_Interrupt::printNode(int i){
    cout << "< Null_Interrupt >" << endl; 
  }     

  values::RuntimeValue* Null_Interrupt::evaluate_node(environment::Environment* env){
    return new values::Null_Interrupt_Value();
  }
    
  Break_keyWord::Break_keyWord(){
    setNodeType(NodeType::Break_Interrupt);
  }
    
  void Break_keyWord::printNode(int i){
    cout << "< Break KeyWord >" << endl; 
  }     

  values::RuntimeValue* Break_keyWord::evaluate_node(environment::Environment* env){
    return new values::Break_Interrupt_Value();
  }

}
