#include "VnlcTypeInferenceResult.hpp"
#include <utility>

VnlcTypeInferenceResult::VnlcTypeInferenceResult(VnlcSemanticType type, bool success) : type(std::move(type)), success(success) {}