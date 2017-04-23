#pragma once

#include "cbc/common.h"
#include "cbc/trace.h"

struct cbc_file_reader
{
  FILE* file;
  char* buffer;
  char* head;
  char* tail;
};


/* Functionality */
void cbc_file_reader_init(struct cbc_file_reader*);
void cbc_file_reader_uninit(struct cbc_file_reader*);
void cbc_file_reader_load(struct cbc_file_reader*, char const* filename);
bool cbc_file_reader_peek(struct cbc_file_reader*, char* out);
void cbc_file_reader_pop (struct cbc_file_reader*);
bool cbc_file_reader_peek_and_pop(struct cbc_file_reader*, char* out);
void cbc_file_reader_retreat(struct cbc_file_reader*);


/* Implementation */
#define CBC_FILE_READER_BUFLEN 8192
void cbc_file_reader_init(struct cbc_file_reader* fr)
{
  fr->file    = NULL;
  fr->buffer  = (char*)malloc(CBC_FILE_READER_BUFLEN);
  fr->head    = fr->buffer;
  fr->tail    = fr->buffer;
}

void cbc_file_reader_unload(struct cbc_file_reader* fr)
{
  if (fr->file) { fclose(fr->file); fr->file = NULL; }
}


void cbc_file_reader_uninit(struct cbc_file_reader* fr)
{
  cbc_file_reader_unload(fr);
}

void cbc_file_reader_load(struct cbc_file_reader* fr, char const* filename)
{
  cbc_file_reader_unload(fr);
  fr->file = fopen(filename, "r");
  ASSERT(fr->file != NULL, "Unable to open file '%s'", filename);

  CURRENT_FILENAME = filename;
  CURRENT_LINENUM  = 1;
  CURRENT_COLNUM   = 1;
}

bool cbc_file_reader_peek(struct cbc_file_reader* fr, char* out)
{
  // likely there is data
  if (fr->head < fr->tail)  { *out = *fr->head; return true; }

  // read more from file
  if (!fr->file)            { return false; }
  fr->head = fr->buffer;
  fr->tail = fr->buffer + fread(fr->buffer, 1, CBC_FILE_READER_BUFLEN, fr->file);
  if (fr->head < fr->tail)  { *out = *fr->head; return true; }

  // nothing left
  fr->head = fr->tail = fr->buffer;
  cbc_file_reader_unload(fr);
  return false;
}

void cbc_file_reader_pop(struct cbc_file_reader* fr)
{
  fr->head += 1;
}

bool cbc_file_reader_peek_and_pop(struct cbc_file_reader* fr, char* out)
{
  if (cbc_file_reader_peek(fr, out))  { cbc_file_reader_pop(fr); return true; }
  else                                { return false; }
}

void cbc_file_reader_retreat(struct cbc_file_reader* fr)
{
  ASSERT(fr->head > fr->buffer);
  fr->head -= 1;
}

