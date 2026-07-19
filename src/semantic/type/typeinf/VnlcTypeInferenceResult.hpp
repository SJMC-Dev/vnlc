#ifndef VNLC_TYPE_INFERENCE_RESULT_HPP
#define VNLC_TYPE_INFERENCE_RESULT_HPP

#include "../VnlcSemanticType.hpp"

struct VnlcTypeInferenceResult {
    bool success;
    VnlcSemanticType type;

    VnlcTypeInferenceResult(VnlcSemanticType type, bool success);
    VnlcTypeInferenceResult(const VnlcTypeInferenceResult&) = delete;
    VnlcTypeInferenceResult& operator=(const VnlcTypeInferenceResult&) = delete;
    VnlcTypeInferenceResult(VnlcTypeInferenceResult&&) noexcept = default;
    VnlcTypeInferenceResult& operator=(VnlcTypeInferenceResult&&) noexcept = default;
};

#endif // VNLC_TYPE_INFERENCE_RESULT_HPP