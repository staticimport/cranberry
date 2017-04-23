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

  void SetUp()
  {
    cbc_tokenizer_init(&mTokenizer);
  }

  void TearDown()
  {
    cbc_tokenizer_uninit(&mTokenizer);
  }
};

TEST_F(TokenizerTest, Init)
{
  EXPECT_EQ(nullptr, mTokenizer.file_reader);
  EXPECT_NE(nullptr, mTokenizer.buffer);
}

