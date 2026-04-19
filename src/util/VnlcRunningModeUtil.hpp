#pragma once

#ifndef VNLC_RUNNING_MODE_UTIL_HPP
#define VNLC_RUNNING_MODE_UTIL_HPP

#include "../config/VnlcRunningMode.hpp"
#include <string_view>

namespace VnlcRunningModeUtil {
    VnlcRunningMode getRunningMode(std::string_view modeLiteral);
}

#endif // VNLC_RUNNING_MODE_UTIL_HPP