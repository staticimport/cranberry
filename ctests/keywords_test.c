
#include "tests_common.h"
#include "cbc/keywords.h"

struct keywords_test {};

CONST_FCN
void keywords_test_create(struct keywords_test* test) { (void)test; }

CONST_FCN
void keywords_test_destroy(struct keywords_test* test) { (void)test; }

CTEST(keywords_test, determine_keyword_fcn)
{
  enum cbc_keyword_type type;
  CTEST_TRUE(cbc_determine_keyword("fcn", 3, &type));
  CTEST_INT_EQ(CBC_KEYWORD_FCN, type);
}

CTEST(keywords_test, determine_keyword_i32)
{
  enum cbc_keyword_type type;
  CTEST_TRUE(cbc_determine_keyword("i32", 3, &type));
  CTEST_INT_EQ(CBC_KEYWORD_I32, type);
}

CTEST(keywords_test, determine_keyword_return)
{
  enum cbc_keyword_type type;
  CTEST_TRUE(cbc_determine_keyword("return", 6, &type));
  CTEST_INT_EQ(CBC_KEYWORD_RETURN, type);
}

CTEST(keywords_test, determine_keyword_not_a_keyword)
{
  enum cbc_keyword_type type;
  CTEST_FALSE(cbc_determine_keyword("return", 5, &type));
  CTEST_FALSE(cbc_determine_keyword("Return", 6, &type));
  CTEST_FALSE(cbc_determine_keyword("I32", 3, &type));
  CTEST_FALSE(cbc_determine_keyword("int", 3, &type));
}

CTEST(keywords_test, is_type)
{
  CTEST_TRUE(cbc_keyword_is_type(CBC_KEYWORD_I32));

  CTEST_FALSE(cbc_keyword_is_type(CBC_KEYWORD_FCN));
  CTEST_FALSE(cbc_keyword_is_type(CBC_KEYWORD_RETURN));
}

