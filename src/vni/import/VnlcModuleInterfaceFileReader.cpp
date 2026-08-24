#include "VnlcModuleInterfaceFileReader.hpp"
#include "../../error/VnlcModuleInterfaceReaderError.hpp"
#include <filesystem>
#include <fmt/core.h>
#include <fstream>

VnlcModuleInterfaceFileReader::VnlcModuleInterfaceFileReader(std::filesystem::path filePath, const VnlcImportDeclarationItem& importItem)
    : filePath(std::filesystem::canonical(filePath)),
      importItem(importItem) {}

std::unique_ptr<VnlcImportedModule> VnlcModuleInterfaceFileReader::read() {
    if (!std::filesystem::exists(filePath)) {
        throw VnlcModuleInterfaceFileReaderError(fmt::format("File {} does not exist", filePath.string()));
    } else if (!std::filesystem::is_regular_file(filePath)) {
        throw VnlcModuleInterfaceFileReaderError(fmt::format("Path {} is not a regular file", filePath.string()));
    } else if (std::filesystem::is_empty(filePath)) {
        throw VnlcModuleInterfaceFileReaderError(fmt::format("File {} is empty", filePath.string()));
    } else if (std::filesystem::is_directory(filePath)) {
        throw VnlcModuleInterfaceFileReaderError(fmt::format("Path {} is a directory, not a file", filePath.string()));
    } else if (!std::filesystem::is_regular_file(filePath)) {
        throw VnlcModuleInterfaceFileReaderError(fmt::format("Path {} is not a regular file", filePath.string()));
    } else if (filePath.extension() != ".vni") {
        throw VnlcModuleInterfaceFileReaderError(fmt::format("File {} must be a .vni file", filePath.string()));
    }

    nlohmann::json json;
    std::ifstream file(filePath);

    try {
        json = nlohmann::json::parse(file);
    } catch (const nlohmann::json::parse_error& e) {
        throw VnlcModuleInterfaceFileReaderError(e.what());
    }

    for (auto& [key, value] : json.items()) {
    }

    throw VnlcModuleInterfaceFileReaderError(fmt::format("Not implemented"));
}