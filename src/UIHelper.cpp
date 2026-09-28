#include "UIHelper.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
static void sleepMs(int ms) {
    Sleep(ms);
}
#else
#include <unistd.h>
static void sleepMs(int ms) {
    usleep(ms * 1000);
}
#endif

// Pure Monochrome Swiss-Style Design System (ANSI grayscale / contrast)
const std::string UIHelper::RESET   = "\033[0m";
const std::string UIHelper::BOLD    = "\033[1m";
const std::string UIHelper::DIM     = "\033[2m";
const std::string UIHelper::RED     = "\033[1m";  // Mapped to monochrome BOLD
const std::string UIHelper::GREEN   = "\033[1m";  // Mapped to monochrome BOLD
const std::string UIHelper::YELLOW  = "\033[2m";  // Mapped to monochrome DIM
const std::string UIHelper::BLUE    = "\033[1m";  // Mapped to monochrome BOLD
const std::string UIHelper::MAGENTA = "\033[1m";  // Mapped to monochrome BOLD
const std::string UIHelper::CYAN    = "\033[1m";  // Mapped to monochrome BOLD
const std::string UIHelper::WHITE   = "\033[1;37m";

void UIHelper::clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void UIHelper::printHorizontalLine(int width, char symbol) {
    std::cout << "+" << std::string(width - 2, symbol) << "+\n";
}

void UIHelper::printBoxTop(int width, char symbol) {
    std::cout << "+" << std::string(width - 2, symbol) << "+\n";
}

void UIHelper::printBoxBottom(int width, char symbol) {
    std::cout << "+" << std::string(width - 2, symbol) << "+\n";
}

void UIHelper::printCenteredText(const std::string& text, int width, const std::string& color) {
    int totalPadding = width - 2 - static_cast<int>(text.length());
    if (totalPadding < 0) totalPadding = 0;
    int leftPadding = totalPadding / 2;
    int rightPadding = totalPadding - leftPadding;

    std::cout << "|";
    if (!color.empty()) std::cout << color;
    std::cout << std::string(leftPadding, ' ') << text << std::string(rightPadding, ' ');
    if (!color.empty()) std::cout << RESET;
    std::cout << "|\n";
}

void UIHelper::printWelcomeBanner() {
    clearScreen();
    int width = 74;
    std::cout << BOLD;
    printHorizontalLine(width, '=');
    printCenteredText("SKYLINK AIRLINE RESERVATION & MANAGEMENT SYSTEM", width, BOLD);
    printCenteredText("AEROSPACE FLEET & PASSENGER CONTROL ENVIRONMENT", width, DIM);
    printCenteredText("MINIMALIST SWISS MONOCHROME EDITION", width, BOLD);
    printHorizontalLine(width, '=');
    std::cout << RESET << "\n";
}

void UIHelper::printLoadingScreen(int durationMs) {
    std::cout << BOLD << "INITIALIZING SYSTEM DATABASE...\n" << RESET;
    std::cout << "[";
    int progressSteps = 10;
    int sleepInterval = durationMs / progressSteps;
    for (int i = 0; i < progressSteps; ++i) {
        std::cout << "=" << std::flush;
        sleepMs(sleepInterval);
    }
    std::cout << "] 100% SYNCHRONIZED\n\n";
}

void UIHelper::printExitBanner() {
    std::cout << BOLD
              << "+==========================================================================+\n"
              << "|               SESSION TERMINATED. ALL SYSTEM DATA COMMITTED              |\n"
              << "+==========================================================================+\n"
              << RESET << "\n";
}

void UIHelper::printFooter() {
    int width = 74;
    std::cout << BOLD;
    printHorizontalLine(width, '=');
    printCenteredText("SkyLink System Engine | Fully OOP C++17 Architecture", width, DIM);
    printHorizontalLine(width, '=');
    std::cout << RESET << "\n";
}

void UIHelper::printMenuBox(const std::string& title, const std::vector<std::string>& options) {
    int width = 60;
    std::cout << BOLD;
    std::cout << "+" << std::string(width - 2, '=') << "+\n";
    printCenteredText(title, width, BOLD);
    std::cout << "+" << std::string(width - 2, '=') << "+\n";
    std::cout << RESET;

    for (const auto& opt : options) {
        std::string line = "  " + opt;
        int padding = width - 2 - static_cast<int>(line.length());
        if (padding < 0) padding = 0;
        std::cout << "| " << opt << std::string(padding, ' ') << "|\n";
    }

    std::cout << BOLD;
    std::cout << "+" << std::string(width - 2, '=') << "+\n";
    std::cout << RESET;
}

void UIHelper::printSuccessMessage(const std::string& msg) {
    int width = 64;
    std::cout << "\n" << BOLD;
    printHorizontalLine(width, '=');
    printCenteredText("[ OK ] OPERATION SUCCESSFUL", width, BOLD);
    printHorizontalLine(width, '-');
    printCenteredText(msg, width, RESET);
    printHorizontalLine(width, '=');
    std::cout << RESET << "\n";
}

void UIHelper::printErrorMessage(const std::string& msg) {
    int width = 64;
    std::cout << "\n" << BOLD;
    printHorizontalLine(width, '=');
    printCenteredText("[ ERROR ] ACTION REJECTED", width, BOLD);
    printHorizontalLine(width, '-');
    printCenteredText(msg, width, RESET);
    printHorizontalLine(width, '=');
    std::cout << RESET << "\n";
}

void UIHelper::printWarningMessage(const std::string& msg) {
    int width = 64;
    std::cout << "\n" << BOLD;
    printHorizontalLine(width, '=');
    printCenteredText("[ NOTICE ] SYSTEM WARNING", width, BOLD);
    printHorizontalLine(width, '-');
    printCenteredText(msg, width, RESET);
    printHorizontalLine(width, '=');
    std::cout << RESET << "\n";
}

void UIHelper::printTableHeader(const std::vector<std::string>& headers, const std::vector<int>& widths) {
    std::cout << "+";
    for (size_t i = 0; i < widths.size(); ++i) {
        std::cout << std::string(widths[i] + 2, '=');
        if (i < widths.size() - 1) std::cout << "+";
    }
    std::cout << "+\n";

    std::cout << "|";
    for (size_t i = 0; i < headers.size(); ++i) {
        std::cout << " " << BOLD << std::left << std::setw(widths[i]) << headers[i] << RESET << " |";
    }
    std::cout << "\n";

    printTableSeparator(widths);
}

void UIHelper::printTableRow(const std::vector<std::string>& cells, const std::vector<int>& widths, const std::string& textColor) {
    std::cout << "|";
    for (size_t i = 0; i < cells.size(); ++i) {
        std::cout << " ";
        if (!textColor.empty()) std::cout << textColor;
        std::cout << std::left << std::setw(widths[i]) << cells[i] << RESET << " |";
    }
    std::cout << "\n";
}

void UIHelper::printTableSeparator(const std::vector<int>& widths) {
    std::cout << "+";
    for (size_t i = 0; i < widths.size(); ++i) {
        std::cout << std::string(widths[i] + 2, '-');
        if (i < widths.size() - 1) std::cout << "+";
    }
    std::cout << "+\n";
}

void UIHelper::printBookingSuccessPopup(const std::string& passengerName, const std::string& flightNo, int seatNo, double fare) {
    int width = 56;
    std::cout << "\n" << BOLD;
    std::cout << "+" << std::string(width - 2, '=') << "+\n";
    printCenteredText("CONFIRMED BOARDING PASS RESERVATION", width, BOLD);
    std::cout << "+" << std::string(width - 2, '=') << "+\n";
    std::cout << RESET;

    std::vector<std::pair<std::string, std::string>> fields = {
        {"PASSENGER NAME", passengerName},
        {"FLIGHT NUMBER  ", flightNo},
        {"SEAT ALLOCATION", "Seat #" + std::to_string(seatNo)},
        {"TOTAL FARE PAID", "$" + std::to_string(static_cast<int>(fare)) + ".00"}
    };

    for (const auto& field : fields) {
        std::string content = "  " + field.first + " : " + field.second;
        int padding = width - 2 - static_cast<int>(content.length());
        if (padding < 0) padding = 0;
        std::cout << "| " << BOLD << field.first << RESET << " : " << std::left << std::setw(width - 6 - static_cast<int>(field.first.length())) << field.second << " |\n";
    }

    std::cout << BOLD;
    std::cout << "+" << std::string(width - 2, '=') << "+\n";
    std::cout << RESET << "\n";
}

void UIHelper::printCancellationSuccessPopup(const std::string& ticketId, const std::string& name, double refundPct, double refundAmount) {
    int width = 56;
    std::cout << "\n" << BOLD;
    std::cout << "+" << std::string(width - 2, '=') << "+\n";
    printCenteredText("TICKET CANCELLED & REFUND SETTLED", width, BOLD);
    std::cout << "+" << std::string(width - 2, '=') << "+\n";
    std::cout << RESET;

    std::vector<std::pair<std::string, std::string>> fields = {
        {"TICKET ID      ", ticketId},
        {"PASSENGER NAME ", name},
        {"REFUND SETTLE  ", std::to_string(static_cast<int>(refundPct)) + "% Settle Policy"},
        {"REFUND AMOUNT  ", "$" + std::to_string(static_cast<int>(refundAmount)) + ".00 Dispatched"}
    };

    for (const auto& field : fields) {
        std::string content = "  " + field.first + " : " + field.second;
        int padding = width - 2 - static_cast<int>(content.length());
        if (padding < 0) padding = 0;
        std::cout << "| " << BOLD << field.first << RESET << " : " << std::left << std::setw(width - 6 - static_cast<int>(field.first.length())) << field.second << " |\n";
    }

    std::cout << BOLD;
    std::cout << "+" << std::string(width - 2, '=') << "+\n";
    std::cout << RESET << "\n";
}
