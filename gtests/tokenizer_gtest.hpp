#pragma once

#include "utils.hpp"

extern "C" {
#include "cbc/tokenizer.h"
}

using namespace ::testing;

class TokenizerTest : public ::testing::Test
{
protected:
  struct cbc_tokenizer mTokenizer;
  struct cbc_file_reader mFileReader;

  void SetUp()
  {
    cbc_tokenizer_init(&mTokenizer);
    cbc_file_reader_init(&mFileReader);
  }

  void TearDown()
  {
    cbc_file_reader_uninit(&mFileReader);
    cbc_tokenizer_uninit(&mTokenizer);
  }

  void ExpectToken(std::string const& s, enum cbc_token_type type)
  {
    struct cbc_token token;
    ASSERT_TRUE(cbc_tokenizer_next(&mTokenizer, &token));
    EXPECT_EQ(s, std::string(token.text, token.textlen));
    EXPECT_EQ(type, token.type);
  }

  void ExpectNoMoreTokens()
  {
    struct cbc_token token;
    ASSERT_FALSE(cbc_tokenizer_next(&mTokenizer, &token));
  }
};

TEST_F(TokenizerTest, Init)
{
  EXPECT_EQ(nullptr, mTokenizer.file_reader);
  EXPECT_NE(nullptr, mTokenizer.buffer);
}

TEST_F(TokenizerTest, Identifiers)
{
  cbc_file_reader_load(&mFileReader, "gtests/resources/tokenizer_gtest_identifiers.txt");
  cbc_tokenizer_start(&mTokenizer, &mFileReader);
  ExpectToken("foo", CBC_TOKEN_IDENTIFIER);
  ExpectToken("Bar", CBC_TOKEN_IDENTIFIER);
  ExpectToken("bLaH", CBC_TOKEN_IDENTIFIER);
  ExpectToken("HERE", CBC_TOKEN_IDENTIFIER);
  ExpectToken("@this_one", CBC_TOKEN_IDENTIFIER);
  ExpectToken("$sup?", CBC_TOKEN_IDENTIFIER);
  ExpectNoMoreTokens();
}

TEST_F(TokenizerTest, Numbers)
{
  cbc_file_reader_load(&mFileReader, "gtests/resources/tokenizer_gtest_numbers.txt");
  cbc_tokenizer_start(&mTokenizer, &mFileReader);
  ExpectToken("1", CBC_TOKEN_NUMBER);
  ExpectToken("89", CBC_TOKEN_NUMBER);
  ExpectToken("1.982", CBC_TOKEN_NUMBER);
  ExpectToken("0.1275828", CBC_TOKEN_NUMBER);
  ExpectToken("1e9", CBC_TOKEN_NUMBER);
  ExpectToken("94e-7", CBC_TOKEN_NUMBER);
  ExpectToken("0xDEADBEEF", CBC_TOKEN_NUMBER);
  ExpectToken("0x9bac04", CBC_TOKEN_NUMBER);
  ExpectNoMoreTokens();
}

TEST_F(TokenizerTest, Strings)
{
  cbc_file_reader_load(&mFileReader, "gtests/resources/tokenizer_gtest_strings.txt");
  cbc_tokenizer_start(&mTokenizer, &mFileReader);
  ExpectToken("\"\"", CBC_TOKEN_STRING);
  ExpectToken("''", CBC_TOKEN_STRING);
  ExpectToken("\"hi there\"", CBC_TOKEN_STRING);
  ExpectToken("'what\"s\" Up?'", CBC_TOKEN_STRING);
  ExpectToken("\"how 'about' now?\"", CBC_TOKEN_STRING);
  ExpectToken("'(\\'\"'", CBC_TOKEN_STRING);
  ExpectToken("\"#\\\"$\"", CBC_TOKEN_STRING);
  ExpectNoMoreTokens();
}
