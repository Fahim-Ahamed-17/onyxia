#include "environment.h"
#include "values.h"
#include <iostream>
#include <algorithm>

using namespace std;

namespace environment
{

    Environment::Environment(){
        parentScope = NULL;
    }

    Environment* Environment::resolveVariable(string var_name){
        if(this->variables.count(var_name) != 0){
            return this;
        }else if((this->variables.count(var_name) == 0) && parentScope == NULL ){
            throw runtime_error("identifier '" + var_name + "' cannot be resolved");
        }

        return parentScope->resolveVariable(var_name);
    }

        values::RuntimeValue* Environment::declare_variable(string name,values::RuntimeValue* value,bool isNullable){
            if(this->variables.count(name) > 0){
                throw runtime_error("Identifier '"+name+"' is already declared!");
            }
            this->variables[name] = value;
            if(isNullable)this->nullableVariables.push_back(name);
            return value;
        }   

        values::RuntimeValue* Environment::assign_variable(string name,values::RuntimeValue* value){
            Environment* scope_of_variable = resolveVariable(name);
            scope_of_variable->variables[name] = value;
            return value;
        }

        values::RuntimeValue* Environment::lookup_variable(string name){
            Environment* scope_of_variable = resolveVariable(name);
            
            return scope_of_variable->variables.at(name);
        }


} // namespace name
