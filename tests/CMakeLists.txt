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

target_link_libraries(
    vnlctest_lexer
    PRIVATE
    GTest::gtest_main
    fmt::fmt
    nlohmann_json::nlohmann_json
    CLI11::CLI11
)

include(GoogleTest)
gtest_discover_tests(vnlctest_lexer)