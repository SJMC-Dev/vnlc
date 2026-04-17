#pragma once

#ifndef __VNLC_APP_HPP__
#define __VNLC_APP_HPP__

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

    int run();
};

#endif // __VNLC_APP_HPP__