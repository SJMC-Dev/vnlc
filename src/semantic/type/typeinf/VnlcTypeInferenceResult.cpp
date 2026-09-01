#include "VnlcTypeInferenceResult.hpp"
#include <utility>

VnlcTypeInferenceResult::VnlcTypeInferenceResult(std::optional<std::unique_ptr<VnlcSemanticType>> type) : type(std::move(type)) {}