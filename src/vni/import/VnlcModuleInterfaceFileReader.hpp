#ifndef VNLC_MODULE_INTERFACE_FILE_READER_HPP
#define VNLC_MODULE_INTERFACE_FILE_READER_HPP

#include "VnlcImportedItem.hpp"
#include <istream>
#include <memory>
#include <nlohmann/json.hpp>

class VnlcModuleInterfaceFileReader {
private:
    std::istream& source;

public:
    VnlcModuleInterfaceFileReader(std::istream& source);

    [[nodiscard]] std::unique_ptr<VnlcImportedItem> read();
};

#endif // VNLC_MODULE_INTERFACE_FILE_READER_HPP