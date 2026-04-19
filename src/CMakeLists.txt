add_executable(
    vnlc
    main.cpp
    lexeme/lexer/VnlcLexer.cpp
    lexeme/token/VnlcToken.cpp
    session/VnlcSession.cpp
    app/VnlcApp.cpp
    util/VnlcRunningModeUtil.cpp
    log/VnlcLogger.cpp
)

target_link_libraries(
    vnlc
    PRIVATE
    fmt::fmt
    nlohmann_json::nlohmann_json
    CLI11::CLI11
)