#include "VnlcParser.hpp"
#include "../ast/declaration/VnlcDeclarationNode.hpp"
#include "../ast/declaration/VnlcExportDeclarationNode.hpp"
#include "../ast/declaration/VnlcImportDeclarationNode.hpp"
#include "../ast/module/VnlcModuleNode.hpp"
#include "../error/VnlcIllegalModuleOrPackageNameError.hpp"
#include "../error/VnlcInternalError.hpp"
#include "../error/VnlcOutOfRangeError.hpp"
#include "inherited/VnlcModuleParsingContext.hpp"
#include "synthesized/VnlcModuleParsingResult.hpp"
#include <memory>
#include <sstream>

VnlcParser::VnlcParser(VnlcLexer&& lexer, unsigned int maxBufferSize) : lexer(std::move(lexer)), tokenBuffer(), currentTokenIndex(0), bufferSize(0) {
    for (unsigned int i = 0; i < maxBufferSize; i++) {
        while (lexer.hasNext() && lexer.next().getType() == VnlcTokenType::BLANK) {
            // Skip blank tokens when filling the initial buffer
        }

        if (lexer.hasNext()) {
            tokenBuffer.push_back(std::move(lexer.next()));
        }
    }

    bufferSize = tokenBuffer.size();
}

bool VnlcParser::hasNextToken() const {
    return lexer.hasNext() || currentTokenIndex < bufferSize;
}

const VnlcToken& VnlcParser::peek(unsigned int offset) const {
    if (currentTokenIndex + offset < bufferSize) {
        return tokenBuffer[currentTokenIndex + offset];
    } else {
        throw VnlcOutOfRangeError("Peek offset exceeds buffer size");
    }
}

const VnlcToken& VnlcParser::peek() const {
    return peek(0);
}

void VnlcParser::fillBuffer() {
    for (unsigned int i = 0; i < bufferSize; i++) {
        while (lexer.hasNext() && lexer.next().getType() == VnlcTokenType::BLANK) {
            // Skip blank tokens when filling the buffer
        }

        if (lexer.hasNext()) {
            tokenBuffer.push_back(std::move(lexer.next()));
        } else {
            bufferSize = tokenBuffer.size();
            break;
        }
    }
}

void VnlcParser::advance() {
    if (currentTokenIndex < bufferSize) {
        currentTokenIndex++;
        if (currentTokenIndex == bufferSize && lexer.hasNext()) {
            fillBuffer();
            currentTokenIndex = 0;
        }
    } else {
        throw VnlcOutOfRangeError("Cannot advance beyond the end of the token buffer");
    }
}

bool VnlcParser::match(VnlcTokenType expectedType) const {
    return hasNextToken() && peek().getType() == expectedType;
}

bool VnlcParser::match(std::span<VnlcTokenType> expectedTypes) const {
    for (unsigned int i = 0; i < expectedTypes.size(); i++) {
        if (peek(i).getType() != expectedTypes[i]) {
            return false;
        }
    }
    return true;
}

std::unique_ptr<VnlcModuleNode> VnlcParser::parse(const VnlcConfig& config) {
    VnlcModuleParsingContext context(config);
    auto result = parseModule(context);

    return std::move(result.moduleNode);
}

VnlcModuleParsingResult VnlcParser::parseModule(VnlcModuleParsingContext context) {
    std::vector<std::unique_ptr<VnlcImportDeclarationNode>> importDeclarations;
    std::vector<std::unique_ptr<VnlcDeclarationNode>> declarations;
    std::vector<std::unique_ptr<VnlcExportDeclarationNode>> exportDeclarations;
    std::string name;
    std::string fullName;

    const VnlcToken& firstToken = peek();

    std::string prefix = context.config.packageRootPath.filename().string();
    std::string fullPath = context.config.inputFilePath.string();

    if (!fullPath.starts_with(prefix)) {
        throw VnlcInternalError("Module file path is outside the package root");
    }
    fullPath.erase(0, prefix.length());

    if (fullPath.ends_with(".vnl")) {
        fullPath.erase(fullPath.length() - 4);
    } else {
        throw VnlcIllegalModuleOrPackageNameError("Module file name must end with .vnl");
    }

    std::stringstream ss(fullPath);
    std::string namePart;
    while (std::getline(ss, namePart, std::filesystem::path::preferred_separator)) {
        if (namePart.empty()) {
            throw VnlcIllegalModuleOrPackageNameError("Module name contains empty part");
        }

        std::stringstream namePartStream(namePart);
        VnlcLexer namePartLexer(namePartStream);
        if (!(namePartLexer.hasNext() && namePartLexer.next().getType() == VnlcTokenType::IDENTIFIER)) {
            throw VnlcIllegalModuleOrPackageNameError(namePart);
        } else if (!(namePartLexer.hasNext() && namePartLexer.next().getType() == VnlcTokenType::END_OF_FILE)) {
            throw VnlcIllegalModuleOrPackageNameError(namePart);
        }

        fullName += namePart + ".";
    }

    if (fullName.ends_with(".")) {
        fullName.pop_back();
    }
    name = fullName.substr(fullName.find_last_of('.') + 1);

    while (match(VnlcTokenType::IMPORT)) {
        auto result = parseImportDeclaration();

        importDeclarations.push_back(std::move(result.declaration));
    }

    while (!match(VnlcTokenType::EXPORT)) {
        auto result = parseTopIdentifierDeclaration();

        declarations.push_back(std::move(result.declaration));
    }

    while (match(VnlcTokenType::EXPORT)) {
        auto result = parseExportDeclaration();

        exportDeclarations.push_back(std::move(result.declaration));
    }

    const VnlcToken& lastToken = peek();

    std::unique_ptr<VnlcModuleNode> node =
        std::make_unique<VnlcModuleNode>(std::move(name), std::move(fullName), std::move(importDeclarations), std::move(declarations), std::move(exportDeclarations), firstToken, lastToken);
    
    return VnlcModuleParsingResult{ .moduleNode = std::move(node) };
}