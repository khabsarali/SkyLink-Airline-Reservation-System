#ifndef CHARTER_FLIGHT_H
#define CHARTER_FLIGHT_H

#include "Flight.h"

// Derived class representing a Charter Flight (private/group booked)
class CharterFlight : public Flight {
private:
    double hourlyRate;
    double flightHours;
    double overheadFee;

public:
    CharterFlight(const std::string& flightNum, const std::string& orig, const std::string& dest,
                  const std::string& depTime, int totSeats, int availSeats,
                  double rate, double hours, double fee);

    // Overriding base class pure virtual functions
    double calculateBaseFare() const override;
    void displayDetails() const override;
    std::string getFlightType() const override;

    // Getters
    double getHourlyRate() const;
    double getFlightHours() const;
    double getOverheadFee() const;
};

#endif // CHARTER_FLIGHT_H
