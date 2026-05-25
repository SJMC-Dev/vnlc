FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.17.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(
    vnlctest_lexer
    ${PROJECT_SOURCE_DIR}/src/lexer/VnlcLexer.cpp
    ${PROJECT_SOURCE_DIR}/src/token/VnlcToken.cpp
    VnlcLexerTest.cpp
)

file(GLOB_RECURSE AST_FILES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/ast/*.cpp
)

add_executable(
    vnlctest_parser
    ${PROJECT_SOURCE_DIR}/src/lexer/VnlcLexer.cpp
    ${PROJECT_SOURCE_DIR}/src/token/VnlcToken.cpp
    ${PROJECT_SOURCE_DIR}/src/parser/VnlcParser.cpp
    ${PROJECT_SOURCE_DIR}/src/util/VnlcTokenTypeUtil.cpp
    ${AST_FILES}
    VnlcParserTest.cpp
)

target_link_libraries(
    vnlctest_lexer
    PRIVATE
    GTest::gtest_main
    fmt::fmt
    nlohmann_json::nlohmann_json
    CLI11::CLI11
)

target_link_libraries(
    vnlctest_parser
    PRIVATE
    GTest::gtest_main
    fmt::fmt
    nlohmann_json::nlohmann_json
    CLI11::CLI11
)

add_test(NAME vnlctest_lexer COMMAND vnlctest_lexer WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_test(NAME vnlctest_parser COMMAND vnlctest_parser WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(vnlctest_lexer)
gtest_discover_tests(vnlctest_parser)