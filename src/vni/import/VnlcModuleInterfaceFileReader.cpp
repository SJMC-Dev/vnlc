#include "VnlcModuleInterfaceFileReader.hpp"

VnlcModuleInterfaceFileReader::VnlcModuleInterfaceFileReader(std::istream& source) : source(source) {}

std::unique_ptr<VnlcImportedItem> VnlcModuleInterfaceFileReader::read() {
    nlohmann::json json;
    source >> json;
}