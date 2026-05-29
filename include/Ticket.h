#ifndef TICKET_H
#define TICKET_H

#include <string>
#include <memory>
#include <iostream>
#include "Passenger.h"
#include "Flight.h"

// Class representing a booked Ticket linking a Passenger to a Flight
class Ticket {
private:
    std::string ticketId;
    std::shared_ptr<Passenger> passenger;
    std::shared_ptr<Flight> flight;
    int seatNumber;
    double farePaid;
    std::string bookingStatus; // "Confirmed" or "Cancelled"

public:
    Ticket(const std::string& tId, std::shared_ptr<Passenger> p, std::shared_ptr<Flight> f,
           int seat, double fare, const std::string& status = "Confirmed");

    // Getters and Setters
    std::string getTicketId() const;
    std::shared_ptr<Passenger> getPassenger() const;
    std::shared_ptr<Flight> getFlight() const;
    int getSeatNumber() const;
    double getFarePaid() const;
    std::string getBookingStatus() const;
    
    void setBookingStatus(const std::string& status);

    // Operator overloads
    bool operator==(const Ticket& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Ticket& ticket);
};

#endif // TICKET_H
