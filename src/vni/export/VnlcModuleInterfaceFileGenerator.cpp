#include "VnlcModuleInterfaceFileGenerator.hpp"
#include "../../ast/declaration/VnlcFunctionDeclarationNode.hpp"
#include "../../ast/declaration/VnlcValueDeclarationNode.hpp"

VnlcModuleInterfaceFileGenerator::VnlcModuleInterfaceFileGenerator(
    std::vector<const VnlcDeclarationNode*>&& declarationNodes,
    std::vector<const VnlcImportedIdentifier*>&& importedIdentifiers,
    const VnlcConfig& config
) noexcept
    : declarationNodes(std::move(declarationNodes)),
      importedIdentifiers(std::move(importedIdentifiers)),
      config(config) {}

nlohmann::json VnlcModuleInterfaceFileGenerator::stringifyMetadata(const std::vector<VnlcDeclarationItem::MetadataTerm>& metadataTerms) {
    nlohmann::json metadataObj;

    for (const auto& metadataTerm : metadataTerms) {
        metadataObj.emplace(metadataTerm.key->getIdentifierString(), metadataTerm.value);
    }

    return metadataObj;
}

std::string VnlcModuleInterfaceFileGenerator::constructFullTypename(const VnlcTypeNode& node) {
    std::string name;

    for (auto& namePart : node.getNameParts()) {
        name += namePart->getIdentifierString();
        name.push_back('.');
    }
}

void VnlcModuleInterfaceFileGenerator::generate() {
    nlohmann::json json;

    for (const auto& declarationNode : declarationNodes) {
        if (const auto* variable = dynamic_cast<const VnlcValueDeclarationNode*>(declarationNode)) {
            nlohmann::json variableObj;
            variableObj.emplace("category", "let");

            const auto& metadata = variable->getMetadataTerms();
            if (!metadata.empty()) {
                variableObj.emplace("metadata", stringifyMetadata(metadata));
            }

        } else if (const auto* function = dynamic_cast<const VnlcFunctionDeclarationNode*>(declarationNode)) {
            nlohmann::json functionObj;
            functionObj.emplace("category", "func");

            const auto& metadata = function->getMetadataTerms();

            if (!metadata.empty()) {
            }
        }
    }

    // not implemented
}