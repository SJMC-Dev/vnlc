#pragma once

#ifndef VNLC_RUNNING_MODE_HPP
#define VNLC_RUNNING_MODE_HPP

enum class VnlcRunningMode {
    COMPILE,
    CHECK,
    LINT,
    FORMAT,
    DUMP_AST,
};

#endif // VNLC_RUNNING_MODE_HPP