#include <cctype>
#include <string>

class Lexer::Alphabet {
public:
  static bool isSpace(const char &c) { return std::isspace(c); }

  static bool isLiteral(const char &c) { return std::isalpha(c) || c == '_'; }

  static bool isDigit(const char &c) { return std::isdigit(c); }

  static bool isAlphanumeric(const char &c) {
    return isLiteral(c) || isDigit(c);
  }

  static bool isMetachar(const char &c) {
    std::string metachars = "+-*/%=!|;{}()[].&";
    return metachars.find(c) != std::string::npos;
  }

  static bool isDot(const char &c) { return c == '.'; }
};
