
#include "common.hpp"
#include "ast/function.hpp"
#include "lex/lexer.hpp"
#include "util/args.hpp"

#include <fstream>

using namespace CBC;

Args ParseArgs(int argc, char const** argv);

int main(int argc, char const** argv)
{
  // setup
  Args const args = ParseArgs(argc, argv);
  std::string const filename = args.GetMandatoryParam("file.cb");
  std::ifstream in(filename);
  CharStream charStream(in);
  TokenStream tokens = Lexer().Lex(charStream);

  // currently on a single function main supported
  Function main;
  main.Parse(tokens);
  ASSERT(nullptr == tokens.TryPop(), "Only a single function 'main' currently supported.");

  return 0;
}

Args ParseArgs(int argc, char const** argv)
{
  Args args;

  // specification
  args.AddMandatoryParam("file.cb", "cranberry source file to compile");
  args.SetVersionInfo("cbc Cranberry Compiler v0.0.0");

  // parse
  try { args.Parse(argc, argv); }
  catch (InvalidArgsException e)
  {
    std::cerr << e.what() << std::endl;
    args.PrintUsage();
    exit(1);
  }

  return args;
}

