
#include "lex/lexer.hpp"
#include <gtest/gtest.h>

namespace CBC { namespace Test {
using namespace ::testing;

class LexerTest : public ::testing::Test
{
protected:
  void AssertOpToken(TokenStream& stream, TokenType type, int line, int col)
  {
    Token const* t = stream.Peek();
    ASSERT_NE(nullptr, t); stream.Pop();
    EXPECT_EQ(type, t->mType);
    EXPECT_EQ(0, t->mText.length());
    EXPECT_EQ(line, t->mLineNum);
    EXPECT_EQ(col,  t->mColumn);
  }

  void AssertTextToken(TokenStream& stream, std::string const& text, TokenType type, int line, int col)
  {
    Token const* t = stream.Peek();
    ASSERT_NE(nullptr, t); stream.Pop();
    EXPECT_EQ(type, t->mType);
    EXPECT_EQ(text, t->mText);
    EXPECT_EQ(line, t->mLineNum);
    EXPECT_EQ(col,  t->mColumn);
  }
};

TEST_F(LexerTest, Empty)
{
  CharStream cs("");
  TokenStream ts = Lexer().Lex(cs);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, LeftParen)
{
  CharStream cs("((");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, RightParen)
{
  CharStream cs("))");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::RIGHT_PAREN, 1, 1);
  AssertOpToken(ts, TokenType::RIGHT_PAREN, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, LeftBrace)
{
  CharStream cs("{{");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::LEFT_BRACE, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_BRACE, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, RightBrace)
{
  CharStream cs("}}");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::RIGHT_BRACE, 1, 1);
  AssertOpToken(ts, TokenType::RIGHT_BRACE, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Comma)
{
  CharStream cs(",,");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::COMMA, 1, 1);
  AssertOpToken(ts, TokenType::COMMA, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Dot)
{
  CharStream cs("..");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::DOT, 1, 1);
  AssertOpToken(ts, TokenType::DOT, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Pound)
{
  CharStream cs("#+kjolkja\n.");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::DOT, 2, 1);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Bang)
{
  CharStream cs("!!");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::BANG, 1, 1);
  AssertOpToken(ts, TokenType::BANG, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, BangEqual)
{
  CharStream cs("!=!");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::BANG_EQUAL, 1, 1);
  AssertOpToken(ts, TokenType::BANG, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Equal)
{
  CharStream cs("=!");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::EQUAL, 1, 1);
  AssertOpToken(ts, TokenType::BANG, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, EqualEqual)
{
  CharStream cs("==(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::EQUAL_EQUAL, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Greater)
{
  CharStream cs(">(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::GREATER, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, GreaterEqual)
{
  CharStream cs(">=(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::GREATER_EQUAL, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, RightShift)
{
  CharStream cs(">>(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::RIGHT_SHIFT, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Less)
{
  CharStream cs("<(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::LESS, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, LessEqual)
{
  CharStream cs("<=(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::LESS_EQUAL, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, LeftShift)
{
  CharStream cs("<<(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::LEFT_SHIFT, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Compare)
{
  CharStream cs("<=>(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::COMPARE, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 4);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Minus)
{
  CharStream cs("-(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::MINUS, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, MinusEqual)
{
  CharStream cs("-=(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::MINUS_EQUAL, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Arrow)
{
  CharStream cs("->(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::ARROW, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Plus)
{
  CharStream cs("+(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::PLUS, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, PlusEqual)
{
  CharStream cs("+=(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::PLUS_EQUAL, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Star)
{
  CharStream cs("*(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::STAR, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, StarEqual)
{
  CharStream cs("*=(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::STAR_EQUAL, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Slash)
{
  CharStream cs("/(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::SLASH, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 2);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, SlashEqual)
{
  CharStream cs("/=(");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::SLASH_EQUAL, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_PAREN, 1, 3);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, WhiteSpace)
{
  CharStream cs("} \t\r\n{");
  TokenStream ts = Lexer().Lex(cs);
  AssertOpToken(ts, TokenType::RIGHT_BRACE, 1, 1);
  AssertOpToken(ts, TokenType::LEFT_BRACE, 2, 1);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, StringSingleQuote)
{
  CharStream cs("'he\"llo\\'there'.");
  TokenStream ts = Lexer().Lex(cs);
  AssertTextToken(ts, "he\"llo\\'there", TokenType::STRING, 1, 1);
  AssertOpToken(ts, TokenType::DOT, 1, 16);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, StringDoubleQuote)
{
  CharStream cs("\"he\\\"llo'there\".");
  TokenStream ts = Lexer().Lex(cs);
  AssertTextToken(ts, "he\\\"llo'there", TokenType::STRING, 1, 1);
  AssertOpToken(ts, TokenType::DOT, 1, 16);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Integer)
{
  CharStream cs("1234.");
  TokenStream ts = Lexer().Lex(cs);
  AssertTextToken(ts, "1234", TokenType::INTEGER, 1, 1);
  AssertOpToken(ts, TokenType::DOT, 1, 5);
  ASSERT_EQ(nullptr, ts.Peek());
}

TEST_F(LexerTest, Float)
{
  CharStream cs("1234.54.");
  TokenStream ts = Lexer().Lex(cs);
  AssertTextToken(ts, "1234.54", TokenType::FLOAT, 1, 1);
  AssertOpToken(ts, TokenType::DOT, 1, 8);
  ASSERT_EQ(nullptr, ts.Peek());
}

}}
