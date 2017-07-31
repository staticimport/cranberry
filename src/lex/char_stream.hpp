#pragma once

#include "common.hpp"

namespace CBC {

/* functionality */
class CharStream
{
public:
  CharStream(std::istream&);
  CharStream(char const*);
  ~CharStream();

  bool    CanPop(void) const;
  bool    Peek(char* out) const;
  char    Pop(void);
  void    Push(void);
  size_t  TotalSize(void) const;
  bool    TryPop(char);
private:
  char* mBuffer;
  char* mHead;
  char* mTail;
};


/* implementation */
CharStream::CharStream(std::istream& in)
  : mBuffer()
{
  size_t old_capacity = 0;
  do {
    size_t capacity = (2 * old_capacity) + 8192; // arbitrary
    mBuffer = static_cast<char*>(realloc(mBuffer, capacity));
    mHead   = mBuffer;
    mTail   = mHead + old_capacity;
    in.read(mTail, capacity - old_capacity);
    mTail  += in.gcount(); // number of bytes returned by last read()
    old_capacity = capacity;
  } while (in.good());
}

CharStream::CharStream(char const* s)
  : mBuffer()
{
  size_t const len = strlen(s);
  mBuffer = static_cast<char*>(malloc(len));
  memcpy(mBuffer, s, len);
  mHead   = mBuffer;
  mTail   = mBuffer + len;
}

CharStream::~CharStream()
{
  free(mBuffer);
}

bool CharStream::CanPop(void) const
{
  return mHead < mTail;
}

bool CharStream::Peek(char* out) const
{
  if (CanPop()) { *out = *mHead; return true; }
  else          { return false; }
}

char CharStream::Pop(void)
{
  ASSERT(CanPop(), "CharStream::Pop() called at end of file");
  return *(mHead++);
}

void CharStream::Push(void)
{
  ASSERT(mHead > mBuffer);
  mHead -= 1;
}

bool CharStream::TryPop(char c)
{
  if (CanPop() and *mHead == c) { mHead++; return true; }
  else                          { return false; }
}

size_t CharStream::TotalSize(void) const
{
  return mTail - mBuffer;
}

} // namespace CBC
