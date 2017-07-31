
#include "common.hpp"

int main(int argc, char** argv) __attribute__((const));
int main(int argc, char** argv)
{
  (void)(argc && argv); // TODO
  ASSERT(false);
  return 0;
}

