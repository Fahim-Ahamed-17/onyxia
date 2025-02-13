#include "environment.h"
#include "values.h"
#include "../frontend/ast.h"
#include "interpreter_part.h"
#include <ios>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <string>

using namespace ast_types;
using namespace std;

namespace interpreter_functions{

//    values::RuntimeValue* evaluate(ast_types::Statement* ast_node,environment::Environment* env){
  //    return new values::NullValue(); 
    //}

  values::RuntimeValue* evaluate_program(ast_types::Statement* stmt,environment::Environment* env){
    ast_types::Program* program = dynamic_cast<ast_types::Program*>(stmt); 
    values::RuntimeValue* lastEvaluated = new values::NumberValue(-1);
    for(int i =0;i< (*program).body.size() ;i++){
      //lastEvaluated = evaluate((*program).body[i],env);
      lastEvaluated = program->body[i]->evaluate_node(env);    
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
        values::RuntimeValue* left_side = bin_expr_ptr->left->evaluate_node(env);
        values::RuntimeValue* right_side = bin_expr_ptr->right->evaluate_node(env);
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
      ConditionalExpression* conditional_expr_node = dynamic_cast<ConditionalExpression*>(ast_node);
      values::RuntimeValue* left_expr_result = conditional_expr_node->left->evaluate_node(env);
      values::RuntimeValue* right_expr_result = conditional_expr_node->right->evaluate_node(env);
      if(left_expr_result->kind == values::ValueType::Number && right_expr_result->kind==values::ValueType::Number){
        return eval_numeric_conditional_expr(left_expr_result,right_expr_result,conditional_expr_node->conditional_operator);
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

    values::RuntimeValue* eval_logical_expr(ast_types::Statement* ast_node,environment::Environment* env){
      ast_types::LogicalExpression* logical_expr_node = dynamic_cast<ast_types::LogicalExpression*>(ast_node);
      bool left_result =  evaluate_truthy(logical_expr_node->left,env); 
      bool right_result = evaluate_truthy(logical_expr_node->right,env); 
      string logical_op = logical_expr_node->logical_operator;
      if(logical_op == "&&"){
        if(left_result && right_result){
          return new values::BooleanValue(true);
        }
        return new values::BooleanValue(false);
      }else if(logical_op == "||"){
        if(left_result || right_result){
          return new values::BooleanValue(true);
        }
        return new values::BooleanValue(false);
      }
      return new values::BooleanValue(false); 
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
        values::RuntimeValue* exprResult = varDecPtr->expression->evaluate_node(env);
        if(exprResult->kind == values::ValueType::Null && !varDecPtr->nullable) throw runtime_error("'"+varDecPtr->varName->symbol+"' is not Nullable make it explicitly nullable" );
        if(exprResult->kind != varDecPtr->runtime_value && !varDecPtr->nullable) throw  runtime_error("assignment of non compatible types expected "+values::tostring(varDecPtr->runtime_value) +" got : "+values::tostring(exprResult->kind));
        return env->declare_variable(varDecPtr->varName->symbol,varDecPtr->expression->evaluate_node(env),varDecPtr->nullable);
    };


values::RuntimeValue* eval_variable_assignment(ast_types::Statement* ast_node,environment::Environment* env){
  ast_types::Variable_Assignment* varDecPtr = dynamic_cast<ast_types::Variable_Assignment*>(ast_node);
        values::RuntimeValue* exprResult = varDecPtr->expression->evaluate_node(env);
        if(find(env->nullableVariables.begin(),env->nullableVariables.end(),varDecPtr->varName->symbol) == env->nullableVariables.end() && (exprResult->kind == values::ValueType::Null) ){
            throw runtime_error("'"+varDecPtr->varName->symbol+"' is not Nullable make it explicitly nullable");
        }
        return env->assign_variable(varDecPtr->varName->symbol,varDecPtr->expression->evaluate_node(env));
}


values::RuntimeValue* eval_if_statement(ast_types::Statement *ast_node,environment::Environment* env){
      If_Statement* if_node = dynamic_cast<If_Statement*>(ast_node);
      environment::Environment* curr_env = new environment::Environment();
      curr_env->parentScope = env;
      // execution of Else block since there is no expreession 
      if(if_node->condition_expr == nullptr){
          for(int j = 0;j<if_node->if_body.size();j++){
              if(if_node->if_body[j]->type == NodeType::Break_Interrupt) return new values::Break_Interrupt_Value();
              values::RuntimeValue* value = if_node->if_body.at(j)->evaluate_node(curr_env);
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
          values::RuntimeValue* value =  if_node->if_body.at(j)->evaluate_node(curr_env);
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
                values::RuntimeValue* stmt_result = curr_stmt->evaluate_node(env);
                if(stmt_result->kind == values::ValueType::Break_Interrupt_value){
                    isRunning = false;
                    break;
                }
            }
        }else break;
    }
}



bool evaluate_truthy(Expression* expression,environment::Environment* env){
    
      values::RuntimeValue* condition_result = expression->evaluate_node(env);
      values::BooleanValue* boolean_value_ptr;
      values::NumberValue* numeric_value_ptr;
      if(condition_result->kind == values::ValueType::Boolean){
          boolean_value_ptr = dynamic_cast<values::BooleanValue*>(condition_result);
          if(boolean_value_ptr->value == false || condition_result->kind == values::ValueType::Null) return false;
      }else if(condition_result->kind == values::ValueType::Number){
          numeric_value_ptr = dynamic_cast<values::NumberValue*>(condition_result);
          if(numeric_value_ptr->value == 0)return false;
      }else if(condition_result->kind == values::ValueType::Null){
        return false;
      }
      return true;
}

}

