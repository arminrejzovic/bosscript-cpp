#include "Parser.h"

Token Parser::expect(TokenType expectedType, const std::string &errorMessage) {
    if(current().type != expectedType){
        throw std::runtime_error(errorMessage);
    }
    Token t = tokens.front();
    tokens.pop();
    return t;
}

Token* Parser::expectPtr(TokenType expectedType, const std::string &errorMessage) {
    if(current().type != expectedType){
        throw std::runtime_error(errorMessage);
    }
    Token& t = tokens.front();
    tokens.pop();
    return &t;
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
        case TokenType::Paket:
            return parseImportStatement();
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
    } while (current().type == TokenType::Comma && expectPtr(TokenType::Comma, "Nedostaje ','") != nullptr);

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
        if(current().type == TokenType::CloseBracket) break;
        auto exp = parseExpression();
        array.emplace_back(std::move(exp));
    } while (current().type == TokenType::Comma && expectPtr(TokenType::Comma, "Nedostaje ','") != nullptr);

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
    } while (current().type == TokenType::Comma && expectPtr(TokenType::Comma, "Expected ,") != nullptr);

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

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement() {
    expect(TokenType::Vrati, "Missing return statement");
    std::unique_ptr<Expression> argument;
    if (current().type != TokenType::Se) {
        argument = parseExpression();
    }
    else {
        expect(TokenType::Se, "Nedostaje ključna riječ 'se'. Povrat bez povratne vrijednosti se piše 'vrati se'");
    }
    expect(TokenType::Semicolon, "Nedostaje ';'");

    return std::make_unique<ReturnStatement>(std::move(argument));
}

std::unique_ptr<FunctionDeclaration> Parser::parseFunctionDeclaration() {
    expect(TokenType::Funkcija, "Nedostaje ključna riječ 'funkcija'");
    auto functionName = parseIdentifier();
    expect(TokenType::OpenParen, "Nedostaje '('");

    std::vector<std::unique_ptr<FunctionParameter>> params;
    if (current().type != TokenType::CloseParen) {
        params = parseFormalParameterList();
    }
    expect(TokenType::CloseParen, "Nedostaje ')'");

    std::unique_ptr<TypeAnnotation> returnType;

    if (current().type == TokenType::Colon) {
        // Non-void return type specified
        consume();
        returnType = parseTypeAnnotation();
    }

    std::unique_ptr<BlockStatement> body;

    if (current().type == TokenType::Arrow) {
        consume();
        std::vector<std::unique_ptr<Statement>> blockBody;
        blockBody.emplace_back(parseExpression());
        body = std::make_unique<BlockStatement>(std::move(blockBody));
    }
    else {
        body = parseBlockStatement();
    }

    return std::make_unique<FunctionDeclaration>(
            std::move(functionName),
            std::move(params),
            std::move(returnType),
            std::move(body)
    );
}

std::vector<std::unique_ptr<FunctionParameter>> Parser::parseFormalParameterList() {
    std::vector<std::unique_ptr<FunctionParameter>> params;

    do {
        auto name = parseIdentifier();
        std::cout<<name->symbol<<std::endl;
        std::unique_ptr<TypeAnnotation> type;
        if(current().type == TokenType::Colon){
            consume();
            type = parseTypeAnnotation();
        }
        params.emplace_back(std::make_unique<FunctionParameter>(std::move(name), std::move(type)));
    } while (current().type == TokenType::Comma && (expectPtr(TokenType::Comma, "Missing comma") != nullptr));

    return params;
}

std::unique_ptr<FunctionExpression> Parser::parseFunctionExpression() {
    expect(TokenType::Funkcija, "Deklaracija funkcije počinje sa 'funkcija'");
    expect(TokenType::OpenParen, "Nedostaje (");

    std::vector<std::unique_ptr<FunctionParameter>> params;
    if (current().type != TokenType::CloseParen) {
        params = parseFormalParameterList();
    }
    expect(TokenType::CloseParen, "Nedostaje ')'");

    std::unique_ptr<TypeAnnotation> returnType;

    if (current().type == TokenType::Colon) {
        // Non-void return type specified
        consume();
        returnType = parseTypeAnnotation();
    }

    std::unique_ptr<BlockStatement> body;

    if (current().type == TokenType::Arrow) {
        consume();
        std::vector<std::unique_ptr<Statement>> blockBody;
        blockBody.emplace_back(parseExpression());
        body = std::make_unique<BlockStatement>(std::move(blockBody));
    }
    else {
        body = parseBlockStatement();
    }

    return std::make_unique<FunctionExpression>(
            params,
            std::move(returnType),
            std::move(body)
    );
}

std::unique_ptr<TryCatchStatement> Parser::parseTryCatchStatement() {
    expect(TokenType::Try, "Nedostaje ključna riječ 'probaj'");
    auto tryBlock = parseBlockStatement();
    expect(TokenType::Catch, "Nedostaje 'probaj' blok");
    auto catchBlock = parseBlockStatement();
    std::unique_ptr<BlockStatement> finallyBlock;
    if (current().type == TokenType::Finally) {
        consume(/* finally */);
        finallyBlock = parseBlockStatement();
    }
    return std::make_unique<TryCatchStatement>(
            std::move(tryBlock),
            std::move(catchBlock),
            std::move(finallyBlock)
    );
}

std::unique_ptr<ImportStatement> Parser::parseImportStatement() {
    expect(TokenType::Paket, "Nedostaje ključna riječ 'paket'");
    auto packageName = parseStringLiteral();
    std::vector<std::unique_ptr<Identifier>> imports;
    if (current().type == TokenType::Semicolon) {
        // Full package import
        consume(/*semicolon*/);
        return std::make_unique<ImportStatement>(
                packageName->value,
                std::move(imports)
        );
    }

    expect(TokenType::OpenBrace, "Nedostaje '{'");
    do {
        imports.emplace_back(parseIdentifier());
    } while (current().type != TokenType::CloseBrace && expectPtr(TokenType::Comma, "Nedostaje zarez ',' između članova paketa") !=
                                                                       nullptr);

    expect(TokenType::CloseBrace, "Nedostaje '}' na kraju liste članova paketa");
    expect(TokenType::Semicolon, "Nedostaje ;");

    return std::make_unique<ImportStatement>(
            packageName->value,
            std::move(imports)
    );
}

std::unique_ptr<TypeDefinitionStatement> Parser::parseTypeDefinitionStatement() {
    expect(TokenType::Tip, "Nedostaje ključna riječ tip na početku deklaracije novog tipa");
    auto name = parseIdentifier();
    std::unique_ptr<Identifier> parentType;
    if (current().value == "<") {
        // Inheritance
        consume(/* < */);
        parentType = parseIdentifier();
    }

    expect(TokenType::OpenBrace, " Nedostaje '{'. Deklaracija tipa je ograničena vitičastim zagradama.");
    std::vector<std::unique_ptr<TypeProperty>> properties;

    while (current().type != TokenType::EndOfFile && current().type != TokenType::CloseBrace) {
        properties.emplace_back(parseTypeProperty());
    }
    expect(TokenType::CloseBrace, "Nedostaje }");

    if (properties.empty()) {
        warning("Tip " + name->symbol + " je prazan");
    }

    return std::make_unique<TypeDefinitionStatement>(
            std::move(name),
            std::move(parentType),
            std::move(properties)
    );
}

std::unique_ptr<TypeProperty> Parser::parseTypeProperty() {
    std::string name = expect(TokenType::Identifier, "").value;

    expect(TokenType::Colon, "Nedostaje :");
    auto type = parseTypeAnnotation();
    expect(TokenType::Semicolon, "Nedostaje ;");

    return std::make_unique<TypeProperty>(
            std::move(name),
            std::move(type)
    );
}

std::unique_ptr<WhileStatement> Parser::parseWhileStatement() {
    expect(TokenType::Dok, "Nedostaje 'dok'");
    expect(TokenType::OpenParen, "Nedostaje '('");
    auto condition = parseExpression();
    expect(TokenType::CloseParen, "Nedostaje ')'");

    // Same rules like in for-loop (shorthand and full loop)
    std::vector<std::unique_ptr<Statement>> blockBody;
    auto loopBody = std::make_unique<BlockStatement>(std::move(blockBody));

    if (current().type == TokenType::Arrow) {
        // Shorthand syntax
        consume(/*Arrow*/);
        loopBody->body.emplace_back(parseStatement());
    }
    else {
        loopBody = parseBlockStatement();
    }

    return std::make_unique<WhileStatement>(
            std::move(condition),
            std::move(loopBody)
    );
}

std::unique_ptr<DoWhileStatement> Parser::parseDoWhileStatement() {
    expect(TokenType::Radi, "Expected 'radi'");
    auto body = parseBlockStatement();
    expect(TokenType::Dok, "Expected 'dok' after 'radi'");
    expect(TokenType::OpenParen, "Expected '('");
    auto condition = parseExpression();
    expect(TokenType::CloseParen, "Expected ')'");
    expect(TokenType::Semicolon, "Missing ';'");

    return std::make_unique<DoWhileStatement>(
            std::move(condition),
            std::move(body)
    );
}

std::unique_ptr<ForStatement> Parser::parseForStatement() {
    expect(TokenType::Za, "Expected 'za'");
    expect(TokenType::Svako, "Missing 'svako' following 'za'");
    expect(TokenType::OpenParen, "Expected '('");
    auto counter = parseIdentifier();
    expect(TokenType::Od, "Expected starting condition for loop, missing keyword 'od'");
    auto startCondition = parseExpression();
    expect(TokenType::Do, "Expected ending condition for loop, missing keyword 'do'");
    auto endCondition = parseExpression();
    std::unique_ptr<Expression> step;
    if (current().type == TokenType::Korak) {
        consume(/*korak*/);
        step = parseExpression();
    }
    expect(TokenType::CloseParen, "Expected ')'");

    // For Statement body must be Block Statement
    // But shorthand syntax for single-line for loops is allowed: za svako(...) => ispis();
    // Shorthand syntax is parsed as BlockStatement(body=<the single expression>)
    std::vector<std::unique_ptr<Statement>> blockBody;
    auto loopBody = std::make_unique<BlockStatement>(std::move(blockBody));

    if (current().type == TokenType::Arrow) {
        // Shorthand syntax
        consume(/*Arrow*/);
        loopBody->body.emplace_back(parseStatement());
    } else {
        // Regular loop
        loopBody = parseBlockStatement();
    }

    return std::make_unique<ForStatement>(
            std::move(counter),
            std::move(startCondition),
            std::move(endCondition),
            std::move(step),
            std::move(loopBody)
    );
}

std::unique_ptr<BreakStatement> Parser::parseBreakStatement() {
    expect(TokenType::Break, "Expected 'prekid'");
    return std::make_unique<BreakStatement>();
}

std::unique_ptr<IfStatement> Parser::parseIfStatement() {
    expect(TokenType::Ako, "Expected 'ako'");
    expect(TokenType::OpenParen, "Expected '('");
    auto condition = parseExpression();
    expect(TokenType::CloseParen, "Expected ')'");
    auto consequent = parseStatement();

    std::unique_ptr<Statement> alternate;
    if (current().type == TokenType::Ili) {
        consume();
        alternate = parseIfStatement();
    } else if (current().type == TokenType::Inace) {
        consume();
        alternate = parseStatement();
    }

    return std::make_unique<IfStatement>(
            std::move(condition),
            std::move(consequent),
            std::move(alternate)
    );
}

std::unique_ptr<UnlessStatement> Parser::parseUnlessStatement() {
    expect(TokenType::Osim, "Expected 'osim'");
    expect(TokenType::Ako, "Expected 'ako'");
    expect(TokenType::OpenParen, "Expected '('");
    auto condition = parseExpression();
    expect(TokenType::CloseParen, "Expected ')'");
    auto consequent = parseStatement();

    std::unique_ptr<Statement> alternate;
    if (current().type == TokenType::Inace) {
        consume();
        alternate = parseStatement();
    }

    return std::make_unique<UnlessStatement>(
            std::move(condition),
            std::move(consequent),
            std::move(alternate)
    );
}

std::unique_ptr<ModelDefinitionStatement> Parser::parseModelDefinitionStatement() {
    expect(TokenType::Model, "Expected 'model'");
    auto classname = parseIdentifier();
    std::unique_ptr<Identifier> parentClassName;
    std::unique_ptr<ModelBlock> privateBlock;
    std::unique_ptr<ModelBlock> publicBlock;
    std::unique_ptr<FunctionDeclaration> constructor;

    if (current().value == "<") {
        consume();
        parentClassName = parseIdentifier();
    }

    expect(TokenType::OpenBrace, "Expected '{'");

    while(current().type != TokenType::CloseBrace){
        if(current().type == TokenType::Constructor && !constructor){
            consume();
            expect(TokenType::OpenParen, "Expected (");

            std::vector<std::unique_ptr<FunctionParameter>> params;

            if (current().type != TokenType::CloseParen) {
                params = parseFormalParameterList();
            }

            expect(TokenType::CloseParen, "Missing ')'");

            auto functionBody = parseBlockStatement();

            constructor = std::make_unique<FunctionDeclaration>(
                    std::move(std::make_unique<Identifier>("konstruktor")),
                    std::move(params),
                    nullptr,
                    std::move(functionBody)
            );
        }
        else if (current().type == TokenType::Private && !privateBlock) {
            consume();
            privateBlock = parseModelBlock();
        }
        else if (current().type == TokenType::Public && !publicBlock) {
            consume();
            publicBlock = parseModelBlock();
        }
        else {
            throw std::runtime_error("Expecting private or public block, or constructor");
        }
    }

    expect(TokenType::CloseBrace, "Expected '}'");

    if(!constructor){
        throw std::runtime_error("Nedostaje konstruktor. Model mora imati konstruktor!");
    }

    return std::make_unique<ModelDefinitionStatement>(
            std::move(classname),
            std::move(parentClassName),
            std::move(constructor),
            std::move(privateBlock),
            std::move(publicBlock)
    );

    throw std::runtime_error("NOT IMPLEMENTED");
}

std::unique_ptr<ModelBlock> Parser::parseModelBlock() {
    expect(TokenType::OpenBrace, "Expected '{'");
    std::unique_ptr<ModelBlock> modelBlock;
    while (current().type != TokenType::CloseBrace) {
        modelBlock->addStatement(parseStatement());
    }
    expect(TokenType::CloseBrace, "Expected '}'");
    return modelBlock;
}
