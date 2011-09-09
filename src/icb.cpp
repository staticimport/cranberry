
#include <iostream>

static const std::string QUIT_COMMAND = ":q";

int main(void)
{
  while (true) {
    std::string line;
    std::getline(std::cin, line);
    if (line == QUIT_COMMAND) {
      break;
    }
    std::cout << "You entered: " << line << std::endl;
  }
  return 0;
}

