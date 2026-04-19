#include "app/VnlcApp.hpp"
#include "log/VnlcLogger.hpp"

int main(int argc, char** argv) {
    try {
        VnlcApp app{argc, argv};
        app.run();
    } catch (const std::exception& e) {
        VNLC_LOG_FATAL(e.what());
    }

    return 0;
}