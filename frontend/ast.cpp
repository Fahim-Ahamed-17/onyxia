#include <iostream>
#include <vector>
#include <iomanip>
#include "ast.h"

using namespace std;

namespace ast_types{
    

    string toString(NodeType a){
        switch (a)
        {
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

    NumericLiteral::NumericLiteral(string num){
        value = stoi(num);
        setNodeType(NodeType::NumericLiteral);
    }

    void NumericLiteral::printNode(int i) {
        cout << "NumericLiteral : " << endl;
        Statement::indent(i);
        cout << "value : " << fixed << setprecision(3) << value << endl;
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
    StringLiteral::StringLiteral(string word){
        value = word;
        setNodeType(NodeType::StringLiteral);
    }


    void StringLiteral::printNode(int i) {
        cout << "StringLiteral : " << endl;
        Statement::indent(i);
        cout << "value : " << fixed << setprecision(3) << value << endl;
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

     If_Statement::If_Statement(Expression* expr,vector<Statement*> body,Statement* other_if_ptr){
        condition_expr = expr;
        if_body = body;
        other_if = other_if_ptr;
        setNodeType(NodeType::If_Statement);
    }
    void If_Statement::printNode(int i){
        Statement::indent(i);
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
            cout << "Else Body : " << endl;
            other_if->printNode(i+2);
        } 
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


    Null_Interrupt::Null_Interrupt(){
        setNodeType(NodeType::Null_Interrupt);
    }
    
    void Null_Interrupt::printNode(int i){
        cout << "< Null_Interrupt >" << endl; 
    }     


    Break_keyWord::Break_keyWord(){
        setNodeType(NodeType::Break_Interrupt);
    }
    
    void Break_keyWord::printNode(int i){
        cout << "< Break KeyWord >" << endl; 
    }     



}
