
#include "cbc/file_reader.h"
#include "cbc/tokenizer.h"

int main(int argc, char** argv)
{
  // args
  if (argc != 2)
  {
    fprintf(stderr, "USAGE: %s <filename>\n", argv[0]);
    return 1;
  }
  char* const filename = argv[1];

  struct cbc_tokenizer tokenizer; cbc_tokenizer_init(&tokenizer);
  struct cbc_file_reader file_reader; cbc_file_reader_init(&file_reader);
  cbc_file_reader_load(&file_reader, filename);
  cbc_tokenizer_start(&tokenizer, &file_reader);

  struct cbc_token token;
  while (cbc_tokenizer_next(&tokenizer, &token))
  {
    printf("text(%.*s) type(%c) location(%s:%d,%d)\n", token.textlen, token.text, token.type,
           CURRENT_FILENAME, CURRENT_LINENUM, CURRENT_COLNUM);
  }

  return 0;
}

