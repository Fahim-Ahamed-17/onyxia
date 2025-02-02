#include <iostream>
#include <iomanip>
#include <vector>
#include <cctype>
#include <algorithm>

using namespace std;

enum class TokenType{
    Bin_op,
    Equals,
    Open_paren,
    Close_paren,
    DataType,
    KeyWords,
    Identifiers,
    Number,
    Boolean,
    String,
    SemiColon,
    Colon,
    OpenBrace,
    CloseBrace,
    QuestionMark,
    DoubleQuotes,
    Logical_Op,
    End_of_file,    
};

enum class KeywordType{
    var_kw,
    const_kw,
    if_kw,
};

class Token{
    private:
        static string isKeyword(string word){
            int number_of_keywords = 6;
            string reserved_KeyWords[] = {"if","let","var","else","while","break"};// change the number
            for(int i = 0;i < number_of_keywords;i++){
                if(reserved_KeyWords[i] == word){
                    return reserved_KeyWords[i];
                }
            }
            return "";
        }


        static string isDataType(string word){
            int number_of_DataType = 3;
            string datatypes[] = {"Number","Boolean","String","Null"};
            for(int i = 0;i < number_of_DataType;i++){
                if(datatypes[i] == word){
                    return datatypes[i];
                }
            }
            return "";
        }


        static bool isNumber(const std::string& word) {
            if(word.empty()) return false;
            int start = (word[0] == '-' || word[0] == '+') ? 1:0;
            return start < word.length() && std::all_of(word.begin()+start,word.end(),::isdigit);
        }

        static string getTheString(const std::string& word,int& position){
            string sentence = "";
            while (position < word.length()-1 && word[position] != '"') {
                sentence.push_back(word[position]);
                position++;
            }
            position++;
            return sentence;
        }   


    public :
        TokenType type;
        string value;
        Token(TokenType t,string data){
            type = t;
            value = data;
        }
        
        Token(){
            value = "";
        }

        static Token createToken(string  source  ,int& position){
            while(source[position] == ' ' && position < source.length()-1){
                position++;
            }
            if(source[position] == ' ' && position == source.length()-1){
                position++;
                return Token(TokenType::End_of_file,"EOF");
            }
            char x = source[position];
            position++;
            static string sentence = "";//this is used as a buffer for the string literal
            Token newToken;
            switch (x)
            {
                case '+':newToken = Token(TokenType::Bin_op,"+");break;
                case '-':newToken = Token(TokenType::Bin_op,"-");break;
                case '*':newToken = Token(TokenType::Bin_op,"*");break;
                case '/':newToken = Token(TokenType::Bin_op,"/");break;
                case '(':newToken = Token(TokenType::Open_paren,"(");break;
                case ')':newToken = Token(TokenType::Close_paren,")");break;
                case '%':newToken = Token(TokenType::Bin_op,"%");break;
                case ':':newToken = Token(TokenType::Colon,":");break;
                case ';':newToken = Token(TokenType::SemiColon,";");break;
                case '?':newToken = Token(TokenType::QuestionMark,"?");break; 
                case '{':newToken = Token(TokenType::OpenBrace,"{");break; 
                case '}':newToken = Token(TokenType::CloseBrace,"}");break; 
                case '<':
                    if(source[position] == '='){
                        position++;
                        newToken = Token(TokenType::Logical_Op,"<=");
                        break;
                    }else{
                        newToken = Token(TokenType::Logical_Op,"<");
                        break;
                    }
                case '>':
                    if(source[position] == '='){
                        position++;
                        newToken = Token(TokenType::Logical_Op,">=");
                        break;
                    }else{
                        newToken = Token(TokenType::Logical_Op,">");
                        break;
                    }
                case '=':
                    if(source[position] == '='){
                        position++;
                        newToken = Token(TokenType::Logical_Op,"==");
                        break;
                    }else{
                        newToken = Token(TokenType::Equals,"=");
                        break;
                    }

                case '"':
                    sentence = getTheString(source,position);
                    newToken = Token(TokenType::String,sentence);
                    break;
                default:break;
            }

            if(newToken.value != "") return newToken;
            string word ;
            word.push_back(x);
            while(position < source.length()&&(isalnum((source[position])) || source[position] == '_')){
                word.push_back(source[position]);
                position++;
            }
            
            // Checking for Keywords
            string result = isKeyword(word);// if the return val is not empty it is a keyword
            if(result != ""){
                newToken = Token(TokenType::KeyWords,result);
                return newToken;
            }

            result = isDataType(word);
            if(result != ""){
                newToken = Token(TokenType::DataType,result);
                return newToken;
            }

            // Checking for integers
            if(isNumber(word)){
                return Token(TokenType::Number,word);
            }

            // Checking for boolean
            if(word == "true" || word == "false"){
                return Token(TokenType::Boolean,word);
            }

            newToken = Token(TokenType::Identifiers,word);
            return newToken;
        }

        static string toString(TokenType type) {
            switch (type) {
                    case TokenType::Bin_op: return "Bin_op";
                    case TokenType::Open_paren: return "Open_paren";
                    case TokenType::Close_paren: return "Close_paren";
                    case TokenType::KeyWords: return "KeyWords";
                    case TokenType::Identifiers: return "Identifiers";
                    case TokenType::Number: return "Number";
                    case TokenType::Boolean: return "Boolean";
                    case TokenType::DataType: return "DataType";
                    case TokenType::End_of_file: return "End_of_file";
                    case TokenType::OpenBrace: return "Open_Brace";
                    case TokenType::CloseBrace: return "CloseBrace";
                    case TokenType::Logical_Op: return "Logical op";
                    case TokenType::SemiColon: return "SemiColon";
                    case TokenType::Colon : return "Colon";
                      default: return "Unknown";
                }
        }

        static void printTokens(vector<Token> tokens){
            cout << "--------" << endl;
            for(int i = 0; i< tokens.size();i++){
               cout << " TokenType : " << left << setw(15)<< Token::toString(tokens[i].type) << "| value : " << tokens[i].value << endl;
            }
        }

        static vector<Token>* Tokenize(string& source){
            source.push_back(' ');
            vector<Token>* tokens = new vector<Token>();
            int i = 0;
            int src_lenght = source.length();
            while (i < src_lenght)
            {
                Token newToken = Token::createToken(source,i);
                (*tokens).push_back(newToken);

            }
            return tokens;
        }
};


// int main(){
//     string src = "  (+-*) let if ahamed12 15 true 17a" ;
//     vector<Token> tokens = *(Token::Tokenize(src));
//     Token::printTokens(tokens);
//     return 0;
// }
