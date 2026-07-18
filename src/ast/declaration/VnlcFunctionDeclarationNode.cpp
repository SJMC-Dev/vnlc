#include "VnlcFunctionDeclarationNode.hpp"

VnlcFunctionDeclarationNode::VnlcFunctionDeclarationNode(
    VnlcFunctionDeclarationType::Kind kind,
    VnlcFunctionDeclarationType::Context context,
    VnlcFunctionDeclarationType::AccessModifier accessModifier,
    VnlcFunctionDeclarationType::Binding binding,
    std::unique_ptr<std::string>&& name,
    std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>&& parameters,
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& returnType,
    std::optional<std::unique_ptr<VnlcBlockStatementNode>>&& body,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken),
      kind(kind),
      context(context),
      accessModifier(accessModifier),
      binding(binding),
      name(std::move(name)),
      parameters(std::move(parameters)),
      returnType(std::move(returnType)),
      body(std::move(body)) {
    generateUniqueName();
}

VnlcFunctionDeclarationNode::VnlcFunctionDeclarationNode(
    VnlcFunctionDeclarationType::Kind kind,
    VnlcFunctionDeclarationType::Context context,
    VnlcFunctionDeclarationType::AccessModifier accessModifier,
    VnlcFunctionDeclarationType::Binding binding,
    std::unique_ptr<std::string>&& name,
    std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>&& parameters,
    std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>&& returnType,
    std::optional<std::unique_ptr<VnlcBlockStatementNode>>&& body,
    const VnlcToken& firstToken,
    const VnlcToken& lastToken,
    std::vector<VnlcDeclarationItem::MetadataTerm>&& metadataTerms
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, std::move(metadataTerms)),
      kind(kind),
      context(context),
      accessModifier(accessModifier),
      binding(binding),
      name(std::move(name)),
      parameters(std::move(parameters)),
      returnType(std::move(returnType)),
      body(std::move(body)) {
    generateUniqueName();
}

void VnlcFunctionDeclarationNode::generateUniqueName() noexcept {
    std::string functionName = name;
    std::vector<std::string> parameterTypes;

    for (const auto& param : parameters) {
        std::string typeName;
        for (auto it = std::get<1>(param)->getTypeNode().getNameParts().begin(); it != std::get<1>(param)->getTypeNode().getNameParts().end(); it++) {
            if (it != std::get<1>(param)->getTypeNode().getNameParts().begin()) {
                typeName.push_back('_');
            }
            typeName.append(*it);
        }
        parameterTypes.push_back(typeName);
    }

    uniqueName = functionName;
    if (!parameterTypes.empty()) {
        uniqueName.push_back('_');
        for (auto& paramType : parameterTypes) {
            uniqueName.append("__");
            uniqueName.append(paramType);
        }
    }
}

const VnlcFunctionDeclarationType::Kind VnlcFunctionDeclarationNode::getKind() const noexcept {
    return kind;
}

const VnlcFunctionDeclarationType::Context VnlcFunctionDeclarationNode::getContext() const noexcept {
    return context;
}

const VnlcFunctionDeclarationType::AccessModifier VnlcFunctionDeclarationNode::getAccessModifier() const noexcept {
    return accessModifier;
}

const VnlcFunctionDeclarationType::Binding VnlcFunctionDeclarationNode::getBinding() const noexcept {
    return binding;
}

std::string_view VnlcFunctionDeclarationNode::getName() const noexcept {
    return name;
}

std::string_view VnlcFunctionDeclarationNode::getUniqueName() const noexcept {
    return uniqueName;
}

const std::vector<std::pair<std::string, std::unique_ptr<VnlcTypeAnnotationNode>>>& VnlcFunctionDeclarationNode::getParameters() const noexcept {
    return parameters;
}

const std::optional<std::unique_ptr<VnlcTypeAnnotationNode>>& VnlcFunctionDeclarationNode::getReturnType() const noexcept {
    return returnType;
}

const std::optional<std::unique_ptr<VnlcBlockStatementNode>>& VnlcFunctionDeclarationNode::getBody() const noexcept {
    return body;
}