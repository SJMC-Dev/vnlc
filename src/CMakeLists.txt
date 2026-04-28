file(GLOB SOURCES "*.cpp")

add_executable(
    vnlc
    ${SOURCES}
)

target_link_libraries(
    vnlc
    PRIVATE
    fmt::fmt
    nlohmann_json::nlohmann_json
    CLI11::CLI11
)
