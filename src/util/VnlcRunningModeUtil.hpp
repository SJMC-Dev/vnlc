#pragma once

#ifndef __VNLC_RUNNING_MODE_UTIL_HPP__
#define __VNLC_RUNNING_MODE_UTIL_HPP__

#include "../config/VnlcRunningMode.hpp"
#include <string_view>

namespace VnlcRunningModeUtil {
    VnlcRunningMode getRunningMode(std::string_view modeLiteral);
}

#endif // __VNLC_RUNNING_MODE_UTIL_HPP__