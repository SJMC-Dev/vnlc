add_executable(
    vnlc
    main.cpp
    frontend/lexeme/lexer/VnlLexer.cpp
    frontend/lexeme/token/VnlToken.cpp
)

target_link_libraries(
    vnlc
    PRIVATE
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)