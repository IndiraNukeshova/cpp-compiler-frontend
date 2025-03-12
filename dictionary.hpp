class Lexer::Dictionary {
public:
  static bool isValidMetasequence(const std::string &);
  static Token::TokenType getKeywordType(const std::string &);
  static const std::unordered_map<std::string, Token::TokenType>
      metaSequenceAssociations;
  static const std::unordered_map<std::string, Token::TokenType>
      keywordAssociations;
};

bool Lexer::Dictionary::isValidMetasequence(const std::string &token) {
  return metaSequenceAssociations.find(token) != metaSequenceAssociations.end();
}

Token::TokenType Lexer::Dictionary::getKeywordType(const std::string &keyword) {
  if (keywordAssociations.find(keyword) != keywordAssociations.end()) {
    return keywordAssociations.at(keyword);
  }

  return Token::IDENTIFIER;
}

const std::unordered_map<std::string, Token::TokenType>
    Lexer::Dictionary::metaSequenceAssociations = {
        {"+", Token::PLUS},        {"-", Token::MINUS},
        {"*", Token::MULTIPLY},    {"/", Token::DIVIDE},
        {"%", Token::MODULO},      {"=", Token::ASSIGN},
        {"!", Token::EXCLAMATION}, {"|", Token::PIPE},
        {";", Token::SEMICOLON},   {"{", Token::LBRACE},
        {"}", Token::RBRACE},      {"(", Token::LPAREN},
        {")", Token::RPAREN},      {"[", Token::LBRACKET},
        {"]", Token::RBRACKET},    {"&", Token::AMPERSAND}};

const std::unordered_map<std::string, Token::TokenType>
    Lexer::Dictionary::keywordAssociations = {
        {"int", Token::INT},     {"double", Token::DOUBLE},
        {"float", Token::FLOAT}, {"string", Token::STRING},
        {"char", Token::CHAR},   {"if", Token::IF},
        {"elif", Token::ELIF},   {"else", Token::ELSE},
        {"while", Token::WHILE}, {"main", Token::MAIN}};
