#include "VnlcTypeInferenceResult.hpp"
#include <utility>

VnlcTypeInferenceResult::VnlcTypeInferenceResult(std::optional<VnlcSemanticType> type) : type(std::move(type)) {}