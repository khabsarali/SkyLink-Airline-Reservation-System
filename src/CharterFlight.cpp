#include "CharterFlight.h"
#include "Exceptions.h"
#include "UIHelper.h"
#include <iostream>
#include <iomanip>

CharterFlight::CharterFlight(const std::string& flightNum, const std::string& orig, const std::string& dest,
                             const std::string& depTime, int totSeats, int availSeats,
                             double rate, double hours, double fee)
    : Flight(flightNum, orig, dest, depTime, totSeats, availSeats),
      hourlyRate(rate), flightHours(hours), overheadFee(fee) {
    if (rate < 0.0) {
        throw InvalidInputException("Hourly rate cannot be negative.");
    }
    if (hours <= 0.0) {
        throw InvalidInputException("Flight hours must be greater than zero.");
    }
    if (fee < 0.0) {
        throw InvalidInputException("Overhead operation fee cannot be negative.");
    }
}

double CharterFlight::calculateBaseFare() const {
    // Charter pricing: (Hourly Rate * Flight Hours + Overhead Fee) / Total Seats
    if (totalSeats <= 0) return 0.0;
    return (hourlyRate * flightHours + overheadFee) / totalSeats;
}

void CharterFlight::displayDetails() const {
    UIHelper::printTableRow({
        flightNumber,
        "Charter (" + std::to_string(static_cast<int>(flightHours)) + "h)",
        origin,
        destination,
        departureTime,
        std::to_string(availableSeats) + " / " + std::to_string(totalSeats),
        "$" + std::to_string(static_cast<int>(calculateBaseFare()))
    }, {10, 13, 14, 14, 18, 11, 9});
}

std::string CharterFlight::getFlightType() const {
    return "Charter";
}

double CharterFlight::getHourlyRate() const {
    return hourlyRate;
}

double CharterFlight::getFlightHours() const {
    return flightHours;
}

double CharterFlight::getOverheadFee() const {
    return overheadFee;
}

void CharterFlight::print(std::ostream& os) const {
    Flight::print(os);
    os << "\nHourly Rate   : $" << hourlyRate << "\n"
       << "Flight Hours  : " << flightHours << "\n"
       << "Overhead Fee  : $" << overheadFee;
}

