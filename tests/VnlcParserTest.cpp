#include "../src/parser/VnlcParser.hpp"
#include "../src/config/VnlcConfig.hpp"
#include <filesystem>
#include <gtest/gtest.h>
#include <fstream>

TEST(VnlcParserTest, SimpleModule) {
    std::ifstream input("assets/demo/main.vnl");
    VnlcLexer lexer(input);
    VnlcParser parser(std::move(lexer));
    
    VnlcConfig config{
        .mode = VnlcRunningMode::COMPILE,
        .vanillangVersion = "1.0",
        .minecraftVersion = "26.1.2",
        .packageRootPath = std::filesystem::canonical("assets/demo"),
        .inputFilePath = std::filesystem::canonical("assets/demo/main.vnl"),
        .outputDirectory = std::nullopt,
        .dependencyPackageRootPaths = {},
        .optimizationLevel = std::nullopt,
    };

    auto result = parser.parse(std::move(config));
}