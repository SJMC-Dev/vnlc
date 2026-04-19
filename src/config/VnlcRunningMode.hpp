#pragma once

#ifndef VNLC_RUNNING_MODE_HPP
#define VNLC_RUNNING_MODE_HPP

enum class VnlcRunningMode {
    COMPILE,
    CHECK,
    LINT,
    FORMAT,
    DUMP_AST,
    DUMP_IR,
};

#endif // VNLC_RUNNING_MODE_HPP