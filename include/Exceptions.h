#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

// Custom exception for when a passenger attempts to book a flight with no available seats.
class FlightFullException : public std::exception {
private:
    std::string message;
public:
    explicit FlightFullException(const std::string& flightNo) {
        message = "Booking Error: Flight " + flightNo + " is already full!";
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Custom exception for when a ticket cancellation is invalid (e.g. not found or already cancelled).
class InvalidCancellationException : public std::exception {
private:
    std::string message;
public:
    explicit InvalidCancellationException(const std::string& reason) {
        message = "Cancellation Error: " + reason;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Custom exception for duplicate booking detection
class DuplicateBookingException : public std::exception {
private:
    std::string message;
public:
    explicit DuplicateBookingException(const std::string& passengerId, const std::string& flightNo) {
        message = "Booking Error: Passenger " + passengerId + " is already booked on Flight " + flightNo + "!";
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Custom exception for input parameter validation failures
class InvalidInputException : public std::exception {
private:
    std::string message;
public:
    explicit InvalidInputException(const std::string& reason) {
        message = "Validation Error: " + reason;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Custom exception for database operations failures
class DatabaseException : public std::exception {
private:
    std::string message;
public:
    explicit DatabaseException(const std::string& operation, const std::string& details) {
        message = "Database Error during " + operation + ": " + details;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif // EXCEPTIONS_H
