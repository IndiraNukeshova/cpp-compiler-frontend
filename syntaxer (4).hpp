#include <memory>
class Syntaxer {
public:
    static std::unique_ptr<Node> parse(const std::vector<Token>&);

private:
    static std::unique_ptr<Node> parse_program(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_declaration(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_variable_declaration(const std::vector<Token>& , std::size_t&);
    static std::unique_ptr<Node> parse_struct_declaration(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_function_declaration(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_function_prototype(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_function_definition(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_block(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_statement(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_conditional(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_loop(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_return(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_jump(const std::vector<Token>&, std::size_t&);
    static std::unique_ptr<Node> parse_type(const std::vector<Token>&, std::size_t&);

    static void expect_token(const Token::TokenType& expected_type, const std::vector<Token>&, std::size_t&);
};

//начинаем парсить, задаем pos=0, и переходим в парсинг программы
std::unique_ptr<Node> Syntaxer::parse(const std::vector<Token>& tokens) {
    std::size_t pos = 0;
    return parse_program(tokens, pos);
}

//парсим программу, main () block
std::unique_ptr<Node> Syntaxer::parse_program(const std::vector<Token>& tokens) {
    std::size_t pos = 0;
    expect_token(TokenType::MAIN, tokens, pos);
    expect_token(TokenType::LPAREN, tokens, pos);
    expect_token(TokenType::RPAREN, tokens, pos);
    auto blockNode = parse_block(tokens, pos);

    return std::make_unique<Node>(Node::Program, "", std::move(blockNode));
}

//парсинг блока {}
std::unique_ptr<Node> Syntaxer::parse_block(const std::vector<Token>& tokens, std::size_t& pos) {
    expect_token(TokenType::LBRACE, tokens, pos);

    auto blockNode = std::make_unique<Node>(Node::Block, "");

    while (tokens[pos].type != TokenType::RBRACE) {
        auto statementNode = parse_statement(tokens, pos);
        if (statementNode) {
            blockNode.emplace_back(std::move(statementNode));
        }
    }

    expect_token(TokenType::RBRACE, tokens, pos);

    return blockNode;
}

//парсим type-объявление, struct - объявление структуры , какой-то идентификатор - объявление функции
std::unique_ptr<Node> Syntaxer::parse_declaration(const std::vector<Token>& tokens, std::size_t& pos) {
    auto token = tokens[pos].type;
    if (token == TokenType::INT || token == TokenType::DOUBLE || token == TokenType::FLOAT ||
        token == TokenType::STRING || token == TokenType::CHAR) {
        return parse_variable_declaration(tokens, pos);
    } else if (token == TokenType::IDENTIFIER) {
        return parse_function_declaration(tokens, pos);
    } else if (token == TokenType::STRUCT) {
        return parse_struct_declaration(tokens, pos);
    }
    return nullptr;
}



//парсинг объявление переменной
std::unique_ptr<Node> Syntaxer::parse_variable_declaration(const std::vector<Token>& tokens, std::size_t& pos) {
    auto typeNode = parse_type(tokens, pos);
    expect_token(TokenType::IDENTIFIER, tokens, pos);
    std::string variableName = tokens[pos - 1].value;

    if (tokens[pos].type == TokenType::ASSIGN) {
        pos++;
        auto expressionNode = parse_expression(tokens, pos);
        expect_token(TokenType::SEMICOLON, tokens, pos);

        return std::make_unique<Node>(Node::VariableDeclaration, variableName, std::move(typeNode), std::move(expressionNode));
    } else {
        expect_token(TokenType::SEMICOLON, tokens, pos);

        return std::make_unique<Node>(Node::VariableDeclaration, variableName, std::move(typeNode));
    }
}


//парсинг объявления структуры
std::unique_ptr<Node> Syntaxer::parse_struct_declaration(const std::vector<Token>& tokens, std::size_t& pos) {
    expect_token(TokenType::STRUCT, tokens, pos);
    expect_token(TokenType::IDENTIFIER, tokens, pos);
    std::string structName = tokens[pos - 1].value;

    expect_token(TokenType::LBRACE, tokens, pos);

    std::vector<std::unique_ptr<Node>> declarations;
    while (tokens[pos].type != TokenType::RBRACE) {
        auto declaration = parse_declaration(tokens, pos);
        declarations.emplace_back(std::move(declaration));

        if (pos >= tokens.size()) {
            throw std::runtime_error("Unexpected end of input");
        }
    }

    expect_token(TokenType::RBRACE, tokens, pos);
    expect_token(TokenType::SEMICOLON, tokens, pos);

    return std::make_unique<Node>(Node::structdecl, structName, std::move(declarations));
}

//парсинг объявления функции
std::unique_ptr<Node> Syntaxer::parse_function_declaration(const std::vector<Token>& tokens, std::size_t& pos) {
    auto prototypeNode = parse_function_prototype(tokens, pos);
    expect_token(TokenType::SEMICOLON, tokens, pos);

    return std::make_unique<Node>(Node::funcdecl, "", std::move(prototypeNode));
}

//парсинг прототипа функции type funcName (type paramName, ... ) 
std::unique_ptr<Node> Syntaxer::parse_function_prototype(const std::vector<Token>& tokens, std::size_t& pos) {
    auto returnTypeNode = parse_type(tokens, pos);
    expect_token(TokenType::IDENTIFIER, tokens, pos);
    std::string functionName = tokens[pos - 1].value;
    expect_token(TokenType::LPAREN, tokens, pos);

    std::vector<std::pair<std::string, std::string>> parameters;
    while (tokens[pos].type != TokenType::RPAREN) {
        auto paramType = parse_type(tokens, pos);
        expect_token(TokenType::IDENTIFIER, tokens, pos);
        std::string paramName = tokens[pos - 1].value;
        parameters.emplace_back(paramType, paramName);

        if (tokens[pos].type == TokenType::COMMA) {
            pos++;
        } else {
            break;
        }
    }

    expect_token(TokenType::RPAREN, tokens, pos);

    return std::make_unique<Node>(Node::FunctionPrototype, functionName, std::move(returnTypeNode), std::move(parameters));
}

//парсинг определения функции type funcName (type paramName, ... ) block
std::unique_ptr<Node> Syntaxer::parse_function_definition(const std::vector<Token>& tokens, std::size_t& pos) {
    auto prototypeNode = parse_function_prototype(tokens, pos);
    auto blockNode = parse_block(tokens, pos);

    return std::make_unique<Node>(Node::FunctionDefinition, "", std::move(prototypeNode), std::move(blockNode));
}

//парсинг стейтментов - объявления, условия, циклы, ретерн, джамп, 
std::unique_ptr<Node> Syntaxer::parse_statement(const std::vector<Token>& tokens, std::size_t& pos) {
    auto token = tokens[pos].type;
    if (token == TokenType::INT || token == TokenType::DOUBLE || token == TokenType::FLOAT ||
        token == TokenType::STRING || token == TokenType::CHAR) {
        return parse_variable_declaration(tokens, pos);
    } else if (token == TokenType::IF) {
        return parse_conditional(tokens, pos);
    } else if (token == TokenType::WHILE) {
        return parse_loop(tokens, pos);
    } else if (token == TokenType::RETURN) {
        return parse_return(tokens, pos);
    } else if (token == TokenType::BREAK || token == TokenType::CONTINUE) {
        return parse_jump(tokens, pos);
    } else if (token == TokenType::IDENTIFIER || token == TokenType::NUMBER ||
               token == TokenType::STRING_LITERAL || token == TokenType::CHAR_LITERAL) {
        return parse_expression(tokens, pos);
    }

    return nullptr;
}

//парсинг условия if () block elif () block
std::unique_ptr<Node> Syntaxer::parse_conditional(const std::vector<Token>& tokens, std::size_t& pos) {
    expect_token(TokenType::IF, tokens, pos);
    expect_token(TokenType::LPAREN, tokens, pos);
    auto conditionNode = parse_expression(tokens, pos);
    expect_token(TokenType::RPAREN, tokens, pos);
    auto ifBlockNode = parse_block(tokens, pos);

    std::vector<std::pair<std::unique_ptr<Node>, std::unique_ptr<Node>>> elifBlocks;
    while (tokens[pos].type == TokenType::ELIF) {
        expect_token(TokenType::ELIF, tokens, pos);
        expect_token(TokenType::LPAREN, tokens, pos);
        auto elifConditionNode = parse_expression(tokens, pos);
        expect_token(TokenType::RPAREN, tokens, pos);
        auto elifBlockNode = parse_block(tokens, pos);
        elifBlocks.emplace_back(std::move(elifConditionNode), std::move(elifBlockNode));
    }

    std::unique_ptr<Node> elseBlockNode;
    if (tokens[pos].type == TokenType::ELSE) {
        expect_token(TokenType::ELSE, tokens, pos);
        elseBlockNode = parse_block(tokens, pos);
    }

    return std::make_unique<Node>(Node::Conditional, "", std::move(conditionNode), std::move(ifBlockNode), std::move(elifBlocks), std::move(elseBlockNode));
}

  //парсинг цикла while () block
std::unique_ptr<Node> Syntaxer::parse_loop(const std::vector<Token>& tokens, std::size_t& pos) {
    expect_token(TokenType::WHILE, tokens, pos);
    expect_token(TokenType::LPAREN, tokens, pos);
    auto conditionNode = parse_expression(tokens, pos);
    expect_token(TokenType::RPAREN, tokens, pos);
    auto loopBlockNode = parse_block(tokens, pos);

    return std::make_unique<Node>(Node::Loop, "", std::move(conditionNode), std::move(loopBlockNode));
}

//парсинг ретерн return expression ;
std::unique_ptr<Node> Syntaxer::parse_return(const std::vector<Token>& tokens, std::size_t& pos) {
    expect_token(TokenType::RETURN, tokens, pos);
    auto expressionNode = parse_expression(tokens, pos);
    expect_token(TokenType::SEMICOLON, tokens, pos);

    return std::make_unique<Node>(Node::Return, "", std::move(expressionNode));
}

 // парсинг break, continue ;
std::unique_ptr<Node> Syntaxer::parse_jump(const std::vector<Token>& tokens, std::size_t& pos) {
    auto token = tokens[pos].type;
    if (token == TokenType::BREAK) { //break, continue  - должен быть в контексте цикла ( вне цикла ошибка)
        expect_token(TokenType::BREAK, tokens, pos);
    } else if (token == TokenType::CONTINUE) {
        expect_token(TokenType::CONTINUE, tokens, pos);
    }
    expect_token(TokenType::SEMICOLON, tokens, pos);

    return std::make_unique<Node>(Node::Jump, "");
}

//int, float ...
std::unique_ptr<Node> Syntaxer::parse_type(const std::vector<Token>& tokens, std::size_t& pos) {
    if (pos < tokens.size()) {
        TokenType tokenType = tokens[pos].type;
        std::string tokenValue = tokens[pos].value;

        if (tokenType == TokenType::INT || tokenType == TokenType::DOUBLE || tokenType == TokenType::FLOAT ||
            tokenType == TokenType::STRING || tokenType == TokenType::CHAR) {
            pos++;
            return std::make_unique<Node>(tokenType, tokenValue);
        } else if (tokenType == TokenType::IDENTIFIER) {
            pos++;
            return std::make_unique<Node>(TokenType::IDENTIFIER, tokenValue);
        }
    }

    throw std::runtime_error("Invalid type");
}

// pos увеличивается , когда токен соответствует ожидаемому типу. В противном случае - выбрасывается исключение.
void expect_token(const Token::TokenType& expected_type, const std::vector<Token>& tokens, std::size_t& pos) {
    if (pos < tokens.size() && tokens[pos].type == expected_type) {
        pos++;
    } else {
        throw std::runtime_error("Unexpected token");
    }
}
