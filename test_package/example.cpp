#include <iostream>
#include <sstream>

int main() {
  std::stringstream ss;

  ss << "Do Testing...";

  std::cout << ss.str() << std::endl;
}
