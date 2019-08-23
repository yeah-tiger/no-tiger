#pragma once
#include "token.hpp"
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
namespace ntc
{ 
  class Tokenizer {
    // static fields
    private:
    static std::unordered_map<std::string, TokenType> keyword_map;

    public:
    Tokenizer() = delete;

    Tokenizer(const std::string& filename);

    Token next_token();
    

    private:
    inline void advance();

    inline char peek();
    
    inline void add_char();
    
    void skip_whitespaces();

    void skip_comments();

    inline Token make_token(TokenType type, SourceLocation location);
    
    inline SourceLocation make_location();

    Token handle_identifer_or_keyword();

    Token handle_number();

    Token handle_string();

    Token handle_char();

    Token handle_symbol_or_op();

    void diag(const std::string& msg);

    bool error_flag;

    std::vector<std::string> error_msgs;

    char current_char;
    
    Token current_token;

    std::string token_buffer;

    std::string filename;

    std::ifstream in;

    int column;

    int line;

   };


} // namespace ntc
