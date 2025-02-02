#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>
#include "values.h"
#include <vector>

using namespace std;

namespace environment
{
    class Environment{
        public :
            Environment* parentScope;
            map<string , values::RuntimeValue*> variables; 
            vector<string> nullableVariables;// holds which elements are nullable
            Environment();
            Environment* resolveVariable(string var_name);
            values::RuntimeValue* declare_variable(string name,values::RuntimeValue* value,bool isNullable);
            values::RuntimeValue* assign_variable(string name,values::RuntimeValue* value);
            values::RuntimeValue* lookup_variable(string name);
    };
} 


#endif
