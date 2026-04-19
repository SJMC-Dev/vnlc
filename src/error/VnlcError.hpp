#pragma once

#ifndef __VNLC_ERROR_HPP__
#define __VNLC_ERROR_HPP__

#include <exception>
#include <string>
#include <string_view>

class VnlcError : public std::exception {
private:
    std::string message;

public:
    VnlcError(std::string_view message) : message(message) {}

    const char* what() const noexcept override { return message.c_str(); }
};

#endif // __VNLC_ERROR_HPP__