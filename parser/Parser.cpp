#include "Parser.h"

Token Parser::expect(TokenType expectedType, const std::string &errorMessage) {
    if(current().type != expectedType){
        throw std::runtime_error(errorMessage);
    }
    Token t = tokens.front();
    tokens.pop();
    return t;
}

std::unique_ptr<Expression> Parser::assertValidAssignmentTarget(std::unique_ptr<Expression> node) {
    if ((node->kind == NodeType::Identifier) || (node->kind == NodeType::MemberExpression)) {
        return node;
    }
    throw std::runtime_error("TODO");
}

std::string Parser::parseAssignmentOperator() {
    if (current().type == TokenType::SimpleAssign || current().type == TokenType::ComplexAssign) {
        return consume().value;
    }
    throw std::runtime_error("Neočekivan token pronađen");
}

void Parser::warning(const std::string &message) {
    const std::string reset = "\033[0m";
    const std::string yellow = "\033[33m";

    std::cout << yellow << "[WARN] " << message << reset << std::endl;
}

Program Parser::parseProgram(const std::string &src) {
    tokens = Lexer::tokenize(src, js);
    return Program(parseStatementList());
}

std::vector<std::unique_ptr<Statement>> Parser::parseStatementList() {
    std::vector<std::unique_ptr<Statement>> statementList;
    while (notEOF()){
        statementList.emplace_back(parseStatement());
    }
    return statementList;
}

std::unique_ptr<Expression> Parser::parsePrimaryExpression() {
    switch (current().type) {
        case TokenType::Number:
            return parseNumericLiteral();
        case TokenType::DoubleQuote:
            return parseStringLiteral();
        case TokenType::OpenParen:
            return parseParenthesizedExpression();
        case TokenType::OpenBracket:
            return parseArrayLiteral();
        case TokenType::OpenBrace:
            return parseObjectLiteral();
        case TokenType::Tacno:
        case TokenType::Netacno:
            return parseBooleanLiteral();
        case TokenType::Nedefinisano:
            return parseNullLiteral();
        case TokenType::Javascript:
            return parseJavascriptSnippet();
        default:
            return parseIdentifier();
    }
}

std::unique_ptr<NumericLiteral> Parser::parseNumericLiteral() {
    return std::make_unique<NumericLiteral>(std::stod(consume().value));
}

std::unique_ptr<StringLiteral> Parser::parseStringLiteral() {
    consume(); //opening double quote
    Token strToken = expect(TokenType::String, "Neočekivani token pronađen. Očekivani token: tekst");
    expect(TokenType::DoubleQuote, "Nedostaju znaci navoda na kraju teksta (\")");
    return std::make_unique<StringLiteral>(strToken.value);
}

std::unique_ptr<BooleanLiteral> Parser::parseBooleanLiteral() {
    return std::make_unique<BooleanLiteral>(consume().type == TokenType::Tacno);
}

std::unique_ptr<NullLiteral> Parser::parseNullLiteral() {
    consume();
    return std::make_unique<NullLiteral>();
}

std::unique_ptr<Identifier> Parser::parseIdentifier() {
    if(current().type == TokenType::This){
        consume(/* @ */);
        return std::make_unique<Identifier>("@");
    }
    std::string identifier = expect(TokenType::Identifier, "Nedostaje očekivani identifikator.").value;
    return std::make_unique<Identifier>(identifier);
}

std::unique_ptr<Statement> Parser::parseStatement() {
    switch (current().type) {
        case TokenType::OpenBrace:
            return parseBlockStatement();
        case TokenType::Semicolon:
            return parseEmptyStatement();
        case TokenType::Var:
        case TokenType::Konst:
            return parseVariableStatement();
        case TokenType::Ako:
            return parseIfStatement();
        case TokenType::Osim:
            return parseUnlessStatement();
        case TokenType::Dok:
            parseWhileStatement();
        case TokenType::Radi:
            parseDoWhileStatement();
        case TokenType::Za:
            parseForStatement();
        case TokenType::Break:
            parseBreakStatement();
        case TokenType::Funkcija:
            parseFunctionDeclaration();
        case TokenType::Vrati:
            return parseReturnStatement();
        case TokenType::Tip:
            return parseTypeDefinitionStatement();
        case TokenType::Model:
            return parseModelDefinitionStatement();
        case TokenType::Try:
            return parseTryCatchStatement();
        default:
            return parseExpressionStatement();
    }
}

std::unique_ptr<JavascriptSnippet> Parser::parseJavascriptSnippet() {
    if(!js){
        throw std::runtime_error(
            "Javascript snippeti nisu dozvoljeni mimo transpajliranja u Javascript. Za transpajliranje u Javascript koristite"
            " komandu 'bosscript <ime_fajla.boss> <ime_fajla.js>'"
        );
    }
    std::string snippet = expect(TokenType::Javascript, "Nedostaje očekivani Javascript snippet").value;
    return std::make_unique<JavascriptSnippet>(snippet);
}

std::unique_ptr<Expression> Parser::parseExpressionStatement() {
    std::unique_ptr<Expression> expression = parseExpression();
    expect(TokenType::Semicolon, "Nedostaje ;");
    return expression;
}

std::unique_ptr<Expression> Parser::parseExpression() {
    if (current().type == TokenType::Funkcija) {
        return parseFunctionExpression();
    }
    return parseAssignmentExpression();
}

std::unique_ptr<Expression> Parser::parseAssignmentExpression() {
    std::unique_ptr<Expression> left = parseLogicalOrExpression();

    if(current().type != TokenType::ComplexAssign && current().type != TokenType::SimpleAssign){
        return left;
    }


    return std::make_unique<AssignmentExpression>(
            assertValidAssignmentTarget(std::move(left)),
            parseAssignmentExpression(),
            parseAssignmentOperator()
    );
}

std::unique_ptr<Expression> Parser::parseLogicalOrExpression() {
    std::unique_ptr<Expression> left = parseLogicalAndExpression();

    while (current().type == TokenType::LogicalOr){
        std::string operator_ = consume().value;
        std::unique_ptr<Expression> right = parseLogicalAndExpression();

        left = std::make_unique<LogicalExpression>(
                std::move(left),
                std::move(right),
                operator_
        );
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseLogicalAndExpression() {
    std::unique_ptr<Expression> left = parseEqualityExpression();

    while (current().type == TokenType::LogicalAnd) {
        std::string operator_ = consume().value;
        std::unique_ptr<Expression> right = parseEqualityExpression();

        left = std::make_unique<LogicalExpression>(
                std::move(left),
                std::move(right),
                operator_
        );
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseEqualityExpression() {
    auto left = parseRelationalExpression();

    while (current().type == TokenType::EqualityOperator) {
        std::string operator_ = consume().value;

        auto right = parseEqualityExpression();
        left = std::make_unique<BinaryExpression>(
                std::move(left),
                std::move(right),
                operator_
        );
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseRelationalExpression() {
    auto left = parseAdditiveExpression();

    while (current().type == TokenType::RelationalOperator) {
        std::string operator_ = consume().value;

        auto right = parseRelationalExpression();
        left = std::make_unique<BinaryExpression>(
                std::move(left),
                std::move(right),
                operator_
        );
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseAdditiveExpression() {
    auto left = parseMultiplicativeExpression();

    while (current().value == "+" || current().value == "-") {
        std::string operator_ = consume().value;

        auto right = parseMultiplicativeExpression();
        left = std::make_unique<BinaryExpression>(
                std::move(left),
                std::move(right),
                operator_
        );
    }
    return left;
}

std::unique_ptr<Expression> Parser::parseMultiplicativeExpression() {
    auto left = parseExponentiationExpression();

    while (current().value == "*" || current().value == "/" || current().value == "%") {
        std::string operator_ = consume().value;

        auto right = parseExponentiationExpression();
        left = std::make_unique<BinaryExpression>(
                std::move(left),
                std::move(right),
                operator_
        );
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseExponentiationExpression() {
    auto left = parseUnaryExpression();

    while (current().type == TokenType::Exponent) {
        std::string operator_ = consume().value;

        auto right = parseUnaryExpression();
        left = std::make_unique<BinaryExpression>(
                std::move(left),
                std::move(right),
                operator_
        );
    }

    return left;
}

std::unique_ptr<Expression> Parser::parseUnaryExpression() {
    std::string operator_;
    const std::vector<std::string> validOperators = {"+", "-", "++", "--", "!"};

    if (std::find(validOperators.begin(), validOperators.end(), current().value) != validOperators.end()) {
        operator_ = consume().value;
    }

    if(!operator_.empty()){
        return std::make_unique<UnaryExpression>(
                operator_,
                parseUnaryExpression()
        );
    }

    return parseLeftHandSideExpression();
}

std::unique_ptr<Expression> Parser::parseLeftHandSideExpression() {
    return parseCallMemberExpression();
}

std::unique_ptr<Expression> Parser::parseCallMemberExpression() {
    auto member = parseMemberExpression();

    if (current().type == TokenType::OpenParen) {
        return parseCallExpression(std::move(member));
    }

    return member;
}

std::unique_ptr<CallExpression> Parser::parseCallExpression(std::unique_ptr<Expression> callee) {
    return std::unique_ptr<CallExpression>();
}

std::unique_ptr<Expression> Parser::parseMemberExpression() {
    auto targetObject = parsePrimaryExpression();
    bool thisExpressionFlag = false;

    // Since @x is a member expression (this.x), but does not require a dot in-between, a special check needs to be made to avoid 'missing dot' error
    if(auto ptr = dynamic_cast<Identifier*>(targetObject.get())){
        if(ptr->symbol == "@" && (current().type == TokenType::Identifier || current().type == TokenType::OpenBracket)){
            thisExpressionFlag = true;
        }
    }

    while (current().type == TokenType::Dot || current().type == TokenType::OpenBracket || current().type == TokenType::OpenParen || thisExpressionFlag){
        if (current().type == TokenType::Dot) {
            consume();
            auto property = parseIdentifier();
            targetObject = std::make_unique<MemberExpression>(
                    false,
                    std::move(targetObject),
                    std::move(property)
            );
        }

        else if (current().type == TokenType::OpenBracket) {
            consume();
            auto property = parseExpression();
            expect(TokenType::CloseBracket, "Nedostaje ']'");

            targetObject = std::make_unique<MemberExpression>(
                    true,
                    std::move(targetObject),
                    std::move(property)
            );
        }

        else if (current().type == TokenType::OpenParen) {
            targetObject = std::make_unique<CallExpression>(
                    parseArguments(),
                    std::move(targetObject)
            );
        }
    }
    return targetObject;
}

std::vector<std::unique_ptr<Expression>> Parser::parseArguments() {
    expect(TokenType::OpenParen, "Nedostaje '(");
    std::vector<std::unique_ptr<Expression>> args;
    if (current().type != TokenType::CloseParen) {
        args = parseArgumentList();
    }

    expect(TokenType::CloseParen, "Nedostaje ')'");
    return args;
}

std::vector<std::unique_ptr<Expression>> Parser::parseArgumentList() {
    std::vector<std::unique_ptr<Expression>> argList;
    do {
        argList.emplace_back(parseExpression());
        expect(TokenType::Comma, "Nedostaje ','");
    } while (current().type == TokenType::Comma);

    return argList;
}

std::unique_ptr<TypeAnnotation> Parser::parseTypeAnnotation() {
    auto typeName = parseIdentifier();
    bool isArray = false;
    if(current().type == TokenType::OpenBracket){
        consume();
        expect(TokenType::CloseBracket, "Nedostaje ]");
        isArray = true;
    }
    return std::make_unique<TypeAnnotation>(
            typeName->symbol,
            isArray
    );
}

std::unique_ptr<ArrayLiteral> Parser::parseArrayLiteral() {
    std::vector<std::unique_ptr<Expression>> array;
    expect(TokenType::OpenBracket, "Nedostaje [");

    do {
        auto exp = parseExpression();
        array.emplace_back(std::move(exp));
        expect(TokenType::Comma, "Nedostaje ','");
    } while (current().type == TokenType::Comma);

    expect(TokenType::CloseBracket, "Nedostaje ]");

    return std::make_unique<ArrayLiteral>(
            std::move(array)
    );
}

std::unique_ptr<ObjectLiteral> Parser::parseObjectLiteral() {
    expect(TokenType::OpenBrace, "Nedostaje {");
    std::vector<std::unique_ptr<ObjectProperty>> properties;

    while (notEOF() && current().type != TokenType::CloseBrace) {
        std::string key = expect(TokenType::Identifier, "Object key expected").value;

        expect(TokenType::Colon, "Nedostaje :");
        auto value = parseExpression();
        properties.emplace_back(std::make_unique<ObjectProperty>(key, std::move(value)));

        if (current().type != TokenType::CloseBrace) {
            expect(TokenType::Comma, "Expected , or }");
        }
    }
    expect(TokenType::CloseBrace, "Expected }");

    return std::make_unique<ObjectLiteral>(
            properties
    );
}

std::unique_ptr<Expression> Parser::parseParenthesizedExpression() {
    expect(TokenType::OpenParen, "Expected '('");
    auto expression = parseExpression();
    expect(TokenType::CloseParen, "Expected ')'");
    return expression;
}

std::unique_ptr<VariableStatement> Parser::parseVariableStatement() {
    Token modifier = consume();
    if(modifier.type != TokenType::Var && modifier.type != TokenType::Konst){
        throw std::runtime_error("Pronađen neočekivan token. Očekivano var ili konst");
    }
    auto declarations = parseVariableDeclarationList();
    return std::make_unique<VariableStatement>(
            std::move(declarations),
            modifier.type == TokenType::Konst
    );
}

std::vector<std::unique_ptr<VariableDeclaration>> Parser::parseVariableDeclarationList() {
    std::vector<std::unique_ptr<VariableDeclaration>> declarations;

    do {
        declarations.emplace_back(parseVariableDeclaration());
        expect(TokenType::Comma, "Expected ,");
    } while (current().type == TokenType::Comma);

    return declarations;
}

std::unique_ptr<VariableDeclaration> Parser::parseVariableDeclaration() {
    auto identifier = parseIdentifier();
    std::unique_ptr<Expression> initializer;

    if (current().type != TokenType::Semicolon && current().type != TokenType::Comma) {
        initializer = parseVariableInitializer();
    }

    return std::make_unique<VariableDeclaration>(
            identifier->symbol,
            std::move(initializer)
    );
}

std::unique_ptr<Expression> Parser::parseVariableInitializer() {
    expect(TokenType::SimpleAssign, "Expected assignment operator");
    return parseExpression();
}

std::unique_ptr<EmptyStatement> Parser::parseEmptyStatement() {
    expect(TokenType::Semicolon, "");
    return std::make_unique<EmptyStatement>();
}

std::unique_ptr<BlockStatement> Parser::parseBlockStatement() {
    expect(TokenType::OpenBrace, "Nedostaje '{'");
    std::vector<std::unique_ptr<Statement>> body;
    while (current().type != TokenType::CloseBrace) {
        body.emplace_back(parseStatement());
    }
    expect(TokenType::CloseBrace, "Nedostaje '}'");
    return std::make_unique<BlockStatement>(std::move(body));
}









