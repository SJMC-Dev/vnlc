#ifndef VNLC_ENVIRONMENT_HPP
#define VNLC_ENVIRONMENT_HPP

#include <filesystem>

class VnlcEnvironment {
private:
    VnlcEnvironment() = delete;
    VnlcEnvironment(const VnlcEnvironment&) = delete;
    VnlcEnvironment& operator=(const VnlcEnvironment&) = delete;
    VnlcEnvironment(VnlcEnvironment&&) noexcept = delete;
    VnlcEnvironment& operator=(VnlcEnvironment&&) noexcept = delete;

public:
    static const std::filesystem::path COMPLILER_CACHE_DIR;
    static const std::filesystem::path MODULE_INTERFACE_FILE_CACHE_DIR;
};

#endif // VNLC_ENVIRONMENT_HPP