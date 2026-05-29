#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>

// Abstract Base Class representing a Passenger
class Passenger {
protected:
    std::string passengerId;
    std::string name;
    std::string email;

public:
    Passenger(const std::string& id, const std::string& name, const std::string& email);
    virtual ~Passenger() = default;

    // Getters & Encapsulation
    std::string getPassengerId() const;
    std::string getName() const;
    std::string getEmail() const;

    // Pure virtual functions representing abstraction
    virtual double getBaggageAllowance() const = 0;
    virtual double getLoyaltyMultiplier() const = 0;
    virtual double getCancellationRefundPercentage() const = 0;
    virtual std::string getPassengerType() const = 0;

    // Display passenger information
    virtual void displayDetails() const;
};

#endif // PASSENGER_H
