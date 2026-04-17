#include "VnlcApp.hpp"
#include <CLI/CLI.hpp>
#include <filesystem>

VnlcApp::VnlcApp(int argc, char** argv) : argc{argc}, argv{argv}, app{"Vanillang Compiler"} {}

int VnlcApp::run() {
    std::string mode = "compile";
    std::string vanillangVersion = "1.0";
    std::string minecraftVersion = "26.1.2";

    bool warningsAsErrors;

    app.require_subcommand(1);

    app.add_option("-s,--standard", "Vanillang standard version")
        ->default_val("1.0")
        ->check(CLI::IsMember({
            "1.0",
        }));
    app.add_option("-t,--target", "Target Minecraft version")
        ->default_val("26.1.2")
        ->check(CLI::IsMember({
            "26.1",
            "26.1.1",
            "26.1.2",
        }));
    app.add_flag("--warnings-as-errors", warningsAsErrors);
    app.add_flag("--verbose", "Enable verbose output");

    CLI::App* compileApp = app.add_subcommand("compile", "Compile a Vanillang source file to one or more mcfunction files");
    CLI::App* checkApp = app.add_subcommand("check", "Check a Vanillang source file for errors without generating any output");
    CLI::App* lintApp = app.add_subcommand("lint", "Check a Vanillang source file for style issues without generating any output");
    CLI::App* formatApp = app.add_subcommand("format", "Format a Vanillang source file according to recommended style guidelines");
    CLI::App* dumpAstApp = app.add_subcommand("dump-ast", "Parse a Vanillang source file and generate a JSON representation of the resulting abstract syntax tree");
    CLI::App* dumpIrApp = app.add_subcommand("dump-ir", "Compile a Vanillang source file and generate a file of intermediate representation");

    compileApp->add_option("-i,--input", "Path to the input Vanillang source file")->required()->check(CLI::ExistingFile);
    compileApp->add_option("-o,--output", "Path to the output directory for generated mcfunction files")->required()->check(CLI::ExistingDirectory);
    compileApp->add_option("-m,--module", "Path to the root module")->default_val(std::filesystem::current_path().string())->check(CLI::ExistingDirectory);
    compileApp->add_option("-d,--dependencies", "Paths to root modules of dependencies your source code require")->check(CLI::ExistingDirectory);
    compileApp->add_option("--optimization-level", "Optimization level that compiler uses, including 0, 1, 2 and 3")->default_val(0)->check(CLI::IsMember({0, 1, 2, 3}));

    checkApp->add_option("-i,--input", "Path to the input Vanillang source file")->required()->check(CLI::ExistingFile);
    checkApp->add_option("-m,--module", "Path to the root module")->default_val(std::filesystem::current_path().string())->check(CLI::ExistingDirectory);
    checkApp->add_option("-d,--dependencies", "Paths to root modules of dependencies your source code require")->check(CLI::ExistingDirectory);

    lintApp->add_option("-i,--input", "Path to the input Vanillang source file")->required()->check(CLI::ExistingFile);
    lintApp->add_option("-m,--module", "Path to the root module")->default_val(std::filesystem::current_path().string())->check(CLI::ExistingDirectory);
    lintApp->add_option("-d,--dependencies", "Paths to root modules of dependencies your source code require")->check(CLI::ExistingDirectory);

    formatApp->add_option("-i,--input", "Path to the input Vanillang source file")->required()->check(CLI::ExistingFile);
    formatApp->add_option("-o,--output", "Path to the output file for formatted source code")->required()->check(CLI::ExistingFile);
    formatApp->add_option("-m,--module", "Path to the root module")->default_val(std::filesystem::current_path().string())->check(CLI::ExistingDirectory);
    formatApp->add_option("-d,--dependencies", "Paths to root modules of dependencies your source code require")->check(CLI::ExistingDirectory);

    dumpAstApp->add_option("-i,--input", "Path to the input Vanillang source file")->required()->check(CLI::ExistingFile);
    dumpAstApp->add_option("-o,--output", "Path to the output file for generated JSON")->required()->check(CLI::ExistingFile);

    dumpIrApp->add_option("-i,--input", "Path to the input Vanillang source file")->required()->check(CLI::ExistingFile);
    dumpIrApp->add_option("-o,--output", "Path to the output file for generated intermediate representation")->required()->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);
    return 0;
}