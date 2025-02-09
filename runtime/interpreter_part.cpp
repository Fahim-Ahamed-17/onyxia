#include "environment.h"
#include "values.h"
#include "../frontend/ast.h"
#include "interpreter_part.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>

using namespace ast_types;
using namespace std;

namespace interpreter_functions{

    values::RuntimeValue* evaluate(ast_types::Statement* ast_node,environment::Environment* env){
        int int_value;
        ast_types::NumericLiteral* numeric_literal_ptr;
        bool bool_value;
        ast_types::BooleanLiteral* boolean_literal_ptr;
        string string_value;
        ast_types::StringLiteral* string_literal_ptr;
        string identifier_value;
        ast_types::Identifier* Identifier_ptr;
        switch ((*ast_node).type){
        case ast_types::NodeType::NumericLiteral :
            numeric_literal_ptr= dynamic_cast<ast_types::NumericLiteral*>(ast_node);
            int_value = numeric_literal_ptr->value;
            return new values::NumberValue(int_value);
        case ast_types::NodeType::StringLiteral  :
            string_literal_ptr= dynamic_cast<ast_types::StringLiteral*>(ast_node);
            string_value = string_literal_ptr->value;
            return new values::StringValue(string_value);
        case ast_types::NodeType::BooleanLiteral :
            boolean_literal_ptr= dynamic_cast<ast_types::BooleanLiteral*>(ast_node);
            bool_value = boolean_literal_ptr->value;
            return new values::BooleanValue(bool_value);
        case ast_types::NodeType::Identifier :
            Identifier_ptr = dynamic_cast<Identifier*>(ast_node);
            if(Identifier_ptr->symbol == "null"){return new values::NullValue();}
            return evaluate_identifier(ast_node,env);
        case ast_types::NodeType::Binary_Expression :
            return eval_binary_expr(ast_node,env);
        case ast_types::NodeType::Conditional_Expression :
            return eval_conditional_expr(ast_node,env);
        case ast_types::NodeType::Variable_Declaration:
            return eval_variable_declaration(ast_node,env);
        case ast_types::NodeType::Variable_Assignment:
            return eval_variable_assignment(ast_node,env);
        case ast_types::NodeType::If_Statement:
            return eval_if_statement(ast_node,env);
        case ast_types::NodeType::While_Loop:
            eval_while_loop(ast_node, env);
            return new values::NullValue();
        case ast_types::NodeType::Program :
            return evaluate_program(ast_node,env);
        default:
            throw runtime_error("This ast node is not implemented yet : " + ast_types::toString((*ast_node).type));
        }
    }

    values::RuntimeValue* evaluate_program(ast_types::Statement* stmt,environment::Environment* env){
        ast_types::Program* program = dynamic_cast<ast_types::Program*>(stmt); 
        values::RuntimeValue* lastEvaluated = new values::NumberValue(-1);
        for(int i =0;i< (*program).body.size() ;i++){
            lastEvaluated = evaluate((*program).body[i],env);
        }
        return lastEvaluated;
    }

    values::RuntimeValue* eval_number_binary_expr(values::NumberValue* rhs,values::NumberValue* lhs,string bin_op){
        float data;
        if(bin_op == "+"){
            data = lhs->value + rhs->value; 
        }else if(bin_op == "-"){
            data = lhs->value - rhs->value; 
        }else if(bin_op == "*"){
            data = lhs->value * rhs->value; 
        }else if(bin_op == "/"){
            data = lhs->value / rhs->value; 
        }else if(bin_op == "%"){
            int lhs_value = (int)lhs->value;
            int rhs_value = (int)lhs->value;
            data = lhs_value % rhs_value; 
        }
        return new values::NumberValue(data);
    }


    values::RuntimeValue* eval_binary_expr(ast_types::Statement* ast_node,environment::Environment* env){
        values::NumberValue* left_number_value ; 
        values::NumberValue* right_number_value;
        ast_types::BinaryExpression* bin_expr_ptr = dynamic_cast<ast_types::BinaryExpression*>(ast_node); 
        values::RuntimeValue* left_side = evaluate(bin_expr_ptr->left,env);
        values::RuntimeValue* right_side = evaluate(bin_expr_ptr->right,env);
        if(left_side->kind == values::ValueType::Number && right_side->kind == values::ValueType::Number){
            left_number_value = dynamic_cast<values::NumberValue*>(left_side); 
            right_number_value = dynamic_cast<values::NumberValue*>(right_side); 
        }else  if(left_side->kind == values::ValueType::Null || right_side->kind == values::ValueType::Null){
            return new values::NullValue();
        }
        if(left_number_value->kind == values::ValueType::Number && right_number_value->kind == values::ValueType::Number){
            return eval_number_binary_expr(right_number_value,left_number_value,bin_expr_ptr->binary_operator);
        }else{
            throw runtime_error("Binary operation for other than numbers are not implemented yet");
        }
    }

    values::RuntimeValue* eval_conditional_expr(ast_types::Statement* ast_node,environment::Environment* env){
      ConditionalExpression* logical_expr_node = dynamic_cast<ConditionalExpression*>(ast_node);
      values::RuntimeValue* left_expr_result = evaluate(logical_expr_node->left,env);
      values::RuntimeValue* right_expr_result = evaluate(logical_expr_node->right,env);
      if(left_expr_result->kind == values::ValueType::Number && right_expr_result->kind==values::ValueType::Number){
        return eval_numeric_conditional_expr(left_expr_result,right_expr_result,logical_expr_node->conditional_operator);
      }else{
        throw std::runtime_error("the operand is not defined for this type");
      } 
    }

    values::RuntimeValue* eval_numeric_conditional_expr(values::RuntimeValue* left,values::RuntimeValue* right,string logical_op){
      values::NumberValue* left_number  = dynamic_cast<values::NumberValue*>(left);
      values::NumberValue* right_number = dynamic_cast<values::NumberValue*>(right);
    
      if(logical_op == "<"){
        if(left_number->value < right_number->value) return new values::BooleanValue(true);
      }else if(logical_op == "<="){
        if(left_number->value <= right_number->value) return new values::BooleanValue(true);
      }else if(logical_op == ">"){
        if(left_number->value > right_number->value) return new values::BooleanValue(true);
      }else if(logical_op == ">="){
        if(left_number->value >= right_number->value) return new values::BooleanValue(true);
      }else if(logical_op == "=="){
        if(left_number->value == right_number->value) return new values::BooleanValue(true);
      }
      else{
        throw std::runtime_error("'" + logical_op + "' is not defined ."); 
      }
      return new values::BooleanValue(false);// this may not be good in the future but idk and idc
}
  
    values::RuntimeValue* evaluate_identifier(ast_types::Statement* ast_node,environment::Environment* env){
        ast_types::Identifier* identifier_ptr = dynamic_cast<ast_types::Identifier*>(ast_node);
        string symbol = identifier_ptr->symbol;
        values::RuntimeValue* value = env->lookup_variable(symbol);
        return value ;
    }
    //---------------------------------------------------------------------------
    // Statement

    values::RuntimeValue* eval_variable_declaration(ast_types::Statement* ast_node,environment::Environment* env){
        ast_types::Variable_Declaration* varDecPtr = dynamic_cast<ast_types::Variable_Declaration*>(ast_node);
        values::RuntimeValue* exprResult = evaluate(varDecPtr->expression,env);
        if(exprResult->kind == values::ValueType::Null && !varDecPtr->nullable) throw runtime_error("'"+varDecPtr->varName->symbol+"' is not Nullable make it explicitly nullable" );
        if(exprResult->kind != varDecPtr->runtime_value && !varDecPtr->nullable) throw  runtime_error("assignment of non compatible types expected "+values::tostring(varDecPtr->runtime_value) +" got : "+values::tostring(exprResult->kind));
        return env->declare_variable(varDecPtr->varName->symbol,evaluate(varDecPtr->expression,env),varDecPtr->nullable);
    };


values::RuntimeValue* eval_variable_assignment(ast_types::Statement* ast_node,environment::Environment* env){
  ast_types::Variable_Assignment* varDecPtr = dynamic_cast<ast_types::Variable_Assignment*>(ast_node);
        values::RuntimeValue* exprResult = evaluate(varDecPtr->expression,env);
        if(find(env->nullableVariables.begin(),env->nullableVariables.end(),varDecPtr->varName->symbol) == env->nullableVariables.end() && (exprResult->kind == values::ValueType::Null) ){
            throw runtime_error("'"+varDecPtr->varName->symbol+"' is not Nullable make it explicitly nullable");
        }
        return env->assign_variable(varDecPtr->varName->symbol,evaluate(varDecPtr->expression,env));
}


values::RuntimeValue* eval_if_statement(ast_types::Statement *ast_node,environment::Environment* env){
      If_Statement* if_node = dynamic_cast<If_Statement*>(ast_node);
      environment::Environment* curr_env = new environment::Environment();
      curr_env->parentScope = env;
      // execution of Else block since there is no expreession 
      if(if_node->condition_expr == nullptr){
          for(int j = 0;j<if_node->if_body.size();j++){
              if(if_node->if_body[j]->type == NodeType::Break_Interrupt) return new values::Break_Interrupt_Value();
              values::RuntimeValue* value = evaluate(if_node->if_body.at(j),curr_env);
              if(value->kind == values::ValueType::Break_Interrupt_value) return new values::Break_Interrupt_Value();
               // debug this line it returns for all the values 
          }
          return new values::Null_Interrupt_Value();
      } 
      bool condition_result = evaluate_truthy(if_node->condition_expr,curr_env);

      if(!condition_result){
              if(if_node->other_if == nullptr) return new values::Null_Interrupt_Value();
              values::RuntimeValue* value =  eval_if_statement(if_node->other_if,env);
              return value;
      }
    
      for(int j = 0;j<if_node->if_body.size();j++){
          if(if_node->if_body[j]->type == NodeType::Break_Interrupt) return new values::Break_Interrupt_Value();
          values::RuntimeValue* value =  evaluate(if_node->if_body.at(j),curr_env);
          if(value->kind == values::ValueType::Break_Interrupt_value) return new values::Break_Interrupt_Value();
      }

      return new values::Null_Interrupt_Value();
}


void eval_while_loop(ast_types::Statement* ast_node, environment::Environment *env){
    While_Loop* while_loop_node = dynamic_cast<While_Loop*>(ast_node);
    bool condition_state = evaluate_truthy(while_loop_node->condition_expr, env);
    bool isRunning = condition_state;//this is flipped when break is encountered
    while(isRunning){
        condition_state = evaluate_truthy(while_loop_node->condition_expr, env);
        if(condition_state){
            for(int i = 0;i < while_loop_node->while_loop_body.size();i++){
                Statement* curr_stmt = while_loop_node->while_loop_body[i];
                if(curr_stmt->type == ast_types::NodeType::Break_Interrupt){
                    isRunning = false;
                    break;
                }
                values::RuntimeValue* stmt_result = evaluate(curr_stmt,env);
                if(stmt_result->kind == values::ValueType::Break_Interrupt_value){
                    isRunning = false;
                    break;
                }
            }
        }else break;
    }
}



bool evaluate_truthy(Expression* expression,environment::Environment* env){
    
      values::RuntimeValue* condition_result = evaluate(expression,env);
      values::BooleanValue* boolean_value_ptr;
      values::NumberValue* numeric_value_ptr;
      if(condition_result->kind == values::ValueType::Boolean){
          boolean_value_ptr = dynamic_cast<values::BooleanValue*>(condition_result);
          if(boolean_value_ptr->value == false || condition_result->kind == values::ValueType::Null) return false;
      }else if(condition_result->kind == values::ValueType::Number){
          numeric_value_ptr = dynamic_cast<values::NumberValue*>(condition_result);
          if(numeric_value_ptr->value == 0)return false;
      }
      return true;
}

}

