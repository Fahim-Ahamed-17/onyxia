#include <iostream>
#include <vector>

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
    Conditional_Op,
    Logical_Op,
    Unary_Op,
    End_of_file,    
};

enum class KeywordType{
    var_kw,
    const_kw,
    if_kw,
};


class Token{
  private:
    static string isKeyword(string word);
    static bool isNumber(const string& word);
    static string isDataType(string word);
    static string getTheString(const string& word,int& position);

  public:
    TokenType type;
    string value;
    Token(TokenType type,string value);
    Token();
    static Token createToken(string source,int& position);
    static string toString(TokenType type);
    static void printToken(vector<Token> tokens);
    static vector<Token>* Tokenize(string& source);
};
