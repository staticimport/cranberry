
#include "tests_common.h"
#include "cbc/operators.h"

struct operators_test {};

CONST_FCN
void operators_test_create(struct operators_test* test) { (void)test; }

CONST_FCN
void operators_test_destroy(struct operators_test* test) { (void)test; }

CTEST(operators_test, determine_operator_brace_open)
{
  enum cbc_operator_type type;
  CTEST_TRUE(cbc_determine_operator("{", 1, &type));
  CTEST_INT_EQ(CBC_OPERATOR_BRACE_OPEN, type);
}

CTEST(operators_test, determine_operator_brace_close)
{
  enum cbc_operator_type type;
  CTEST_TRUE(cbc_determine_operator("}", 1, &type));
  CTEST_INT_EQ(CBC_OPERATOR_BRACE_CLOSE, type);
}

CTEST(operators_test, determine_operator_colon)
{
  enum cbc_operator_type type;
  CTEST_TRUE(cbc_determine_operator(":", 1, &type));
  CTEST_INT_EQ(CBC_OPERATOR_COLON, type);
}

CTEST(operators_test, determine_operator_paren_open)
{
  enum cbc_operator_type type;
  CTEST_TRUE(cbc_determine_operator("(", 1, &type));
  CTEST_INT_EQ(CBC_OPERATOR_PAREN_OPEN, type);
}

CTEST(operators_test, determine_operator_paren_close)
{
  enum cbc_operator_type type;
  CTEST_TRUE(cbc_determine_operator(")", 1, &type));
  CTEST_INT_EQ(CBC_OPERATOR_PAREN_CLOSE, type);
}

CTEST(operators_test, determine_operator_semicolon)
{
  enum cbc_operator_type type;
  CTEST_TRUE(cbc_determine_operator(";", 1, &type));
  CTEST_INT_EQ(CBC_OPERATOR_SEMICOLON, type);
}

CTEST(operators_test, determine_operator_not_an_operator)
{
  enum cbc_operator_type type;
  CTEST_FALSE(cbc_determine_operator(";;", 2, &type));
  CTEST_FALSE(cbc_determine_operator("<!", 2, &type));
  CTEST_FALSE(cbc_determine_operator("~", 1, &type));
}
