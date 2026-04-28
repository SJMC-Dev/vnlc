#pragma once

#ifndef VNLC_DICT_LITERAL_NODE_HPP
#define VNLC_DICT_LITERAL_NODE_HPP

#include "VnlcLiteralNode.hpp"
#include <memory>
#include <unordered_map>

class VnlcDictLiteralNode : public VnlcLiteralNode {
private:
    VnlcDictLiteralNode() = delete;

    std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>> entries;

public:
    VnlcDictLiteralNode(std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>&& entries, const VnlcToken& firstToken, const VnlcToken& lastToken) noexcept;

    const std::unordered_map<std::string, std::unique_ptr<VnlcExpressionNode>>& getEntries() const noexcept;
};

#endif // VNLC_DICT_LITERAL_NODE_HPP