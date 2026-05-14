#pragma once

#ifndef VNLC_CONFIG_HPP
#define VNLC_CONFIG_HPP

#include "VnlcRunningMode.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <filesystem>

struct VnlcConfig {
    VnlcRunningMode mode;

    std::string vanillangVersion;
    std::string minecraftVersion;

    std::filesystem::path packageRootPath;
    std::filesystem::path inputFilePath;
    std::optional<std::filesystem::path> outputDirectory;

    std::unordered_map<std::string, std::filesystem::path> dependencyPackageRootPaths;

    std::optional<int> optimizationLevel;
};

#endif // VNLC_CONFIG_HPP