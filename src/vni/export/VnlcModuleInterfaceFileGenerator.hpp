#ifndef VNLC_MODULE_INTERFACE_FILE_GENERATOR_HPP
#define VNLC_MODULE_INTERFACE_FILE_GENERATOR_HPP

#include "../../ast/declaration/VnlcDeclarationNode.hpp"
#include "../../ast/type/VnlcTypeNode.hpp"
#include "../../config/VnlcConfig.hpp"
#include "../import/VnlcImportedIdentifier.hpp"
#include <nlohmann/json.hpp>
#include <vector>

class VnlcModuleInterfaceFileGenerator {
private:
    std::vector<const VnlcDeclarationNode*> declarationNodes;
    std::vector<const VnlcImportedIdentifier*> importedIdentifiers;
    const VnlcConfig& config;

    [[nodiscard]] nlohmann::json stringifyMetadata(const std::vector<VnlcDeclarationItem::MetadataTerm>& metadataTerms);
    [[nodiscard]] std::string constructFullTypename(const VnlcTypeNode& node);

public:
    VnlcModuleInterfaceFileGenerator(
        std::vector<const VnlcDeclarationNode*>&& declarationNodes,
        std::vector<const VnlcImportedIdentifier*>&& importedIdentifiers,
        const VnlcConfig& config
    ) noexcept;

    void generate();
};

#endif // VNLC_MODULE_INTERFACE_FILE_GENERATOR_HPP