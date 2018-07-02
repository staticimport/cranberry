#pragma once

#include "ast/ast.hpp"

namespace CBC {

class TypeReference : public ASTNode
{
public:
  void Parse(TokenStream&) override;
  void Transpile(std::ostream&) const override;
private:
  std::string mName;
  void* mType; // TODO: pointer to Class, function to resolve
};

void TypeReference::Parse(TokenStream& tokens)
{
  int bracket_depth = 0;
  mType = nullptr; // to be resolved later
  mName = TokenUtils::PopType(tokens, TokenType::IDENTIFIER, "type reference name");
  do {
    Token const* t = tokens.Peek();
    if (!t) { goto done_parsing_type; }
    switch (t->mType)
    {
    case TokenType::LEFT_BRACKET:   ++bracket_depth; break;
    case TokenType::RIGHT_BRACKET:  --bracket_depth; break;
    case TokenType::IDENTIFIER:     break;
    case TokenType::COMMA:          break;

    // remaining token types all cause the loop to be exited
    case TokenType::LEFT_PAREN:
    case TokenType::RIGHT_PAREN:
    case TokenType::LEFT_BRACE:
    case TokenType::RIGHT_BRACE:
    case TokenType::DOT:
    case TokenType::COLON:
    case TokenType::BANG:
    case TokenType::BANG_EQUAL:
    case TokenType::EQUAL:
    case TokenType::EQUAL_EQUAL:
    case TokenType::GREATER:
    case TokenType::GREATER_EQUAL:
    case TokenType::RIGHT_SHIFT:
    case TokenType::LESS:
    case TokenType::LESS_EQUAL:
    case TokenType::LEFT_SHIFT:
    case TokenType::COMPARE:
    case TokenType::MINUS:
    case TokenType::MINUS_EQUAL:
    case TokenType::ARROW:
    case TokenType::PLUS:
    case TokenType::PLUS_EQUAL:
    case TokenType::STAR:
    case TokenType::STAR_EQUAL:
    case TokenType::SLASH:
    case TokenType::SLASH_EQUAL:
    case TokenType::STRING:
    case TokenType::INTEGER:
    case TokenType::FLOAT:
    case TokenType::AND:
    case TokenType::BOOL:
    case TokenType::CATCH:
    case TokenType::DEF:
    case TokenType::ELSE:
    case TokenType::ELSE_IF:
    case TokenType::FCN:
    case TokenType::FLT:
    case TokenType::IF:
    case TokenType::INT:
    case TokenType::NEXT:
    case TokenType::NIL:
    case TokenType::NO:
    case TokenType::NOT:
    case TokenType::OR:
    case TokenType::SELF:
    case TokenType::STR:
    case TokenType::SUPER:
    case TokenType::TRY:
    case TokenType::UNLESS:
    case TokenType::UNTIL:
    case TokenType::WHILE:
    case TokenType::YES:
    case TokenType::YIELD:
    case TokenType::VAL:
    case TokenType::VAR:
    default:                        goto done_parsing_type;
    }
    mName += t->mText;
  } while (bracket_depth > 0);
done_parsing_type:
  if (bracket_depth != 0) { throw ParseError("type reference -> failed to parse"); }
}

void TypeReference::Transpile(std::ostream& out) const
{
  out << "TypeReference::Transpile TODO!\n";
}

}
