#include "FirstClassPassenger.h"

FirstClassPassenger::FirstClassPassenger(const std::string& id, const std::string& name, const std::string& email)
    : Passenger(id, name, email) {}

double FirstClassPassenger::getBaggageAllowance() const {
    return 50.0; // 50 kg limit
}

double FirstClassPassenger::getLoyaltyMultiplier() const {
    return 2.0; // Double loyalty points
}

double FirstClassPassenger::getCancellationRefundPercentage() const {
    return 90.0; // 90% refund upon cancellation
}

std::string FirstClassPassenger::getPassengerType() const {
    return "FirstClass";
}

void FirstClassPassenger::print(std::ostream& os) const {
    Passenger::print(os);
    os << "\nBenefits     : Luxury suite access, private transfer, premium catering.";
}

