#ifndef VALUES_H
#define VALUES_H

#include <iostream>

using namespace std;

namespace values{
    enum class ValueType{
        Number,
        Boolean,
        String,
        Null,
        Null_Interrupt_value,
        Break_Interrupt_value
    };

    static string tostring(ValueType a){
        switch (a)
        {
        case ValueType::Number : return "Number";
        case ValueType::String: return "String";
        case ValueType::Boolean : return "Boolean";
        case ValueType::Null : return "null";
        case ValueType::Null_Interrupt_value : return "Null_Interrupt_value";
        case ValueType::Break_Interrupt_value : return "Break_Interrupt_value";
        default:throw runtime_error("unkown valuetype");
        }
    }

    class RuntimeValue{
        public :
            ValueType kind;
            virtual void printValue(){
                cout << "runtime value printing"<<endl;
            };
    };

    class NumberValue : public RuntimeValue{
       public :
            NumberValue(float data);
            string kindInString = "Number";//this is to compare the array of possible types
            float value;
            void printValue() override;
    };

    class StringValue : public RuntimeValue{
       public :
            StringValue(string sentence);
            string kindInString = "String";//this is to compare the array of possible types
            string value;
            void printValue() override;
    };

    class NullValue : public RuntimeValue{
        public :
            NullValue(){
                kind = ValueType::Null;
            }
            string value = "null";
            void printValue() override;
    };

    class BooleanValue : public RuntimeValue{
        public :
            BooleanValue(bool data);
            string kindInString = "Boolean";//this is to compare the array of possible types
            bool value;
            void printValue() override;
    };


    class Null_Interrupt_Value : public RuntimeValue{
        public : 
            Null_Interrupt_Value(){
               kind = ValueType::Null_Interrupt_value;
            }
            void printValue() override;
    };

    class Break_Interrupt_Value : public RuntimeValue{
        public : 
            Break_Interrupt_Value(){
              kind = ValueType::Break_Interrupt_value;

            }
            void printValue() override;
    };

    

}

#endif 
