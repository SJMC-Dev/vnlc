#include "VnlcDiagnostic.hpp"

VnlcDiagnostic::VnlcDiagnostic(
    VnlcDiagnosticPhase phase,
    VnlcDiagnosticSeverity severity,
    std::string message,
    unsigned int line,
    unsigned int column,
    unsigned int offset,
    unsigned int length
)
    : phase(phase),
      severity(severity),
      message(std::move(message)),
      line(line),
      column(column),
      offset(offset),
      length(length) {}

VnlcDiagnosticPhase VnlcDiagnostic::getPhase() const {
    return phase;
}

VnlcDiagnosticSeverity VnlcDiagnostic::getSeverity() const {
    return severity;
}

std::string_view VnlcDiagnostic::getMessage() const {
    return message;
}

unsigned int VnlcDiagnostic::getLine() const {
    return line;
}

unsigned int VnlcDiagnostic::getColumn() const {
    return column;
}

unsigned int VnlcDiagnostic::getOffset() const {
    return offset;
}

unsigned int VnlcDiagnostic::getLength() const {
    return length;
}