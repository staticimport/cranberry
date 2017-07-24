#pragma once

#include "utils.hpp"

extern "C" {
#include "cbc/parse/expression.h"
}

using namespace ::testing;

class ExpressionTest : public ::testing::Test
{
protected:
    struct cbc_file_reader mFileReader;
    struct cbc_tokenizer   mTokenizer;

    void SetUp()
    {
      cbc_file_reader_init(&mFileReader);
      cbc_tokenizer_init(&mTokenizer);
    }

    void TearDown()
    {
      cbc_tokenizer_uninit(&mTokenizer);
      cbc_file_reader_uninit(&mFileReader);
    }
};
