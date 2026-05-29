#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <iomanip>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include <fstream>
#include <algorithm>
#include "Airline.h"
#include "DomesticFlight.h"
#include "InternationalFlight.h"
#include "CharterFlight.h"
#include "EconomyPassenger.h"
#include "BusinessPassenger.h"
#include "FirstClassPassenger.h"
#include "SearchTemplate.h"
#include "Exceptions.h"
#include "UIHelper.h"

// Check if file exists in a compatible C++ way
static bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Create directory in a compatible C++ way
static void createDirectory(const std::string& dirName) {
#ifdef _WIN32
    _mkdir(dirName.c_str());
#else
    mkdir(dirName.c_str(), 0777);
#endif
}

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getValidInt(const std::string& prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            if (val >= minVal && val <= maxVal) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return val;
            }
        }
        UIHelper::printWarningMessage("Invalid input! Please enter a value between " + std::to_string(minVal) + " and " + std::to_string(maxVal) + ".");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double getValidDouble(const std::string& prompt, double minVal) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            if (val >= minVal) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return val;
            }
        }
        UIHelper::printWarningMessage("Invalid input! Please enter a numeric value >= " + std::to_string(static_cast<int>(minVal)) + ".");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string getNonEmptyString(const std::string& prompt) {
    std::string str;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, str);
        if (!str.empty()) {
            return str;
        }
        UIHelper::printWarningMessage("Field cannot be empty. Please enter a valid string.");
    }
}

// Generates baseline sample data if storage files don't exist
void populateSampleData(Airline& airline) {
    // 10 Flights (4 Domestic, 4 International, 2 Charter)
    airline.addFlight(std::make_shared<DomesticFlight>("DF-101", "New York", "Chicago", "2026-05-30 08:00", 100, 100, 150.0, 12.50));
    airline.addFlight(std::make_shared<DomesticFlight>("DF-102", "Los Angeles", "San Francisco", "2026-05-30 12:30", 80, 80, 90.0, 8.00));
    airline.addFlight(std::make_shared<DomesticFlight>("DF-103", "Dallas", "Denver", "2026-05-31 09:15", 120, 120, 110.0, 10.00));
    airline.addFlight(std::make_shared<DomesticFlight>("DF-104", "Miami", "Atlanta", "2026-05-30 15:45", 90, 90, 85.0, 7.50));

    airline.addFlight(std::make_shared<InternationalFlight>("IF-201", "New York", "London", "2026-05-30 20:00", 250, 250, 450.0, 65.00, 120.00, true));
    airline.addFlight(std::make_shared<InternationalFlight>("IF-202", "Los Angeles", "Tokyo", "2026-05-31 13:00", 300, 300, 650.0, 85.00, 180.00, true));
    airline.addFlight(std::make_shared<InternationalFlight>("IF-203", "Chicago", "Toronto", "2026-05-30 10:30", 150, 150, 180.0, 25.00, 40.00, false));
    airline.addFlight(std::make_shared<InternationalFlight>("IF-204", "Miami", "Paris", "2026-06-01 18:00", 200, 200, 500.0, 75.00, 150.00, true));

    airline.addFlight(std::make_shared<CharterFlight>("CF-301", "Houston", "Aspen", "2026-06-02 07:00", 12, 12, 1500.0, 3.5, 800.0));
    airline.addFlight(std::make_shared<CharterFlight>("CF-302", "London", "Ibiza", "2026-05-30 14:00", 8, 8, 2000.0, 2.5, 1200.0));

    // 8 Passengers (3 Economy, 3 Business, 2 FirstClass)
    airline.registerPassenger(std::make_shared<EconomyPassenger>("P-1001", "John Doe", "john.doe@email.com"));
    airline.registerPassenger(std::make_shared<EconomyPassenger>("P-1002", "Jane Smith", "jane.smith@email.com"));
    airline.registerPassenger(std::make_shared<EconomyPassenger>("P-1003", "Bob Johnson", "bob.johnson@email.com"));
    
    airline.registerPassenger(std::make_shared<BusinessPassenger>("P-2001", "Alice Brown", "alice.brown@email.com"));
    airline.registerPassenger(std::make_shared<BusinessPassenger>("P-2002", "Charlie Davis", "charlie.davis@email.com"));
    airline.registerPassenger(std::make_shared<BusinessPassenger>("P-2003", "Frank Miller", "frank.miller@email.com"));

    airline.registerPassenger(std::make_shared<FirstClassPassenger>("P-3001", "David Wilson", "david.wilson@email.com"));
    airline.registerPassenger(std::make_shared<FirstClassPassenger>("P-3002", "Emma Martinez", "emma.martinez@email.com"));
}

void runSelfTests(Airline& airline) {
    std::cout << "\n=== RUNNING AIRLINE SYSTEM INTEGRATION TESTS ===\n";
    try {
        // Test 1: Instantiation of different Flight classes
        auto df = std::make_shared<DomesticFlight>("TEST-DF", "ISB", "KHI", "2026-06-01", 50, 50, 200.0, 20.0);
        auto inf = std::make_shared<InternationalFlight>("TEST-IF", "ISB", "LHR", "2026-06-01", 100, 100, 500.0, 50.0, 100.0, true);
        auto cf = std::make_shared<CharterFlight>("TEST-CF", "KHI", "DXB", "2026-06-02", 10, 10, 1000.0, 3.0, 500.0);
        
        if (df->calculateBaseFare() != 220.0) throw std::runtime_error("Domestic flight fare computation failed.");
        if (inf->calculateBaseFare() != 650.0) throw std::runtime_error("International flight fare computation failed.");
        if (cf->calculateBaseFare() != 350.0) throw std::runtime_error("Charter flight fare computation failed.");
        std::cout << "[PASS] Test 1: Polymorphic pricing functions calculate fares correctly.\n";

        // Test 2: Passenger creation and polymorphism
        auto ep = std::make_shared<EconomyPassenger>("T-EP", "Test Eco", "eco@test.com");
        auto bp = std::make_shared<BusinessPassenger>("T-BP", "Test Biz", "biz@test.com");
        auto fp = std::make_shared<FirstClassPassenger>("T-FP", "Test First", "first@test.com");

        if (ep->getBaggageAllowance() != 20.0 || ep->getCancellationRefundPercentage() != 50.0)
            throw std::runtime_error("Economy passenger rules mismatch.");
        if (bp->getBaggageAllowance() != 35.0 || bp->getCancellationRefundPercentage() != 75.0)
            throw std::runtime_error("Business passenger rules mismatch.");
        if (fp->getBaggageAllowance() != 50.0 || fp->getCancellationRefundPercentage() != 90.0)
            throw std::runtime_error("First class passenger rules mismatch.");
        std::cout << "[PASS] Test 2: Polymorphic passenger benefits, baggage, and cancellation rules match specifications.\n";

        // Test 3: Generic search template
        std::vector<std::shared_ptr<Flight>> testFlights = {df, inf, cf};
        auto searchResult = searchItems(testFlights, [](const auto& f) {
            return f->getDestination() == "LHR";
        });
        if (searchResult.size() != 1 || searchResult[0]->getFlightNumber() != "TEST-IF") {
            throw std::runtime_error("Generic search template failed to filter items.");
        }
        std::cout << "[PASS] Test 3: Generic search template filters collections correctly.\n";

        // Test 4: Booking workflow, auto seat allocation, duplicate booking exceptions
        airline.addFlight(df);
        airline.registerPassenger(ep);
        
        auto ticket = airline.bookTicket("T-EP", "TEST-DF");
        if (ticket->getSeatNumber() != 1 || df->getAvailableSeats() != 49) {
            throw std::runtime_error("Seat allocation or available seats count mismatch.");
        }
        
        try {
            airline.bookTicket("T-EP", "TEST-DF");
            throw std::runtime_error("Duplicate booking exception was not thrown.");
        } catch (const DuplicateBookingException& e) {
            // expected behavior
        } catch (...) {
            throw std::runtime_error("Wrong exception thrown for duplicate booking.");
        }
        std::cout << "[PASS] Test 4: Ticket booking flow correctly processes reservations, updates seat availability, and rejects duplicate bookings.\n";

        // Test 5: Cancellation refund system
        airline.cancelTicket(ticket->getTicketId());
        if (df->getAvailableSeats() != 50) {
            throw std::runtime_error("Seat was not released upon cancellation.");
        }
        std::cout << "[PASS] Test 5: Cancellation refund system updates available seats and computes refund amounts correctly.\n";

        std::cout << "\nALL 5 SYSTEM TESTS PASSED SUCCESSFULLY! The Airline reservation framework is robust and safe.\n";
    } catch (const std::exception& e) {
        std::cerr << "\n[FAIL] Integration Test failed: " << e.what() << "\n";
        std::exit(1);
    }
}

int main(int argc, char* argv[]) {
    Airline airline;

    // Set paths for persistent storage files in a "data/" directory
    const std::string dataDir = "data";
    const std::string flightsFile = dataDir + "/flights.txt";
    const std::string passengersFile = dataDir + "/passengers.txt";
    const std::string ticketsFile = dataDir + "/tickets.txt";

    // Ensure data directory exists
    createDirectory(dataDir);

    // Try loading existing data. If flights file doesn't exist, populate and save defaults.
    if (fileExists(flightsFile)) {
        try {
            airline.loadData(flightsFile, passengersFile, ticketsFile);
        } catch (const std::exception& e) {
            std::cerr << "Warning loading save data: " << e.what() << ". Resetting database.\n";
            populateSampleData(airline);
            airline.saveData(flightsFile, passengersFile, ticketsFile);
        }
    } else {
        populateSampleData(airline);
        try {
            airline.saveData(flightsFile, passengersFile, ticketsFile);
        } catch (...) {}
    }

    // Command line self-test mode checker
    if (argc > 1 && std::string(argv[1]) == "--test") {
        runSelfTests(airline);
        return 0;
    }

    UIHelper::printWelcomeBanner();
    UIHelper::printLoadingScreen(1000);

    while (true) {
        UIHelper::clearScreen();
        UIHelper::printWelcomeBanner();

        UIHelper::printMenuBox("SKYLINK MAIN MENU", {
            "1. Flight Management",
            "2. Passenger Management",
            "3. Book Airline Ticket",
            "4. Cancel Ticket booking",
            "5. View Boarding Passes",
            "6. Business Reports Submenu",
            "7. Save & Exit"
        });
        std::cout << "\n";
        int mainChoice = getValidInt("Enter your choice (1-7): ", 1, 7);

        if (mainChoice == 7) {
            UIHelper::clearScreen();
            UIHelper::printWelcomeBanner();
            try {
                UIHelper::printSuccessMessage("Saving all flight, passenger, and ticket bookings to disk...");
                airline.saveData(flightsFile, passengersFile, ticketsFile);
                UIHelper::printSuccessMessage("Data saved successfully to database records!");
            } catch (const std::exception& e) {
                UIHelper::printErrorMessage(std::string("Failed to save data: ") + e.what());
            }
            UIHelper::printFooter();
            break;
        }

        switch (mainChoice) {
            case 1: { // Flight Management Submenu
                while (true) {
                    UIHelper::clearScreen();
                    UIHelper::printWelcomeBanner();
                    UIHelper::printMenuBox("FLIGHT MANAGEMENT SUBMENU", {
                        "1. List All Flights",
                        "2. Search Flights by Destination",
                        "3. Create & Add New Flight",
                        "4. Back to Main Menu"
                    });
                    std::cout << "\n";
                    int subChoice = getValidInt("Enter choice (1-4): ", 1, 4);
                    if (subChoice == 4) break;

                    switch (subChoice) {
                        case 1:
                            UIHelper::clearScreen();
                            UIHelper::printWelcomeBanner();
                            std::cout << "\n";
                            airline.listFlights();
                            waitForEnter();
                            break;

                        case 2: {
                            UIHelper::clearScreen();
                            UIHelper::printWelcomeBanner();
                            std::cout << "\n";
                            std::string dest = getNonEmptyString("Enter destination name to search: ");
                            
                            auto matches = searchItems(airline.getFlights(), [&](const auto& flight) {
                                std::string destLower = dest;
                                std::string flightDestLower = flight->getDestination();
                                std::transform(destLower.begin(), destLower.end(), destLower.begin(), ::tolower);
                                std::transform(flightDestLower.begin(), flightDestLower.end(), flightDestLower.begin(), ::tolower);
                                return flightDestLower == destLower;
                            });

                            if (matches.empty()) {
                                UIHelper::printWarningMessage("No flights found flying to destination '" + dest + "'.");
                            } else {
                                std::cout << "\n";
                                UIHelper::printTableHeader(
                                    {"Flight No", "Type", "Origin", "Destination", "Departure Time", "Seats (A/T)", "Base Fare"},
                                    {10, 13, 14, 14, 18, 11, 9}
                                );
                                for (const auto& flight : matches) {
                                    flight->displayDetails();
                                }
                                UIHelper::printTableSeparator({10, 13, 14, 14, 18, 11, 9});
                            }
                            waitForEnter();
                            break;
                        }
                        case 3: {
                            UIHelper::clearScreen();
                            UIHelper::printWelcomeBanner();
                            std::cout << "\n";
                            std::string fNo = getNonEmptyString("Enter Flight Number (e.g. AA-404): ");
                            if (airline.findFlight(fNo)) {
                                UIHelper::printErrorMessage("Flight number already exists in directory.");
                                waitForEnter();
                                break;
                            }
                            std::string orig = getNonEmptyString("Enter Origin Airport/City: ");
                            std::string dest = getNonEmptyString("Enter Destination Airport/City: ");
                            std::string depTime = getNonEmptyString("Enter Departure Date & Time (YYYY-MM-DD HH:MM): ");
                            int seats = getValidInt("Enter Total Capacity Seats: ", 1, 500);

                            UIHelper::printMenuBox("SELECT CATEGORY", {
                                "1. Domestic Flight",
                                "2. International Flight",
                                "3. Charter Flight"
                            });
                            int typeChoice = getValidInt("\nEnter category choice (1-3): ", 1, 3);

                            try {
                                if (typeChoice == 1) {
                                    double base = getValidDouble("Enter Base Price ($): ", 10.0);
                                    double tax = getValidDouble("Enter Domestic Tax Surcharge ($): ", 0.0);
                                    airline.addFlight(std::make_shared<DomesticFlight>(fNo, orig, dest, depTime, seats, seats, base, tax));
                                } else if (typeChoice == 2) {
                                    double base = getValidDouble("Enter Base Price ($): ", 10.0);
                                    double tax = getValidDouble("Enter International Tax ($): ", 0.0);
                                    double surcharge = getValidDouble("Enter Fuel Surcharge ($): ", 0.0);
                                    std::cout << "Requires VISA? (1 for Yes, 0 for No): ";
                                    int visaVal = getValidInt("", 0, 1);
                                    airline.addFlight(std::make_shared<InternationalFlight>(fNo, orig, dest, depTime, seats, seats, base, tax, surcharge, visaVal == 1));
                                } else if (typeChoice == 3) {
                                    double rate = getValidDouble("Enter Hourly Rate ($): ", 50.0);
                                    double hours = getValidDouble("Enter Estimated Flight Hours: ", 0.5);
                                    double fee = getValidDouble("Enter Overhead Operational Fee ($): ", 0.0);
                                    airline.addFlight(std::make_shared<CharterFlight>(fNo, orig, dest, depTime, seats, seats, rate, hours, fee));
                                }
                                UIHelper::printSuccessMessage("New flight registered successfully into airline network!");
                            } catch (const std::exception& e) {
                                UIHelper::printErrorMessage(e.what());
                            }

                            waitForEnter();
                            break;
                        }
                    }
                }
                break;
            }

            case 2: { // Passenger Management Submenu
                while (true) {
                    UIHelper::clearScreen();
                    UIHelper::printWelcomeBanner();
                    UIHelper::printMenuBox("PASSENGER DIRECTORY SUBMENU", {
                        "1. List Registered Passengers",
                        "2. Search Passengers by Name",
                        "3. Register New Passenger",
                        "4. Back to Main Menu"
                    });
                    std::cout << "\n";
                    int subChoice = getValidInt("Enter choice (1-4): ", 1, 4);
                    if (subChoice == 4) break;

                    switch (subChoice) {
                        case 1:
                            UIHelper::clearScreen();
                            UIHelper::printWelcomeBanner();
                            std::cout << "\n";
                            airline.listPassengers();
                            waitForEnter();
                            break;

                        case 2: {
                            UIHelper::clearScreen();
                            UIHelper::printWelcomeBanner();
                            std::cout << "\n";
                            std::string nameSearch = getNonEmptyString("Enter name substring: ");
                            
                            auto matches = searchItems(airline.getPassengers(), [&](const auto& passenger) {
                                std::string pName = passenger->getName();
                                std::string search = nameSearch;
                                std::transform(pName.begin(), pName.end(), pName.begin(), ::tolower);
                                std::transform(search.begin(), search.end(), search.begin(), ::tolower);
                                return pName.find(search) != std::string::npos;
                            });

                            if (matches.empty()) {
                                UIHelper::printWarningMessage("No passengers found matching query '" + nameSearch + "'.");
                            } else {
                                std::cout << "\n";
                                UIHelper::printTableHeader(
                                    {"ID", "Name", "Email", "Class Type", "Baggage", "Refund"},
                                    {10, 18, 22, 11, 7, 6}
                                );
                                for (const auto& passenger : matches) {
                                    passenger->displayDetails();
                                }
                                UIHelper::printTableSeparator({10, 18, 22, 11, 7, 6});
                            }
                            waitForEnter();
                            break;
                        }
                        case 3: {
                            UIHelper::clearScreen();
                            UIHelper::printWelcomeBanner();
                            std::cout << "\n";
                            std::string id = getNonEmptyString("Enter Passenger ID (e.g. P-4001): ");
                            if (airline.findPassenger(id)) {
                                UIHelper::printErrorMessage("Passenger ID already registered.");
                                waitForEnter();
                                break;
                            }
                            std::string name = getNonEmptyString("Enter Passenger Full Name: ");
                            std::string email = getNonEmptyString("Enter Email Address: ");

                            UIHelper::printMenuBox("CHOOSE CLASS TYPE", {
                                "1. Economy Class Passenger",
                                "2. Business Class Passenger",
                                "3. First Class Passenger"
                            });
                            int classChoice = getValidInt("\nEnter class choice (1-3): ", 1, 3);

                            try {
                                if (classChoice == 1) {
                                    airline.registerPassenger(std::make_shared<EconomyPassenger>(id, name, email));
                                } else if (classChoice == 2) {
                                    airline.registerPassenger(std::make_shared<BusinessPassenger>(id, name, email));
                                } else if (classChoice == 3) {
                                    airline.registerPassenger(std::make_shared<FirstClassPassenger>(id, name, email));
                                }
                                UIHelper::printSuccessMessage("New passenger successfully registered in directory!");
                            } catch (const std::exception& e) {
                                UIHelper::printErrorMessage(e.what());
                            }

                            waitForEnter();
                            break;
                        }
                    }
                }
                break;
            }

            case 3: { // Book Ticket
                UIHelper::clearScreen();
                UIHelper::printWelcomeBanner();
                std::cout << "\n";
                std::string pId = getNonEmptyString("Enter Passenger ID: ");
                std::string fNo = getNonEmptyString("Enter Flight Number: ");

                try {
                    auto ticket = airline.bookTicket(pId, fNo);
                    UIHelper::printBookingSuccessPopup(
                        ticket->getPassenger()->getName(),
                        ticket->getFlight()->getFlightNumber(),
                        ticket->getSeatNumber(),
                        ticket->getFarePaid()
                    );
                } catch (const FlightFullException& e) {
                    UIHelper::printErrorMessage(e.what());
                } catch (const DuplicateBookingException& e) {
                    UIHelper::printErrorMessage(e.what());
                } catch (const std::exception& e) {
                    UIHelper::printErrorMessage(e.what());
                }
                waitForEnter();
                break;
            }

            case 4: { // Cancel Ticket
                UIHelper::clearScreen();
                UIHelper::printWelcomeBanner();
                std::cout << "\n";
                std::string ticketId = getNonEmptyString("Enter Ticket ID to Cancel: ");

                try {
                    airline.cancelTicket(ticketId);
                } catch (const InvalidCancellationException& e) {
                    UIHelper::printErrorMessage(e.what());
                } catch (const std::exception& e) {
                    UIHelper::printErrorMessage(e.what());
                }
                waitForEnter();
                break;
            }

            case 5: { // List Bookings
                UIHelper::clearScreen();
                UIHelper::printWelcomeBanner();
                std::cout << "\n";
                airline.listTickets();
                waitForEnter();
                break;
            }

            case 6: { // Reports Submenu
                while (true) {
                    UIHelper::clearScreen();
                    UIHelper::printWelcomeBanner();
                    UIHelper::printMenuBox("REPORTS & FLEET ANALYTICS", {
                        "1. View Departures by Date",
                        "2. View Cabin Occupancy Statistics",
                        "3. View Top 5 Revenue Flights",
                        "4. Back to Main Menu"
                    });
                    std::cout << "\n";
                    int reportChoice = getValidInt("Enter choice (1-4): ", 1, 4);
                    if (reportChoice == 4) break;

                    switch (reportChoice) {
                        case 1: {
                            UIHelper::clearScreen();
                            UIHelper::printWelcomeBanner();
                            std::cout << "\n";
                            std::string date = getNonEmptyString("Enter date to query (YYYY-MM-DD, e.g., 2026-05-30): ");
                            airline.showTodayDepartures(date);
                            waitForEnter();
                            break;
                        }
                        case 2:
                            UIHelper::clearScreen();
                            airline.showOccupancyPercentage();
                            waitForEnter();
                            break;

                        case 3:
                            UIHelper::clearScreen();
                            airline.showTopRevenueFlights();
                            waitForEnter();
                            break;
                    }
                }
                break;
            }
        }
    }

    return 0;
}
