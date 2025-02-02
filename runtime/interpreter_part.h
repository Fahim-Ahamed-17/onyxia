#ifndef INTERPRETER_PART_H
#define INTERPRETER_PART_H

#include "values.h"
#include "environment.h"
#include "../frontend/ast.h"

namespace interpreter_functions
{
    values::RuntimeValue* evaluate(ast_types::Statement* ast_node,environment::Environment* env);
    values::RuntimeValue* evaluate_program(ast_types::Statement* stmt,environment::Environment* env);
    values::RuntimeValue* eval_binary_expr(ast_types::Statement* ast_node,environment::Environment* env);
    values::RuntimeValue* eval_number_binary_expr(values::NumberValue* rhs,values::NumberValue* lhs,string bin_op);
    values::RuntimeValue* evaluate_identifier(ast_types::Statement* ast_node,environment::Environment* env);
    values::RuntimeValue* eval_logical_expr(ast_types::Statement* ast_node,environment::Environment* env);
    values::RuntimeValue* eval_numeric_logical_expr(values::RuntimeValue* left,values::RuntimeValue* right,string logical_op);

    values::RuntimeValue* eval_variable_declaration(ast_types::Statement* ast_node,environment::Environment* env);
    values::RuntimeValue* eval_variable_assignment(ast_types::Statement* ast_node,environment::Environment* env);
values::RuntimeValue* eval_if_statement(ast_types::Statement* ast_node,environment::Environment* env);
    void eval_while_loop(ast_types::Statement* ast_node,environment::Environment* env);
    bool evaluate_truthy(ast_types::Expression* expression,environment::Environment* env);
}
    

#endif
