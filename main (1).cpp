#include "lexer.hpp"
#include <iostream>

int main() {
  std::string input;
  getline(std::cin, input);
  std::vector<Token> tokens = Lexer::parse(input);
  for (auto it : tokens) {
    std::cout << it.value << " ";
    std::cout << '\n';
  }
}
