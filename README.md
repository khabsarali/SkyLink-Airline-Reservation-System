# SkyLink Airline Reservation & Flight Management System

SkyLink is a complete, production-grade C++17 console-based application designed using Object-Oriented Programming (OOP) principles. It offers comprehensive flight management, passenger registration, seat booking, ticket cancellation, persistence, and reporting capabilities.

---

## 📂 Project Structure

```text
SkyLink-Airline-Reservation-System/
│
├── include/                 # Header Files (.h)
│   ├── Airline.h            # Main airline system controller
│   ├── Flight.h             # Abstract Flight base class
│   ├── DomesticFlight.h     # Derived domestic flight class
│   ├── InternationalFlight.h# Derived international flight class
│   ├── CharterFlight.h      # Derived private charter flight class
│   ├── Passenger.h          # Abstract Passenger base class
│   ├── EconomyPassenger.h   # Derived economy passenger class
│   ├── BusinessPassenger.h  # Derived business passenger class
│   ├── FirstClassPassenger.h# Derived first class passenger class
│   ├── Ticket.h             # Passenger-flight link (boarding pass)
│   ├── Exceptions.h         # Custom Exception definitions
│   └── SearchTemplate.h     # Generic template-based search utility
│
├── src/                     # Source Files (.cpp)
│   ├── Airline.cpp
│   ├── Flight.cpp
│   ├── DomesticFlight.cpp
│   ├── InternationalFlight.cpp
│   ├── CharterFlight.cpp
│   ├── Passenger.cpp
│   ├── EconomyPassenger.cpp
│   ├── BusinessPassenger.cpp
│   ├── FirstClassPassenger.cpp
│   └── Ticket.cpp
│
├── data/                    # Database Directory (Persistent Storage)
│   ├── flights.txt          # Saved flights list
│   ├── passengers.txt       # Saved passengers list
│   └── tickets.txt          # Saved tickets list
│
├── docs/                    # Documentation
│   └── viva_notes.md        # Comprehensive viva preparation guide
│
├── main.cpp                 # Application entry point & console menu
├── Makefile                 # Automated compilation script
└── README.md                # Project documentation (this file)
```

---

## 🛠️ Compilation & Execution

This project is configured to build with **C++17** standard or higher.

### 1. Compile using `Makefile`
If you have `make` installed on your system (e.g., MinGW on Windows or GNU Make on Linux/macOS):
```bash
make
```

### 2. Compile directly with `g++`
Alternatively, you can compile all files in one command:
```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp main.cpp -o SkyLinkSystem
```

### 3. Run the application
Run the generated executable:
* **Windows**:
  ```cmd
  SkyLinkSystem.exe
  ```
* **Linux / macOS**:
  ```bash
  ./SkyLinkSystem
  ```

---

## 🚀 Key Features

1. **Robust Console Menu**: Styled clean interface with automated option selections and user input validation (rejecting non-integers, range errors, and empty strings).
2. **Polymorphic Pricing & Flight Structures**: Domestic, International, and Charter flights implement unique cost calculations (`calculateBaseFare()`).
3. **Multi-Class Passenger Structure**: Economy, Business, and First-Class passengers receive unique benefits, baggage allowances, loyalty multiplier discounts, and refund percentages.
4. **Validation Rules**:
   - **No Duplicate Bookings**: Uses overloaded `operator==` to reject identical booking requests.
   - **No Overselling**: Throws a custom `FlightFullException` when capacity is exceeded.
   - **Seat Allocation**: Automatic incremental seat numbering.
5. **Polymorphic Refunds**: Custom refunds (`getCancellationRefundPercentage()`) triggered upon ticket cancellation; throws `InvalidCancellationException` if ticket isn't active.
6. **Persistence (Save/Load)**: Automatic parsing of saved plain text database files on startup. Creates files inside the `data/` folder.
7. **Business Reports**:
   - Query flights departing on a specific date.
   - Live fleet occupancy statistics.
   - Top 5 revenue generating flights (uses maps and sorting).
8. **Generic Search Engine**: Implemented via a C++ template function in `SearchTemplate.h` filtering records with user-defined lambda predicates.
