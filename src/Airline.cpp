#include "Airline.h"
#include "DomesticFlight.h"
#include "InternationalFlight.h"
#include "CharterFlight.h"
#include "EconomyPassenger.h"
#include "BusinessPassenger.h"
#include "FirstClassPassenger.h"
#include "Exceptions.h"
#include "ConsoleHelper.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <map>

// Helper function to split a string by delimiter
static std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Getters for containers
const std::vector<std::shared_ptr<Flight>>& Airline::getFlights() const {
    return flights;
}

const std::vector<std::shared_ptr<Passenger>>& Airline::getPassengers() const {
    return passengers;
}

const std::vector<std::shared_ptr<Ticket>>& Airline::getTickets() const {
    return tickets;
}

// Flight Management
void Airline::addFlight(std::shared_ptr<Flight> flight) {
    if (flight) {
        flights.push_back(flight);
    }
}

bool Airline::removeFlight(const std::string& flightNo) {
    auto it = std::find_if(flights.begin(), flights.end(), [&](const auto& f) {
        return f->getFlightNumber() == flightNo;
    });
    if (it != flights.end()) {
        flights.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<Flight> Airline::findFlight(const std::string& flightNo) const {
    auto it = std::find_if(flights.begin(), flights.end(), [&](const auto& f) {
        return f->getFlightNumber() == flightNo;
    });
    if (it != flights.end()) {
        return *it;
    }
    return nullptr;
}

#include "UIHelper.h"

void Airline::listFlights() const {
    if (flights.empty()) {
        std::cout << "No flights registered in the system.\n";
        return;
    }
    UIHelper::printTableHeader(
        {"Flight No", "Type", "Origin", "Destination", "Departure Time", "Seats (A/T)", "Base Fare"},
        {10, 13, 14, 14, 18, 11, 9}
    );
    for (const auto& flight : flights) {
        flight->displayDetails();
    }
    UIHelper::printTableSeparator({10, 13, 14, 14, 18, 11, 9});
}

// Passenger Management
void Airline::registerPassenger(std::shared_ptr<Passenger> passenger) {
    if (passenger) {
        passengers.push_back(passenger);
    }
}

bool Airline::removePassenger(const std::string& passengerId) {
    auto it = std::find_if(passengers.begin(), passengers.end(), [&](const auto& p) {
        return p->getPassengerId() == passengerId;
    });
    if (it != passengers.end()) {
        passengers.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<Passenger> Airline::findPassenger(const std::string& passengerId) const {
    auto it = std::find_if(passengers.begin(), passengers.end(), [&](const auto& p) {
        return p->getPassengerId() == passengerId;
    });
    if (it != passengers.end()) {
        return *it;
    }
    return nullptr;
}

void Airline::listPassengers() const {
    if (passengers.empty()) {
        std::cout << "No passengers registered in the system.\n";
        return;
    }
    UIHelper::printTableHeader(
        {"ID", "Name", "Email", "Class Type", "Baggage", "Refund"},
        {10, 18, 22, 11, 7, 6}
    );
    for (const auto& passenger : passengers) {
        passenger->displayDetails();
    }
    UIHelper::printTableSeparator({10, 18, 22, 11, 7, 6});
}

// Booking System
std::shared_ptr<Ticket> Airline::bookTicket(const std::string& passengerId, const std::string& flightNo, int seatNo) {
    auto passenger = findPassenger(passengerId);
    if (!passenger) {
        throw InvalidInputException("Booking Failed: Passenger ID " + passengerId + " not found.");
    }

    auto flight = findFlight(flightNo);
    if (!flight) {
        throw InvalidInputException("Booking Failed: Flight " + flightNo + " not found.");
    }

    // Rule 1: Reject duplicate booking (same passenger confirmed on same flight)
    Ticket tempTicket("TEMP", passenger, flight, 1, 0.0);
    auto dupIt = std::find_if(tickets.begin(), tickets.end(), [&](const auto& t) {
        return t->getBookingStatus() == "Confirmed" && *t == tempTicket;
    });
    if (dupIt != tickets.end()) {
        throw DuplicateBookingException(passengerId, flightNo);
    }

    // Rule 2: Reject if flight is full
    if (flight->getAvailableSeats() <= 0) {
        throw FlightFullException(flightNo);
    }

    // Rule 3: Seat selection and validation
    if (seatNo > 0) {
        if (seatNo > flight->getTotalSeats()) {
            throw InvalidInputException("Booking Failed: Seat number " + std::to_string(seatNo) + 
                                       " exceeds flight capacity of " + std::to_string(flight->getTotalSeats()) + ".");
        }
        // Check if the requested seat is already booked
        for (const auto& t : tickets) {
            if (t->getFlight()->getFlightNumber() == flightNo && 
                t->getSeatNumber() == seatNo && 
                t->getBookingStatus() == "Confirmed") {
                throw InvalidInputException("Booking Failed: Seat number " + std::to_string(seatNo) + 
                                           " is already occupied on flight " + flightNo + ".");
            }
        }
    } else {
        // Auto-allocate: find first unoccupied seat
        int allocatedSeat = 0;
        for (int s = 1; s <= flight->getTotalSeats(); ++s) {
            bool occupied = false;
            for (const auto& t : tickets) {
                if (t->getFlight()->getFlightNumber() == flightNo && 
                    t->getSeatNumber() == s && 
                    t->getBookingStatus() == "Confirmed") {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) {
                allocatedSeat = s;
                break;
            }
        }
        if (allocatedSeat == 0) {
            throw FlightFullException(flightNo);
        }
        seatNo = allocatedSeat;
    }

    flight->bookSeat();

    // Price calculation: Base Fare divided by loyalty multiplier (discount for higher status)
    double baseFare = flight->calculateBaseFare();
    double loyaltyMultiplier = passenger->getLoyaltyMultiplier();
    double finalFare = baseFare / loyaltyMultiplier;

    std::string ticketId = "TKT-" + flightNo + "-" + std::to_string(100 + tickets.size() + 1);
    auto newTicket = std::make_shared<Ticket>(ticketId, passenger, flight, seatNo, finalFare, "Confirmed");
    tickets.push_back(newTicket);

    // Track ticket in Passenger's history using weak_ptr
    passenger->addTicketToHistory(newTicket);

    return newTicket;
}

// Cancellation System
void Airline::cancelTicket(const std::string& ticketId) {
    auto it = std::find_if(tickets.begin(), tickets.end(), [&](const auto& t) {
        return t->getTicketId() == ticketId;
    });

    if (it == tickets.end()) {
        throw InvalidCancellationException("Ticket ID " + ticketId + " not found.");
    }

    auto ticket = *it;
    if (ticket->getBookingStatus() == "Cancelled") {
        throw InvalidCancellationException("Ticket ID " + ticketId + " is already cancelled.");
    }

    // Polymorphic refund calculation based on Passenger properties
    double refundPercentage = ticket->getPassenger()->getCancellationRefundPercentage();
    double refundAmount = ticket->getFarePaid() * (refundPercentage / 100.0);

    // Release seat & update ticket status
    ticket->getFlight()->releaseSeat();
    ticket->setBookingStatus("Cancelled");

    UIHelper::printCancellationSuccessPopup(ticketId, ticket->getPassenger()->getName(), refundPercentage, refundAmount);
}

void Airline::listTickets() const {
    if (tickets.empty()) {
        std::cout << "No bookings registered in the system.\n";
        return;
    }
    for (const auto& ticket : tickets) {
        std::cout << *ticket << "\n\n";
    }
}

std::shared_ptr<Ticket> Airline::findTicket(const std::string& ticketId) const {
    auto it = std::find_if(tickets.begin(), tickets.end(), [&](const auto& t) {
        return t->getTicketId() == ticketId;
    });
    if (it != tickets.end()) {
        return *it;
    }
    return nullptr;
}

// Reports System
void Airline::showTodayDepartures(const std::string& date) const {
    std::cout << "\n" << Console::CYAN << "=== DEPARTURES FOR: " << Console::BOLD << date << Console::CYAN << " ===" << Console::RESET << "\n";
    std::cout << Console::BOLD << Console::BLUE << std::left 
              << std::setw(10) << "Flight No"
              << std::setw(15) << "Type"
              << std::setw(15) << "Origin"
              << std::setw(15) << "Destination"
              << std::setw(20) << "Departure Time"
              << std::setw(15) << "Seats (Avail/Tot)"
              << "Base Fare" << Console::RESET << "\n";
    std::cout << Console::BLUE << std::string(85, '-') << Console::RESET << "\n";
    bool found = false;
    for (const auto& flight : flights) {
        if (flight->getDepartureTime().find(date) != std::string::npos) {
            flight->displayDetails();
            found = true;
        }
    }
    if (!found) {
        std::cout << Console::YELLOW << "No flights departing on this date." << Console::RESET << "\n";
    }
}

void Airline::showOccupancyPercentage() const {
    if (flights.empty()) {
        std::cout << "No flights available to compute occupancy.\n";
        return;
    }

    std::cout << "\n" << Console::BLUE << "=======================================================\n"
              << "                  OCCUPANCY REPORT                     \n"
              << "=======================================================\n" << Console::RESET;
    std::cout << Console::BOLD << Console::BLUE << std::left 
              << std::setw(12) << "Flight No"
              << std::setw(15) << "Type"
              << std::setw(15) << "Occupied/Total"
              << "Occupancy %\n" << Console::RESET;
    std::cout << Console::BLUE << std::string(55, '-') << Console::RESET << "\n";

    double totalOccupied = 0;
    double totalCapacity = 0;

    for (const auto& flight : flights) {
        int occupied = flight->getTotalSeats() - flight->getAvailableSeats();
        double pct = (static_cast<double>(occupied) / flight->getTotalSeats()) * 100.0;
        
        totalOccupied += occupied;
        totalCapacity += flight->getTotalSeats();

        std::string color = Console::RESET;
        if (pct >= 90.0) {
            color = Console::RED;
        } else if (pct >= 50.0) {
            color = Console::GREEN;
        } else if (pct > 0.0) {
            color = Console::YELLOW;
        } else {
            color = Console::DIM;
        }

        std::cout << color << std::left
                  << std::setw(12) << flight->getFlightNumber()
                  << std::setw(15) << flight->getFlightType()
                  << std::setw(2) << occupied << "/" << std::setw(11) << flight->getTotalSeats()
                  << std::fixed << std::setprecision(2) << pct << " %\n" << Console::RESET;
    }

    double overallPct = (totalCapacity > 0) ? (totalOccupied / totalCapacity) * 100.0 : 0.0;
    std::cout << Console::BLUE << std::string(55, '-') << Console::RESET << "\n"
              << "Overall Fleet Occupancy: " << Console::BOLD << Console::GREEN 
              << std::fixed << std::setprecision(2) << overallPct << " %" << Console::RESET << "\n"
              << Console::BLUE << "=======================================================\n" << Console::RESET;
}

void Airline::showTopRevenueFlights() const {
    if (flights.empty()) {
        std::cout << "No flight records found.\n";
        return;
    }

    // Use map to aggregate revenues per flight
    std::map<std::string, double> flightRevenues;
    for (const auto& f : flights) {
        flightRevenues[f->getFlightNumber()] = 0.0;
    }

    for (const auto& ticket : tickets) {
        if (ticket->getBookingStatus() == "Confirmed") {
            flightRevenues[ticket->getFlight()->getFlightNumber()] += ticket->getFarePaid();
        }
    }

    // Helper structure for sorting
    struct FlightRevenue {
        std::shared_ptr<Flight> flightPtr;
        double revenue;
    };

    std::vector<FlightRevenue> revenueList;
    for (const auto& f : flights) {
        revenueList.push_back({f, flightRevenues[f->getFlightNumber()]});
    }

    // Sort using STL algorithm in descending order of revenue
    std::sort(revenueList.begin(), revenueList.end(), [](const auto& a, const auto& b) {
        return a.revenue > b.revenue;
    });

    std::cout << "\n" << Console::MAGENTA << "=======================================================\n"
              << "                 TOP REVENUE FLIGHTS                   \n"
              << "=======================================================\n" << Console::RESET;
    std::cout << Console::BOLD << Console::MAGENTA << std::left 
              << std::setw(15) << "Flight No"
              << std::setw(15) << "Type"
              << std::setw(15) << "Destination"
              << "Revenue Generated\n" << Console::RESET;
    std::cout << Console::MAGENTA << std::string(55, '-') << Console::RESET << "\n";

    int limit = std::min(5, static_cast<int>(revenueList.size()));
    for (int i = 0; i < limit; ++i) {
        const auto& item = revenueList[i];
        std::cout << std::left
                  << std::setw(15) << item.flightPtr->getFlightNumber()
                  << std::setw(15) << item.flightPtr->getFlightType()
                  << std::setw(15) << item.flightPtr->getDestination()
                  << Console::BOLD << Console::GREEN << "$" << std::fixed << std::setprecision(2) << item.revenue << Console::RESET << "\n";
    }
    std::cout << Console::MAGENTA << "=======================================================\n" << Console::RESET;
}

// File Handling - saveData
void Airline::saveData(const std::string& flightsFile, const std::string& passengersFile, const std::string& ticketsFile) const {
    // 1. Save Flights
    std::ofstream outFlights(flightsFile);
    if (!outFlights.is_open()) {
        throw DatabaseException("Saving Flights", "Could not open file: " + flightsFile);
    }
    for (const auto& f : flights) {
        outFlights << f->getFlightType() << "|"
                   << f->getFlightNumber() << "|"
                   << f->getOrigin() << "|"
                   << f->getDestination() << "|"
                   << f->getDepartureTime() << "|"
                   << f->getTotalSeats() << "|"
                   << f->getAvailableSeats() << "|";
        if (f->getFlightType() == "Domestic") {
            auto df = std::dynamic_pointer_cast<DomesticFlight>(f);
            outFlights << df->getBasePrice() << "|" << df->getDomesticTax();
        } else if (f->getFlightType() == "International") {
            auto inf = std::dynamic_pointer_cast<InternationalFlight>(f);
            outFlights << inf->getBasePrice() << "|" << inf->getIntlTax() << "|"
                       << inf->getFuelSurcharge() << "|" << (inf->getRequiresVisa() ? "1" : "0");
        } else if (f->getFlightType() == "Charter") {
            auto cf = std::dynamic_pointer_cast<CharterFlight>(f);
            outFlights << cf->getHourlyRate() << "|" << cf->getFlightHours() << "|" << cf->getOverheadFee();
        }
        outFlights << "\n";
    }
    outFlights.close();

    // 2. Save Passengers
    std::ofstream outPassengers(passengersFile);
    if (!outPassengers.is_open()) {
        throw DatabaseException("Saving Passengers", "Could not open file: " + passengersFile);
    }
    for (const auto& p : passengers) {
        outPassengers << p->getPassengerType() << "|"
                      << p->getPassengerId() << "|"
                      << p->getName() << "|"
                      << p->getEmail() << "\n";
    }
    outPassengers.close();

    // 3. Save Tickets
    std::ofstream outTickets(ticketsFile);
    if (!outTickets.is_open()) {
        throw DatabaseException("Saving Tickets", "Could not open file: " + ticketsFile);
    }
    for (const auto& t : tickets) {
        outTickets << t->getTicketId() << "|"
                   << t->getPassenger()->getPassengerId() << "|"
                   << t->getFlight()->getFlightNumber() << "|"
                   << t->getSeatNumber() << "|"
                   << t->getFarePaid() << "|"
                   << t->getBookingStatus() << "\n";
    }
    outTickets.close();
}

// File Handling - loadData
void Airline::loadData(const std::string& flightsFile, const std::string& passengersFile, const std::string& ticketsFile) {
    // Clear existing data to avoid duplicates upon reload
    flights.clear();
    passengers.clear();
    tickets.clear();

    // 1. Load Flights
    std::ifstream inFlights(flightsFile);
    if (!inFlights.is_open()) {
        throw DatabaseException("Loading Flights", "Could not open file: " + flightsFile);
    }
    std::string line;
    while (std::getline(inFlights, line)) {
        if (line.empty()) continue;
        auto tokens = split(line, '|');
        if (tokens.size() < 7) continue;

        std::string type = tokens[0];
        std::string fNo = tokens[1];
        std::string orig = tokens[2];
        std::string dest = tokens[3];
        std::string depTime = tokens[4];
        int totSeats = std::stoi(tokens[5]);
        int availSeats = std::stoi(tokens[6]);

        if (type == "Domestic" && tokens.size() >= 9) {
            double basePrice = std::stod(tokens[7]);
            double tax = std::stod(tokens[8]);
            auto df = std::make_shared<DomesticFlight>(fNo, orig, dest, depTime, totSeats, availSeats, basePrice, tax);
            flights.push_back(df);
        } else if (type == "International" && tokens.size() >= 11) {
            double basePrice = std::stod(tokens[7]);
            double tax = std::stod(tokens[8]);
            double surcharge = std::stod(tokens[9]);
            bool visa = (tokens[10] == "1");
            auto inf = std::make_shared<InternationalFlight>(fNo, orig, dest, depTime, totSeats, availSeats, basePrice, tax, surcharge, visa);
            flights.push_back(inf);
        } else if (type == "Charter" && tokens.size() >= 10) {
            double rate = std::stod(tokens[7]);
            double hours = std::stod(tokens[8]);
            double fee = std::stod(tokens[9]);
            auto cf = std::make_shared<CharterFlight>(fNo, orig, dest, depTime, totSeats, availSeats, rate, hours, fee);
            flights.push_back(cf);
        }
    }
    inFlights.close();

    // 2. Load Passengers
    std::ifstream inPassengers(passengersFile);
    if (!inPassengers.is_open()) {
        throw DatabaseException("Loading Passengers", "Could not open file: " + passengersFile);
    }
    while (std::getline(inPassengers, line)) {
        if (line.empty()) continue;
        auto tokens = split(line, '|');
        if (tokens.size() < 4) continue;

        std::string type = tokens[0];
        std::string id = tokens[1];
        std::string name = tokens[2];
        std::string email = tokens[3];

        if (type == "Economy") {
            passengers.push_back(std::make_shared<EconomyPassenger>(id, name, email));
        } else if (type == "Business") {
            passengers.push_back(std::make_shared<BusinessPassenger>(id, name, email));
        } else if (type == "FirstClass") {
            passengers.push_back(std::make_shared<FirstClassPassenger>(id, name, email));
        }
    }
    inPassengers.close();

    // 3. Load Tickets
    std::ifstream inTickets(ticketsFile);
    if (!inTickets.is_open()) {
        throw DatabaseException("Loading Tickets", "Could not open file: " + ticketsFile);
    }
    while (std::getline(inTickets, line)) {
        if (line.empty()) continue;
        auto tokens = split(line, '|');
        if (tokens.size() < 6) continue;

        std::string ticketId = tokens[0];
        std::string pId = tokens[1];
        std::string fNo = tokens[2];
        int seatNo = std::stoi(tokens[3]);
        double fare = std::stod(tokens[4]);
        std::string status = tokens[5];

        auto passenger = findPassenger(pId);
        auto flight = findFlight(fNo);

        if (passenger && flight) {
            auto ticket = std::make_shared<Ticket>(ticketId, passenger, flight, seatNo, fare, status);
            tickets.push_back(ticket);
            passenger->addTicketToHistory(ticket);
        }
    }
    inTickets.close();
}

void Airline::showSeatMap(const std::string& flightNo) const {
    auto flight = findFlight(flightNo);
    if (!flight) {
        throw InvalidInputException("Flight " + flightNo + " not found.");
    }
    int totalSeats = flight->getTotalSeats();
    
    // Scan occupied seats
    std::vector<bool> booked(totalSeats + 1, false);
    for (const auto& tkt : tickets) {
        if (tkt->getFlight()->getFlightNumber() == flightNo && tkt->getBookingStatus() == "Confirmed") {
            int s = tkt->getSeatNumber();
            if (s >= 1 && s <= totalSeats) {
                booked[s] = true;
            }
        }
    }

    std::cout << "\n" << UIHelper::BOLD << UIHelper::CYAN 
              << "=======================================================\n"
              << "             SEAT MAP FOR FLIGHT: " << flightNo << "\n"
              << "=======================================================\n" << UIHelper::RESET;
    std::cout << "  Row Letters (A-Z) and Column Numbers (1-3) represent seats.\n";
    std::cout << "  Available seats display code (e.g. A1). Booked seats marked 'X'.\n\n";

    int numRows = (totalSeats + 2) / 3;
    for (int r = 0; r < numRows; ++r) {
        char rowLetter = 'A' + r;
        std::cout << "  ";
        for (int c = 0; c < 3; ++c) {
            int seatNum = r * 3 + c + 1;
            if (seatNum <= totalSeats) {
                if (booked[seatNum]) {
                    std::cout << UIHelper::RED << "X " << UIHelper::RESET;
                } else {
                    std::cout << UIHelper::GREEN << rowLetter << (c + 1) << UIHelper::RESET;
                }
            } else {
                std::cout << "  ";
            }
            if (c < 2) {
                std::cout << "    "; // Spacing between columns
            }
        }
        std::cout << "\n";
    }
    std::cout << UIHelper::BOLD << UIHelper::CYAN 
              << "=======================================================\n" << UIHelper::RESET;
}

void Airline::showMonthlyRevenueReport(const std::string& monthYear) const {
    if (flights.empty()) {
        std::cout << "No flights registered in the system.\n";
        return;
    }

    // Map to aggregate revenues per flight for the specified month/year
    std::map<std::string, double> flightRevenues;
    for (const auto& f : flights) {
        flightRevenues[f->getFlightNumber()] = 0.0;
    }

    for (const auto& ticket : tickets) {
        if (ticket->getBookingStatus() == "Confirmed") {
            if (ticket->getFlight()->getDepartureTime().find(monthYear) != std::string::npos) {
                flightRevenues[ticket->getFlight()->getFlightNumber()] += ticket->getFarePaid();
            }
        }
    }

    struct FlightRevenue {
        std::shared_ptr<Flight> flightPtr;
        double revenue;
    };

    std::vector<FlightRevenue> revenueList;
    for (const auto& f : flights) {
        if (f->getDepartureTime().find(monthYear) != std::string::npos) {
            revenueList.push_back({f, flightRevenues[f->getFlightNumber()]});
        }
    }

    if (revenueList.empty()) {
        UIHelper::printWarningMessage("No flights scheduled for the month: " + monthYear);
        return;
    }

    // Sort using STL algorithm std::sort in descending order
    std::sort(revenueList.begin(), revenueList.end(), [](const auto& a, const auto& b) {
        return a.revenue > b.revenue;
    });

    std::cout << "\n" << UIHelper::BOLD << UIHelper::MAGENTA
              << "=======================================================\n"
              << "          MONTHLY REVENUE REPORT: " << monthYear << "\n"
              << "=======================================================\n" << UIHelper::RESET;
    std::cout << UIHelper::BOLD << UIHelper::MAGENTA << std::left
              << std::setw(12) << "Flight No"
              << std::setw(15) << "Type"
              << std::setw(15) << "Destination"
              << "Revenue Generated\n" << UIHelper::RESET;
    std::cout << UIHelper::MAGENTA << std::string(55, '-') << UIHelper::RESET << "\n";

    double totalMonthlyRevenue = 0.0;
    for (const auto& item : revenueList) {
        std::cout << std::left
                  << std::setw(12) << item.flightPtr->getFlightNumber()
                  << std::setw(15) << item.flightPtr->getFlightType()
                  << std::setw(15) << item.flightPtr->getDestination()
                  << UIHelper::BOLD << UIHelper::GREEN << "$" << std::fixed << std::setprecision(2) << item.revenue << UIHelper::RESET << "\n";
        totalMonthlyRevenue += item.revenue;
    }
    std::cout << UIHelper::MAGENTA << std::string(55, '-') << UIHelper::RESET << "\n"
              << "Total Monthly Revenue: " << UIHelper::BOLD << UIHelper::GREEN << "$" << totalMonthlyRevenue << UIHelper::RESET << "\n"
              << UIHelper::MAGENTA << "=======================================================\n" << UIHelper::RESET;
}

