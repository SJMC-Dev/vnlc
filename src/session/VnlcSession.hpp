#pragma once

#ifndef __VNLC_SESSION_HPP__
#define __VNLC_SESSION_HPP__

#include "../config/VnlcConfig.hpp"

class VnlcSession {
private:
    const VnlcConfig config;

public:
    VnlcSession(VnlcConfig&& config);

    void run();
};

#endif // __VNLC_SESSION_HPP__