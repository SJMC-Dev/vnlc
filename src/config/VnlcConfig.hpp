#pragma once

#ifndef __VNLC_CONFIG_HPP__
#define __VNLC_CONFIG_HPP__

#include "VnlcRunningMode.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

struct VnlcConfig {
    VnlcRunningMode mode;

    std::string vanillangVersion;
    std::string minecraftVersion;

    std::string moduleRootPath;

    std::vector<std::string> inputFilePaths;
    std::optional<std::string> outputDirectory;

    std::unordered_map<std::string, std::string> dependencyModuleRootPaths;

    int optimizationLevel;
    bool warningsAsErrors;

    bool verbose;
};

#endif // __VNLC_CONFIG_HPP__