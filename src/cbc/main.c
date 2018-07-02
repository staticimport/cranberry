
#include "cbc/ast/ast.h"
#include "cbc/cgen/cgen.h"
#include "cbc/tokens.h"
#include "cbc/utils/files.h"

int main(int argc, char** argv)
{
  // args
  if (argc != 4) { fprintf(stderr, "USAGE: cbc source_file.cb output_directory app_name\n"); exit(1); }
  char const* filename = argv[1];
  char const* output_directory = argv[2];
  char const* app_name = argv[3];

  // load file
  size_t source_len; char* source = cbc_load_file(filename, &source_len);
  ASSERT(source != NULL);
  printf("source_len:%zu\n", source_len);

  // tokenize
  size_t num_tokens; struct cbc_token* tokens = cbc_tokenize(source, source_len, &num_tokens);
  printf("num_tokens:%zu\n", num_tokens);

  // AST
  struct cbc_ast ast; cbc_ast_parse(&ast, tokens, tokens + num_tokens);

  // C Gen
  char* c_filename;
  asprintf(&c_filename, "%s/%s.gen.c", output_directory, basename(filename));
  cbc_cgen(&ast, c_filename);

  // compile
  char* compile_command;
  asprintf(&compile_command, "gcc -std=gnu99 -g3 -D_GNU_SOURCE -pthread -lm -O2 -Wall -Werror %s -o %s/%s",
           c_filename, output_directory, app_name);
  system(compile_command);

  return 0;
}

