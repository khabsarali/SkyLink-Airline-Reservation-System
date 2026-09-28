# Function Catalog for SkyLink Airline Reservation System

This document lists all free‑standing helper functions (non‑member) used in the codebase, together with their signatures and a short description of what they do. It is intended as a quick reference for developers.

---

## File & Directory Helpers
- `bool fileExists(const std::string& filename)`
  > Checks whether the given file exists and is readable.
- `void createDirectory(const std::string& dirName)`
  > Creates a directory (cross‑platform). Uses `_mkdir` on Windows and `mkdir` on POSIX.

## UI Helpers (free functions)
- `void waitForEnter()`
  > Pauses execution until the user presses **Enter**. Used after displaying messages.

## Validation Helpers
- `bool isValidDateTime(const std::string& dt)`
  > Validates a date‑time string in the exact format `YYYY‑MM‑DD HH:MM`.
- `bool isValidMonthYear(const std::string& my)`
  > Validates a month‑year string in the format `YYYY‑MM`.
- `int getValidInt(const std::string& prompt, int minVal, int maxVal)`
  > Prompts the user for an integer within the inclusive range `[minVal, maxVal]`. Re‑asks on invalid input.
- `double getValidDouble(const std::string& prompt, double minVal)`
  > Prompts the user for a double greater than or equal to `minVal`. Re‑asks on invalid input.
- `std::string getNonEmptyString(const std::string& prompt)`
  > Prompts the user for a non‑empty string; repeats until a non‑empty value is entered.
- `std::string getValidDateTime(const std::string& prompt)`
  > Prompts the user for a date‑time string and validates it via `isValidDateTime`.
- `std::string getValidMonthYear(const std::string& prompt)`
  > Prompts the user for a month‑year string and validates it via `isValidMonthYear`.

## Sample Data & Testing
- `void populateSampleData(Airline& airline)`
  > Populates the `Airline` instance with a pre‑defined set of flights and passenger records.
- `void runSelfTests(Airline& airline)`
  > Executes a suite of integration tests covering flight creation, passenger creation, booking, cancellation, and search utilities.

---

*All functions above are defined in **main.cpp** (or included headers) and are used throughout the project for input handling, validation, file system access, and test setup.*

---
