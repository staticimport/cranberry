#pragma once

struct cbc_buffer
{
  size_t capacity;
  char*  array;
  char*  head;
  char*  tail;
};

void  cbc_buffer_init(struct cbc_buffer*);
void  cbc_buffer_uninit(struct cbc_buffer*);
char* cbc_buffer_read_begin (struct cbc_buffer*, size_t*);
void  cbc_buffer_read_end   (struct cbc_buffer*, size_t);
char* cbc_buffer_write_begin(struct cbc_buffer*, size_t);
void  cbc_buffer_write_end  (struct cbc_buffer*, size_t);


/* implementation */
void  cbc_buffer_init(struct cbc_buffer* b)
{
  b->capacity = 1024;
  b->array    = malloc(b->capacity);
  b->head     = b->array;
  b->tail     = b->array;
}

void  cbc_buffer_uninit(struct cbc_buffer*);
char* cbc_buffer_read_begin (struct cbc_buffer*, size_t*);
void  cbc_buffer_read_end   (struct cbc_buffer*, size_t);
char* cbc_buffer_write_begin(struct cbc_buffer*, size_t);
void  cbc_buffer_write_end  (struct cbc_buffer*, size_t);
