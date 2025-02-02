#include "values.h"
#include <iostream>

using namespace std;

namespace values{
    NumberValue::NumberValue(float data){
        kind = ValueType::Number;
        value = data;
    }


    void NumberValue::printValue(){
        std::cout << "NumberValue : " << value << endl; 
    }

    void NullValue::printValue(){
        std::cout << "NullValue : " << value << endl; 
    }


    BooleanValue::BooleanValue(bool data){
        kind = ValueType::Boolean;
        value = data;
    }

    void BooleanValue::printValue(){
        std::cout << "BooleanValue : " << value << endl; 
    }

    StringValue::StringValue(string sentence){
        kind = ValueType::String;
        value = sentence;
    }


    void StringValue::printValue(){
        std::cout << "StringValue : " << value << endl; 
    }
    
    void Null_Interrupt_Value::printValue(){
        std::cout << "Null interrupt value " << endl;
    }

    void Break_Interrupt_Value::printValue(){
        std::cout << "Break interrupt value " << endl;
    }
}
