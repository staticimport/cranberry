
#include "lex/char_stream.hpp"
#include <sstream>
#include <gtest/gtest.h>

namespace CBC { namespace Test {
using namespace ::testing;

TEST(CharStreamTest, FromStream)
{
  std::istringstream in("hello");
  CharStream cs(in);
  ASSERT_EQ('h', cs.Pop());
  ASSERT_EQ('e', cs.Pop());
  ASSERT_EQ('l', cs.Pop());
  ASSERT_EQ('l', cs.Pop());
  ASSERT_EQ('o', cs.Pop());
  EXPECT_THROW(cs.Pop(), std::runtime_error);
}

TEST(CharStreamTest, FromCStr)
{
  CharStream cs("hello");
  ASSERT_EQ('h', cs.Pop());
  ASSERT_EQ('e', cs.Pop());
  ASSERT_EQ('l', cs.Pop());
  ASSERT_EQ('l', cs.Pop());
  ASSERT_EQ('o', cs.Pop());
  EXPECT_THROW(cs.Pop(), std::runtime_error);
}

TEST(CharStreamTest, PeekPopPush)
{
  CharStream cs("hello");
  char c;
  ASSERT_TRUE(cs.Peek(&c)); ASSERT_EQ('h', c);
  ASSERT_TRUE(cs.Peek(&c)); ASSERT_EQ('h', c);
  ASSERT_EQ('h', cs.Pop());
  ASSERT_EQ('e', cs.Pop());
  ASSERT_EQ('l', cs.Pop());
  cs.Push();
  ASSERT_TRUE(cs.Peek(&c)); ASSERT_EQ('l', c);
  cs.Push();
  ASSERT_TRUE(cs.Peek(&c)); ASSERT_EQ('e', c);
  cs.Push();
  ASSERT_TRUE(cs.Peek(&c)); ASSERT_EQ('h', c);
  EXPECT_THROW(cs.Push(), std::runtime_error);
  ASSERT_EQ('h', cs.Pop());
  ASSERT_EQ('e', cs.Pop());
  ASSERT_EQ('l', cs.Pop());
  ASSERT_EQ('l', cs.Pop());
  ASSERT_EQ('o', cs.Pop());
  ASSERT_FALSE(cs.Peek(&c));
  EXPECT_THROW(cs.Pop(), std::runtime_error);
}

}}
