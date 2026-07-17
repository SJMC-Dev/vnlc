#include "VnlcEnvironment.hpp"

const std::filesystem::path VnlcEnvironment::COMPLILER_CACHE_DIR = std::filesystem::temp_directory_path() / "vnlc_cache";
const std::filesystem::path VnlcEnvironment::MODULE_INTERFACE_FILE_CACHE_DIR = VnlcEnvironment::COMPLILER_CACHE_DIR / "module_interfaces";