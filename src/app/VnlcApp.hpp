#pragma once

#ifndef VNLC_APP_HPP
#define VNLC_APP_HPP

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

class VnlcApp {
private:
    CLI::App app;

    int argc;
    char** argv;

    VnlcApp() = delete;
    VnlcApp(const VnlcApp&) = delete;
    VnlcApp(VnlcApp&&) noexcept = delete;

    VnlcApp& operator=(const VnlcApp&) = delete;

public:
    VnlcApp(int argc, char** argv);

    void run();
};

#endif // VNLC_APP_HPP