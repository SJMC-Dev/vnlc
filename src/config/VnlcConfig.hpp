#pragma once

#ifndef VNLC_CONFIG_HPP
#define VNLC_CONFIG_HPP

#include "VnlcRunningMode.hpp"

#include <optional>
#include <string>
#include <unordered_map>

struct VnlcConfig {
    VnlcRunningMode mode;

    std::string vanillangVersion;
    std::string minecraftVersion;

    std::string moduleRootPath;

    std::string inputFilePath;
    std::optional<std::string> outputDirectory;

    std::unordered_map<std::string, std::string> dependencyModuleRootPaths;

    std::optional<int> optimizationLevel;
};

#endif // VNLC_CONFIG_HPP