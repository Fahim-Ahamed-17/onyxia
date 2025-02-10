#ifndef AST_H
#define AST_H

#include<string>
#include<vector>
#include<iostream>
#include "../runtime/values.h"

using namespace std;

namespace ast_types{

    enum class NodeType{
        Program,
        NumericLiteral,
        BooleanLiteral,
        StringLiteral,
        NullLiteral,
        Identifier,
        Binary_Expression,
        Conditional_Expression,
        Variable_Declaration,
        Variable_Assignment,
        If_Statement,
        While_Loop,
        Null_Interrupt,
        Break_Interrupt,
    };

    enum class DataTypes{
        Number,
        Bool,
        String,
        Null,
    };

    string toString(NodeType a);

    // This Class is Just an Abstract class so that other NodeTypes can  be built on top of this
    class Statement{ 
      public :    
        NodeType type; // type of statement
        virtual void dummy(){cout << "stmt\n";};
        void setNodeType(NodeType NodeKind);
        NodeType getNodeType();
        virtual void printNode(int i){
          cout << "statements print" << endl;
        };
        void indent(int i){
          for(int j = 1; j <= i;j++){
            cout << "  " ;
          }
        }
        virtual values::RuntimeValue* evaluate_node();
        virtual ~Statement(){}
    };

    class Expression : public Statement{
      public :
        virtual void dummy2(){};
        virtual void printNode(int i){};
        virtual values::RuntimeValue* evaluate_node();
        virtual ~Expression(){}
            
    };


    class Program : public Statement{
      public : 
        Program();
        vector<Statement*> body;
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;
    };

    class BinaryExpression : public Expression{
      public :
        Expression* left;
        Expression* right;
        std::string binary_operator;

        BinaryExpression(
          Expression* left,
          Expression* right,
          string binary_operator
        );
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;
   };



    class ConditionalExpression : public Expression{
      public :
        Expression* left;
        Expression* right;
        string conditional_operator;

        ConditionalExpression(
          Expression* left,
          Expression* right,
          string conditional_operator
        );

        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;
   };

    class NumericLiteral : public Expression{
      public :
        float value;
        NumericLiteral(string number);
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;

    };

    class BooleanLiteral :public Expression{
      public :
        bool value;
        BooleanLiteral(string data);
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;
    };

    class StringLiteral :public Expression{
      public:
        string value;
        StringLiteral(string literal);
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;
    };


    class Identifier : public Expression{
      public :
        string symbol;
        Identifier(string value);
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;
    };


    // Statements --------------------------------------------------------------

    class Variable_Declaration : public Statement{
      public :
        Variable_Declaration(Identifier* name,Expression* expr,DataTypes data,bool isNullable);
        Identifier* varName;
        Expression* expression;
        DataTypes datatype;
        values::ValueType runtime_value;
        bool nullable;
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;
    };

    class Variable_Assignment : public Statement{
      public :
        Variable_Assignment(Identifier* name,Expression* expr,DataTypes data);
        Identifier* varName;
        Expression* expression;
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;
    };

    class If_Statement : public Statement{
      public:
        If_Statement(Expression* expr,vector<Statement*> body,Statement* other_if_ptr = nullptr);
        bool else_if = false;
        Expression* condition_expr;
        vector<Statement*> if_body;
        Statement* other_if = nullptr;
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;
    };

  
    class While_Loop : public Statement{
      public:
        While_Loop(Expression* expr,vector<Statement*> body);
        Expression* condition_expr;
        vector<Statement*> while_loop_body;
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;
    };
    
    //this class acts as the placeholder for statements like break,return,continue
    class Null_Interrupt :public Statement{
      public : 
        Null_Interrupt();
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override; 
    };  


    class Break_keyWord :public Statement{
      public :
        Break_keyWord();
        void printNode(int i) override;
        values::RuntimeValue* evaluate_node() override;

    }; 

}


#endif
