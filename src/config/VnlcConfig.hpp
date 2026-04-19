#pragma once

#ifndef __VNLC_CONFIG_HPP__
#define __VNLC_CONFIG_HPP__

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

#endif // __VNLC_CONFIG_HPP__