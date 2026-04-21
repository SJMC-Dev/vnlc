#pragma once

#ifndef VNLC_ERROR_HPP
#define VNLC_ERROR_HPP

#include <exception>
#include <string>
#include <string_view>

class VnlcError : public std::exception {
private:
    std::string message;

public:
    VnlcError(std::string_view message) : message(message) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif // VNLC_ERROR_HPP