#pragma once

#ifndef VNLC_MODULE_HPP
#define VNLC_MODULE_HPP

#include "VnlcAstNode.hpp"

class VnlcModule : public VnlcAstNode {
private:
    std::string fullName;
    std::string name;

public:
    VnlcModule(std::string_view fullName, VnlcToken&& firstToken, VnlcToken&& lastToken) noexcept;
};

#endif // VNLC_MODULE_HPP