add_executable(
    vnlc
    main.cpp
    lexeme/lexer/VnlcLexer.cpp
    lexeme/token/VnlcToken.cpp
    session/VnlcSession.cpp
)

target_link_libraries(
    vnlc
    PRIVATE
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    CLI11::CLI11
)