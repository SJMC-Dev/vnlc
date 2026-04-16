add_executable(
    vnlc
    main.cpp
    frontend/lexeme/lexer/VnlcLexer.cpp
    frontend/lexeme/token/VnlcToken.cpp
)

target_link_libraries(
    vnlc
    PRIVATE
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)