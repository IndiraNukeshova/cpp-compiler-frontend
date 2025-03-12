#include <string>
#include <vector>
#include <unordered_map>

struct Token {
	enum TokenType {
		NUMBER, IDENTIFIER, METACHARS, KEYWORDS, STRING_LITERALS, CHAR_LITERALS, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO, ASSIGN, EXCLAMATION, PIPE, SEMICOLON, LBRACE, RBRACE, LPAREN, RPAREN, LBRACKET, RBRACKET, AMPERSAND, INT, DOUBLE, FLOAT, STRING, CHAR, IF, ELIF, ELSE, WHILE, MAIN
	};

	TokenType type;
	std::string value;

	Token(const TokenType&, const std::string&);
};

Token::Token(const TokenType& type, const std::string& value) : type(type), value(value) {}

class Lexer {
public:
	static std::vector<Token> parse(const std::string&);

private:
	class Alphabet;
	class Dictionary;
	static Token extract_number(const std::string&, std::size_t&);
	static Token extract_identifier(const std::string&, std::size_t&);
	static Token extract_metachar(const std::string&, std::size_t&);
	static Token extract_string_literal(const std::string&, std::size_t&);
	static Token extract_char_literal(const std::string&, std::size_t&);

};

#include "alphabet.hpp"
#include "dictionary.hpp"

std::vector<Token> Lexer::parse(const std::string& input) {
	std::vector<Token> tokens;
	std::size_t pos = 0;
	const std::size_t len = input.length();

	while (pos < len) {
		const char c = input[pos];
		if (std::isdigit(c)) {
			tokens.push_back(extract_number(input, pos));
		} else if (std::isalpha(c) || c == '_') {
			tokens.push_back(extract_identifier(input, pos));
		} else if (c == '"' || c == '\'') {
			tokens.push_back(extract_string_literal(input, pos));
		} else if (Dictionary::metaSequenceAssociations.find(std::string(1, c)) != Dictionary::metaSequenceAssociations.end()) {
			tokens.push_back({ Dictionary::metaSequenceAssociations.at(std::string(1, c) ), std::string(1, c) });
			++pos;
		} else if (std::isspace(c)) {
			++pos;
		} else {
			throw std::runtime_error("Invalid character");
		}
	}

	return tokens;
}

Token Lexer::extract_number(const std::string& input, std::size_t& pos) {
	const std::size_t len = input.length();
	std::string number;

	while (pos < len && std::isdigit(input[pos])) {
		number.push_back(input[pos]);
		++pos;
	}

	return { Token::NUMBER, number };
}

Token Lexer::extract_identifier(const std::string& input, std::size_t& pos) {
	const std::size_t len = input.length();
	std::string identifier;

	while (pos < len && (std::isalnum(input[pos]) || input[pos] == '_')) {
		identifier.push_back(input[pos]);
		++pos;
	}

	if (Dictionary::keywordAssociations.find(identifier) != Dictionary::keywordAssociations.end()) {
		return { Token::KEYWORDS, identifier };
	}

	return { Token::IDENTIFIER, identifier };
}

Token Lexer::extract_metachar(const std::string& input, std::size_t& pos) {
	const std::size_t len = input.length();
	std::string metasequence;
	metasequence.push_back(input[pos]);

	if (pos < len - 1) {
		metasequence.push_back(input[pos + 1]);
	}

	if (Dictionary::metaSequenceAssociations.find(metasequence) != Dictionary::metaSequenceAssociations.end()) {
		++pos;
	}

	return { Token::METACHARS, metasequence };
}

Token Lexer::extract_string_literal(const std::string& input, std::size_t& pos) {
	const std::size_t len = input.length();
	const char delimiter = input[pos];
	std::string value;
	++pos;

	while (pos < len && input[pos] != delimiter) {
		value.push_back(input[pos]);
		++pos;
	}

	if (pos < len && input[pos] == delimiter) {
		++pos;
		return { Token::STRING_LITERALS, value };
	}

	throw std::runtime_error("Unclosed string literal");
}
