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
   - **Seat Allocation**: Automatic incremental seat numbering or manual seat selection via an interactive Seat Map.
5. **Polymorphic Refunds**: Custom refunds (`getCancellationRefundPercentage()`) triggered upon ticket cancellation; throws `InvalidCancellationException` if ticket isn't active.
6. **Persistence & Recovery**: Automatic parsing of saved database files on startup. Gracefully catches file missing/corruption errors, performing default database initialization to prevent crashes.
7. **Business Reports**:
   - Query flights departing on a specific date.
   - Live fleet occupancy statistics.
   - Top 5 revenue generating flights.
   - Monthly Revenue Reports dynamically sorted by revenue using STL sorting algorithms (`std::sort`).
8. **Generic Search Engine**: Redesigned template-based search utility in `SearchTemplate.h` utilizing standard STL iterators and predicates for filtering flights, passengers, and tickets.
9. **Seat Map Visualization**: Visual seating map displaying rows (A-Z) and columns (1-3) dynamically, marking booked seats as `X` and available seats with their codes in green.

---

## 💎 OOP Concepts Used

1. **Abstraction**: Handled using abstract base classes (`Flight` and `Passenger`) with pure virtual methods defining common interface schemas.
2. **Encapsulation**: Leveraged throughout the framework using private class properties, custom constructors validating data bounds, and public getters/setters.
3. **Inheritance**: Implemented for derived flight structures (`DomesticFlight`, `InternationalFlight`, `CharterFlight`) and traveler structures (`EconomyPassenger`, `BusinessPassenger`, `FirstClassPassenger`).
4. **Runtime Polymorphism**: Achieved via Vtable/Vptr method dispatching on virtual overrides (`calculateBaseFare()`, `getBaggageAllowance()`, `getCancellationRefundPercentage()`) and virtual polymorphic print formatting (`print()`).
5. **Memory Safety**: Enforced using smart pointers (`std::shared_ptr`, `std::unique_ptr`). We break reference loops between passengers and tickets by utilizing `std::weak_ptr` inside the passenger travel history collection.
6. **Rule of Five**: Explicitly implemented Destructor, Copy Constructor, Copy Assignment, Move Constructor, and Move Assignment inside the `Ticket` class for resource management.

---

## 🖼️ Screenshots

*Placeholders for console capture recordings:*
- **Admin System Dashboard**: `[Insert Admin Interface Screenshot here]`
- **Interactive Seat Map**: `[Insert Seat Map Layout Screenshot here]`
- **Boarding Pass Printout**: `[Insert Boarding Pass Output Screenshot here]`

---

## 🔮 Future Improvements

1. **Graphical User Interface (GUI)**: Transitioning from a command-line interface to a visual application using libraries like Qt or wxWidgets.
2. **Multi-Threaded Ticketing**: Adding lock protection (`std::mutex`) to make ticket booking safe against concurrent race conditions during network queries.
3. **Global Currency Exchange**: Integrating live currency converters for booking fares in multiple currencies.

---

## ⚠️ Known Limitations

1. **Flat File Database**: Plaintext files are used for persistence rather than transactional SQL engines.
2. **Synchronous File IO**: Saving data blocks execution temporarily while executing disk write calls.
