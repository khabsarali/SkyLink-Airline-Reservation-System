#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>

class Ticket;

// Abstract Base Class representing a Passenger
class Passenger {
protected:
    std::string passengerId;
    std::string name;
    std::string email;
    std::vector<std::weak_ptr<Ticket>> bookingHistory;

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

    // Travel History management (uses weak_ptr to break reference cycles)
    void addTicketToHistory(std::shared_ptr<Ticket> ticket);
    std::vector<std::shared_ptr<Ticket>> getBookingHistory() const;

    // Display passenger information
    virtual void displayDetails() const;

    // Virtual print function for polymorphic stream insertion
    virtual void print(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const Passenger& passenger);
};


#endif // PASSENGER_H
