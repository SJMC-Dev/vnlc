#ifndef VNLC_MODULE_INTERFACE_FILE_READER_HPP
#define VNLC_MODULE_INTERFACE_FILE_READER_HPP

#include "../../ast/declaration/VnlcImportDeclarationItem.hpp"
#include "VnlcImportedAlias.hpp"
#include "VnlcImportedClass.hpp"
#include "VnlcImportedConst.hpp"
#include "VnlcImportedEnum.hpp"
#include "VnlcImportedEnumMember.hpp"
#include "VnlcImportedFunc.hpp"
#include "VnlcImportedInterface.hpp"
#include "VnlcImportedLet.hpp"
#include "VnlcImportedMethod.hpp"
#include "VnlcImportedModule.hpp"
#include "VnlcImportedParameter.hpp"
#include "VnlcImportedProperty.hpp"
#include "VnlcImportedTypeAlias.hpp"
#include "VnlcImportedVar.hpp"
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>

class VnlcModuleInterfaceFileReader {
private:
    std::filesystem::path filePath;
    const VnlcImportDeclarationItem& importItem;

    std::unique_ptr<VnlcImportedVar> parseImportedVar(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedLet> parseImportedLet(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedConst> parseImportedConst(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedFunc> parseImportedFunc(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedMethod> parseImportedMethod(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedClass> parseImportedClass(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedInterface> parseImportedInterface(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedEnum> parseImportedEnum(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedEnumMember> parseImportedEnumMember(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedTypeAlias> parseImportedTypeAlias(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedAlias> parseImportedAlias(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedProperty> parseImportedProperty(const nlohmann::json& json);
    std::unique_ptr<VnlcImportedParameter> parseImportedParameter(const nlohmann::json& json);

public:
    VnlcModuleInterfaceFileReader(std::filesystem::path filePath, const VnlcImportDeclarationItem& importItem);

    [[nodiscard]] std::unique_ptr<VnlcImportedModule> read();
};

#endif // VNLC_MODULE_INTERFACE_FILE_READER_HPP