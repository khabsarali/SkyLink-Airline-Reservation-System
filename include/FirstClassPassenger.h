#ifndef FIRST_CLASS_PASSENGER_H
#define FIRST_CLASS_PASSENGER_H

#include "Passenger.h"

// Derived class for First Class Passenger
class FirstClassPassenger : public Passenger {
public:
    FirstClassPassenger(const std::string& id, const std::string& name, const std::string& email);

    double getBaggageAllowance() const override;
    double getLoyaltyMultiplier() const override;
    double getCancellationRefundPercentage() const override;
    std::string getPassengerType() const override;
};

#endif // FIRST_CLASS_PASSENGER_H
