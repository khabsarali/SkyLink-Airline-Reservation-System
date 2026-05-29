#ifndef CONSOLE_HELPER_H
#define CONSOLE_HELPER_H

#include <string>
#include <iostream>

namespace Console {
    // ANSI Escape Sequences for Terminal Coloring
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[1;31m";
    const std::string GREEN   = "\033[1;32m";
    const std::string YELLOW  = "\033[1;33m";
    const std::string BLUE    = "\033[1;34m";
    const std::string MAGENTA = "\033[1;35m";
    const std::string CYAN    = "\033[1;36m";
    const std::string BOLD    = "\033[1m";
    const std::string DIM     = "\033[2m";

    // Output helper functions
    inline void printSuccess(const std::string& msg) {
        std::cout << GREEN << "[SUCCESS] " << msg << RESET << "\n";
    }

    inline void printError(const std::string& msg) {
        std::cout << RED << "[ERROR] " << msg << RESET << "\n";
    }

    inline void printWarning(const std::string& msg) {
        std::cout << YELLOW << "[WARNING] " << msg << RESET << "\n";
    }

    inline void printInfo(const std::string& msg) {
        std::cout << CYAN << "[INFO] " << msg << RESET << "\n";
    }

    inline void printHeader(const std::string& title) {
        int width = 60;
        std::cout << "\n" << BLUE << std::string(width, '=') << RESET << "\n";
        int padding = (width - static_cast<int>(title.length())) / 2;
        if (padding < 0) padding = 0;
        std::cout << BOLD << CYAN << std::string(padding, ' ') << title << RESET << "\n";
        std::cout << BLUE << std::string(width, '=') << RESET << "\n\n";
    }
}

#endif // CONSOLE_HELPER_H
