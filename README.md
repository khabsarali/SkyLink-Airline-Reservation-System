# SkyLink Airline Reservation & Flight Management System

SkyLink is a complete, beginner-friendly C++17 console-based application designed using core Object-Oriented Programming (OOP) principles. It is specifically structured for a university OOP assignment, offering clean, highly readable multi-file code that a student can easily explain in a viva exam.

It supports flight management, passenger registration, seat booking, ticket cancellation, persistence, and reporting capabilities.

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
│   ├── Passenger.h          # Abstract Passenger base class
│   ├── EconomyPassenger.h   # Derived economy passenger class
│   ├── BusinessPassenger.h  # Derived business passenger class
│   ├── Ticket.h             # Passenger-flight link (boarding pass)
│   ├── Exceptions.h         # Custom exception definition (AirlineException)
│   └── SearchTemplate.h     # Simple, beginner-friendly template filter function
│
├── src/                     # Source Files (.cpp)
│   ├── Airline.cpp
│   ├── Flight.cpp
│   ├── DomesticFlight.cpp
│   ├── InternationalFlight.cpp
│   ├── Passenger.cpp
│   ├── EconomyPassenger.cpp
│   ├── BusinessPassenger.cpp
│   └── Ticket.cpp
│
├── data/                    # Database Directory (Persistent Storage)
│   ├── flights.txt          # Saved flights list
│   ├── passengers.txt       # Saved passengers list
│   └── tickets.txt          # Saved tickets list
│
├── main.cpp                 # Application entry point & console menu
├── Makefile                 # Automated compilation script
├── VIVA_NOTES.md            # Comprehensive viva preparation guide & UML Class Diagram
└── README.md                # Project documentation (this file)
```

---

## 🛠️ Compilation & Execution

This project is configured to build with **C++17** standard or higher.

### 1. Compile directly with `g++`
You can compile all files easily in one command:
```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp main.cpp -o SkyLinkSystem
```

### 2. Run the application
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
2. **Polymorphic Pricing & Flight Structures**: Domestic and International flights implement unique cost calculations (`calculateBaseFare()`).
3. **Multi-Class Passenger Structure**: Economy and Business passengers receive unique benefits, baggage allowances, loyalty multiplier discounts, and refund percentages.
4. **Validation Rules**:
   - **No Duplicate Bookings**: Uses overloaded `operator==` to reject identical booking requests.
   - **No Overselling**: Throws a custom `AirlineException` when capacity is exceeded.
   - **Seat Allocation**: Automatic incremental seat numbering or manual seat selection via an interactive Seat Map.
5. **Polymorphic Refunds**: Custom refunds (`getCancellationRefundPercentage()`) triggered upon ticket cancellation; throws `AirlineException` if the ticket isn't active.
6. **Persistence & Recovery**: Automatic parsing of saved database files on startup. Gracefully catches file missing/corruption errors, performing default database initialization to prevent crashes.
7. **Business Reports**:
   - Query flights departing on a specific date.
   - Live fleet occupancy statistics.
   - Top revenue generating flights.
   - Monthly Revenue Reports dynamically sorted by revenue using STL sorting algorithms (`std::sort`).
8. **Generic Search Engine**: A simple template-based search utility in `SearchTemplate.h` that is extremely easy to explain in a viva.
9. **Seat Map Visualization**: Visual seating map displaying rows (A-Z) and columns (1-3) dynamically, marking booked seats as `X` and available seats with their codes in green.

---

## 💎 OOP Concepts Used

1. **Abstraction**: Handled using abstract base classes (`Flight` and `Passenger`) with pure virtual methods defining common interface schemas.
2. **Encapsulation**: Leveraged throughout the framework using private class properties, custom constructors validating data bounds, and public getters/setters.
3. **Inheritance**: Implemented for derived flight structures (`DomesticFlight`, `InternationalFlight`) and traveler structures (`EconomyPassenger`, `BusinessPassenger`).
4. **Runtime Polymorphism**: Achieved via Vtable/Vptr method dispatching on virtual overrides (`calculateBaseFare()`, `getBaggageAllowance()`, `getCancellationRefundPercentage()`) and virtual polymorphic print formatting (`print()`).
5. **Memory Safety**: Enforced using smart pointers (`std::shared_ptr`). Circular references are naturally avoided by dynamically querying bookings instead of holding nested cycles.
6. **Rule of Zero**: Follows modern C++ best practices. Since `Ticket` properties are managed by standard containers and smart pointers, we rely on compiler-generated default copy, move, and destruction semantics.

---

## 🔮 Future Improvements

1. **Graphical User Interface (GUI)**: Transitioning from a command-line interface to a visual application using libraries like Qt or wxWidgets.
2. **Multi-Threaded Ticketing**: Adding lock protection (`std::mutex`) to make ticket booking safe against concurrent race conditions during network queries.

---

## ⚠️ Known Limitations

1. **Flat File Database**: Plaintext files are used for persistence rather than transactional SQL engines.
2. **Synchronous File IO**: Saving data blocks execution temporarily while executing disk write calls.
