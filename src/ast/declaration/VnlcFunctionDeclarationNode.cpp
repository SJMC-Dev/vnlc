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
      body(std::move(body)) {}

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
    bool includeMetadata,
    std::optional<bool> deprecated,
    std::optional<bool> experimental,
    std::optional<bool> nowarnings,
    std::optional<std::string>&& minGameVersion,
    std::optional<std::string>&& maxGameVersion
) noexcept
    : VnlcDeclarationNode(firstToken, lastToken, includeMetadata, deprecated, experimental, nowarnings, std::move(minGameVersion), std::move(maxGameVersion)),
      kind(kind),
      context(context),
      accessModifier(accessModifier),
      binding(binding),
      name(std::move(name)),
      parameters(std::move(parameters)),
      returnType(std::move(returnType)),
      body(std::move(body)) {}

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
    return *name;
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