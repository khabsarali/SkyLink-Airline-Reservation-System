#include "Passenger.h"
#include "Exceptions.h"
#include "UIHelper.h"
#include <iostream>
#include <iomanip>

Passenger::Passenger(const std::string& id, const std::string& passengerName, const std::string& passengerEmail)
    : passengerId(id), name(passengerName), email(passengerEmail) {
    if (id.empty()) {
        throw InvalidInputException("Passenger ID cannot be empty.");
    }
    if (passengerName.empty()) {
        throw InvalidInputException("Passenger name cannot be empty.");
    }
    if (passengerEmail.empty()) {
        throw InvalidInputException("Passenger email cannot be empty.");
    }
    if (id.find('|') != std::string::npos || passengerName.find('|') != std::string::npos || passengerEmail.find('|') != std::string::npos) {
        throw InvalidInputException("Input fields cannot contain the vertical pipe character '|'.");
    }
}

std::string Passenger::getPassengerId() const {
    return passengerId;
}

std::string Passenger::getName() const {
    return name;
}

std::string Passenger::getEmail() const {
    return email;
}

void Passenger::displayDetails() const {
    UIHelper::printTableRow({
        passengerId,
        name,
        email,
        getPassengerType(),
        std::to_string(static_cast<int>(getBaggageAllowance())) + " kg",
        std::to_string(static_cast<int>(getCancellationRefundPercentage())) + "%"
    }, {10, 18, 22, 11, 7, 6});
}
