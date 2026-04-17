#pragma once

#ifndef __VNLC_RUNNING_MODE_HPP__
#define __VNLC_RUNNING_MODE_HPP__

enum class VnlcRunningMode {
    COMPILE,
    CHECK,
    LINT,
    FORMAT,
    DUMP_AST,
    DUMP_IR,
};

#endif //__VNLC_RUNNING_MODE_HPP__