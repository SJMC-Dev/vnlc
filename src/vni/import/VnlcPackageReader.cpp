#include "VnlcPackageReader.hpp"
#include "../../error/VnlcPackageReaderError.hpp"
#include "VnlcModuleInterfaceFileReader.hpp"
#include <algorithm>

VnlcPackageReader::VnlcPackageReader(std::unordered_map<std::string, std::unique_ptr<VnlcImportedPackage>>& packages) : packages(packages) {}

void VnlcPackageReader::readPackage(const VnlcImportDeclarationItem& importItem, const VnlcConfig& config) {
    std::vector<std::filesystem::path> candidatePackages;
    std::transform(config.dependencyPackageRootPaths.begin(), config.dependencyPackageRootPaths.end(), std::back_inserter(candidatePackages), [&](const auto& package) {
        return package.first;
    });

    readRecursively(importItem, candidatePackages, nullptr);
}

void VnlcPackageReader::readRecursively(const VnlcImportDeclarationItem& importItem, const std::vector<std::filesystem::path>& candidatePaths, VnlcImportedPackage* currentPackage) {
    std::vector<std::filesystem::path> nextCandidatePaths = std::move(candidatePaths);

    for (const auto& namePrefixNode : importItem.namePrefix) {
        std::string namePrefix = std::string(namePrefixNode->getIdentifierString());
        std::vector<std::string> nextCandidates;
        std::transform(nextCandidatePaths.begin(), nextCandidatePaths.end(), nextCandidates.begin(), [&](const auto& path) { return path.filename().string(); });

        bool foundCandidate = false;
        bool isModule = false;
        for (auto& candidate : nextCandidates) {
            if (candidate == namePrefix) {
                foundCandidate = true;
                break;
            }
            if (candidate == namePrefix + ".vni") {
                foundCandidate = true;
                isModule = true;
                break;
            }
        }
        if (!foundCandidate) {
            throw VnlcPackageReaderError(fmt::format("Could not find package or module with name: {}", namePrefix));
        }

        if (isModule) {
            if (!currentPackage) {
                throw VnlcPackageReaderError(fmt::format("Module {} cannot be imported at the root level", namePrefix));
            }

            VnlcModuleInterfaceFileReader moduleReader(std::filesystem::path(namePrefix + ".vni"), importItem);
            currentPackage->addModule(moduleReader.read());

            return;
        }

        if (!currentPackage) {
            if (packages.find(namePrefix) == packages.end()) {
                packages.emplace(
                    namePrefix,
                    std::make_unique<VnlcImportedPackage>(
                        namePrefix,
                        std::unordered_map<std::string, std::unique_ptr<VnlcImportedPackage>>(),
                        std::unordered_map<std::string, std::unique_ptr<VnlcImportedModule>>()
                    )
                );
            }

            currentPackage = packages[namePrefix].get();
        } else {
            currentPackage->addSubPackage(
                std::make_unique<VnlcImportedPackage>(
                    namePrefix,
                    std::unordered_map<std::string, std::unique_ptr<VnlcImportedPackage>>(),
                    std::unordered_map<std::string, std::unique_ptr<VnlcImportedModule>>()
                )
            );
            currentPackage = currentPackage->getSubPackages().at(std::string(namePrefix)).get();
        }

        nextCandidatePaths.erase(nextCandidatePaths.begin(), nextCandidatePaths.end());

        for (auto& path : nextCandidatePaths) {
            if (path.filename() == std::string(namePrefix)) {
                for (const auto& entry : std::filesystem::directory_iterator(path)) {
                    if (entry.is_directory() || (entry.is_regular_file() && entry.path().extension() == ".vni")) {
                        nextCandidatePaths.push_back(entry.path());
                    }
                }
            }

            break;
        }
    }

    if (importItem.nameSuffices.has_value()) {
        for (const auto& suffix : importItem.nameSuffices.value()) {
            readRecursively(*suffix, nextCandidatePaths, currentPackage);
        }
    }
}