#include "VnlcApp.hpp"
#include "../config/VnlcConfig.hpp"
#include "../error/VnlcModuleNameConflictError.hpp"
#include "../log/VnlcLogger.hpp"
#include "../session/VnlcSession.hpp"
#include "../util/VnlcRunningModeUtil.hpp"
#include <CLI/CLI.hpp>
#include <filesystem>
#include <fmt/core.h>
#include <vector>

VnlcApp::VnlcApp(int argc, char** argv) : argc{ argc }, argv{ argv }, app{ "Vanillang Compiler" } {}

void VnlcApp::run() {
    std::string_view mode;
    std::string vanillangVersion;
    std::string minecraftVersion;

    bool verbose = false;

    std::string moduleRootPath = std::filesystem::current_path().string();
    std::string inputFilePath;
    std::string outputDirectory;
    std::vector<std::string> dependencyModuleRootPaths;

    int optimizationLevel = 0;

    app.require_subcommand(1);

    app.add_option("-s,--standard", vanillangVersion, "Vanillang standard version")
        ->default_val("1.0")
        ->check(
            CLI::IsMember(
                {
                    "1.0",
                }
            )
        );
    app.add_option("-t,--target", minecraftVersion, "Target Minecraft version")
        ->default_val("26.1.2")
        ->check(
            CLI::IsMember(
                {
                    "26.1",
                    "26.1.1",
                    "26.1.2",
                }
            )
        );
    app.add_flag("--verbose", verbose, "Enable verbose output")->default_val(false);

    CLI::App* compileApp = app.add_subcommand("compile", "Compile a Vanillang source file to one or more mcfunction files");
    CLI::App* checkApp = app.add_subcommand("check", "Check a Vanillang source file for errors without generating any output");
    CLI::App* lintApp = app.add_subcommand("lint", "Check a Vanillang source file for style issues without generating any output");
    CLI::App* formatApp = app.add_subcommand("format", "Format a Vanillang source file according to recommended style guidelines");
    CLI::App* dumpAstApp = app.add_subcommand("dump-ast", "Parse a Vanillang source file and generate a JSON representation of the resulting abstract syntax tree");
    CLI::App* dumpIrApp = app.add_subcommand("dump-ir", "Compile a Vanillang source file and generate a file of intermediate representation");

    auto addCommonOptions = [&](CLI::App* subApp, bool requireOutput = false) {
        subApp->add_option("-i,--input", inputFilePath, "Path to the input Vanillang source file")->required()->check(CLI::ExistingFile);
        subApp->add_option("-m,--module", moduleRootPath, "Path to the root module")->default_val(std::filesystem::current_path().string())->check(CLI::ExistingDirectory);
        subApp->add_option("-d,--dependencies", dependencyModuleRootPaths, "Paths to root modules of dependencies your source code require")->check(CLI::ExistingDirectory);

        if (requireOutput) {
            subApp->add_option("-o,--output", outputDirectory, "Path to the output directory")->required()->check(CLI::ExistingDirectory);
        }
    };

    addCommonOptions(compileApp, true);
    compileApp->add_option("--optimization-level", optimizationLevel, "Optimization level that compiler uses, including 0, 1, 2 and 3")->default_val(0)->check(CLI::IsMember({ 0, 1, 2, 3 }));

    addCommonOptions(checkApp);
    addCommonOptions(lintApp);
    addCommonOptions(formatApp);
    addCommonOptions(dumpAstApp, true);
    addCommonOptions(dumpIrApp, true);

    compileApp->callback([&]() { mode = "compile"; });
    checkApp->callback([&]() { mode = "check"; });
    lintApp->callback([&]() { mode = "lint"; });
    formatApp->callback([&]() { mode = "format"; });
    dumpAstApp->callback([&]() { mode = "dump-ast"; });
    dumpIrApp->callback([&]() { mode = "dump-ir"; });

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        std::exit(app.exit(e));
    }
    VnlcLogger::setVerbose(verbose);

    VNLC_LOG_INFO(fmt::format("Vanillang Compiler started in {} mode.", mode));

    VnlcConfig config{
        .mode = VnlcRunningModeUtil::getRunningMode(mode),
        .vanillangVersion{ std::move(vanillangVersion) },
        .minecraftVersion{ std::move(minecraftVersion) },
        .moduleRootPath{ std::move(moduleRootPath) },
        .inputFilePath{ std::move(inputFilePath) },
        .outputDirectory{ outputDirectory.empty() ? std::nullopt : std::make_optional(std::move(outputDirectory)) },
        .dependencyModuleRootPaths{},
        .optimizationLevel = (mode == "compile") ? std::make_optional(optimizationLevel) : std::nullopt,
    };

    std::string moduleName = std::filesystem::path(config.moduleRootPath).filename().string();
    for (auto& dependencyModuleRootPath : dependencyModuleRootPaths) {
        std::string dependencyName = std::filesystem::path(dependencyModuleRootPath).filename().string();

        if (dependencyName == moduleName || config.dependencyModuleRootPaths.contains(dependencyName)) {
            throw VnlcModuleNameConflictError(std::move(dependencyName));
        }

        config.dependencyModuleRootPaths.emplace(std::move(dependencyName), std::move(dependencyModuleRootPath));
    }

    VnlcSession session{ std::move(config) };
    session.run();
}