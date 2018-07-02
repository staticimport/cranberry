#pragma once

#include "cbc/ast/types.h"

void cbc_cgen(struct cbc_ast const*, char const* filename);


/* implementation */
void cbc_cgen(struct cbc_ast const* ast, char const* filename)
{
  FILE* f = fopen(filename, "w+");

  // default includes
  fprintf(f, "#include <stdarg.h>\n"
             "#include <stdbool.h>\n"
             "#include <stddef.h>\n"
             "#include <stdint.h>\n"
             "#include <stdio.h>\n"
             "#include <string.h>\n"
             "\n\n");

  // write main function
  fprintf(f, "int main(void) {\n"
             "  return %.*s;\n"
             "}\n",
          (int)ast->main_function.return_value->textlen,
          ast->main_function.return_value->text);

  // done
  fclose(f);
}
