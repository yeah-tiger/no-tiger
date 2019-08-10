#pragma once
#include "token.hpp"
#include <unordered_map>
#include <string>
namespace ntc
{ 
  class Tokenizer {
    public:
    
    Token next_token();
        
    private:
    static std::unordered_map<std::string, TokenType> keyword_map;
    std::string filename;
    
   };


} // namespace ntc
