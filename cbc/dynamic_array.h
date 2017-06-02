#pragma once

#define CBC_DYN_ARRAY(name, type) \
        struct { \
          type*  array; \
          size_t count; \
          size_t capacity; \
        } name

#define CBC_DYN_ARRAY_INIT(name, init_capacity) \
        do { \
          typeof(&name)* da_ = &(name); \
          da_->array    = malloc(init_capacity * sizeof(da_->array[0])); \
          da_->count    = 0; \
          da_->capacity = init_capacity; \
        } while (false)

#define CBC_DYN_ARRAY_UNINIT(name) \
        free((name).array)

#define CBC_DYN_ARRAY_APPEND(name, value) \
        do { \
          typeof(&name)* da_ = &(name); \
          (da_->array[0] == value); \
          if (da_->count == da_->capacity) { \
            da->capacity *= 2; \
            da_->array    = realloc(da->array_, da->capacity * sizeof(da->array_[0])); \
          } \
          da_->array[da_->count++] = value; \
        } while (false)

