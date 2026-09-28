# 🌟 SkyLink Airline Reservation & Fleet Management System

**A Pure Object-Oriented C++17 Architecture with a Modern Swiss Monochrome Front-End UI/UX.**

---

## 🏗️ Project Architecture & OOP Hierarchy

SkyLink is built upon core Object-Oriented Programming (OOP) principles: **Abstraction**, **Encapsulation**, **Inheritance**, and **Runtime Polymorphism**.

### 1. Flight Hierarchy
- **`Flight` (Abstract Base Class)**: Base interface defining `calculateBaseFare()`, `displayDetails()`, `getFlightType()`, and seat management methods (`bookSeat()`, `releaseSeat()`).
  - **`DomesticFlight`**: Inherits `Flight`. Adds domestic tax computation.
  - **`InternationalFlight`**: Inherits `Flight`. Adds international taxes, fuel surcharges, and visa requirement flags.
  - **`CharterFlight`**: Inherits `Flight`. Adds private charter hourly rates, flight hours, and overhead catering fees. Base fare is calculated polymorphically as `(hourlyRate * flightHours + overheadFee) / totalSeats`.

### 2. Passenger Hierarchy
- **`Passenger` (Abstract Base Class)**: Encapsulates passenger ID, name, and email with pure virtual benefit methods.
  - **`EconomyPassenger`**: Standard 20kg baggage limit, 1.0x loyalty multiplier, 50% cancellation refund policy. **Can select and book Chartered Flights directly.**
  - **`BusinessPassenger`**: 35kg baggage limit, 1.5x loyalty multiplier, 75% cancellation refund policy, priority boarding privileges.

### 3. Association & Controller
- **`Ticket`**: Links a `Passenger` to a `Flight` with seat allocation, fare paid, and booking status. Overloads `operator==` for duplicate booking prevention and `operator<<` for polymorphic stream output.
- **`Airline`**: Aggregate controller managing STL vectors of `Flight`, `Passenger`, and `Ticket` objects with file persistence (`flights.txt`, `passengers.txt`, `tickets.txt`).

---

## 📂 Project Structure

```
SkyLink-Airline-Reservation-System/
│
├── include/                 # C++ Header Files (.h)
│   ├── Airline.h            # Controller class definition
│   ├── Flight.h             # Abstract Flight base class
│   ├── DomesticFlight.h     # Domestic flight class
│   ├── InternationalFlight.h# International flight class
│   ├── CharterFlight.h      # Chartered flight class (Private / VIP)
│   ├── Passenger.h          # Abstract Passenger base class
│   ├── EconomyPassenger.h   # Economy passenger class
│   ├── BusinessPassenger.h  # Business passenger class
│   ├── Ticket.h             # Ticket class with overloaded operators
│   ├── Exceptions.h         # Custom AirlineException handler
│   ├── SearchTemplate.h     # Generic template search algorithm
│   └── UIHelper.h           # Swiss monochrome terminal UI helper
│
├── src/                     # C++ Implementation Files (.cpp)
│   ├── Airline.cpp
│   ├── Flight.cpp
│   ├── DomesticFlight.cpp
│   ├── InternationalFlight.cpp
│   ├── CharterFlight.cpp
│   ├── Passenger.cpp
│   ├── EconomyPassenger.cpp
│   ├── BusinessPassenger.cpp
│   ├── Ticket.cpp
│   └── UIHelper.cpp
│
├── data/                    # Persistent storage (flat text files)
│   ├── flights.txt          # Domestic, International, & Chartered flights
│   ├── passengers.txt       # Economy & Business passenger records
│   └── tickets.txt          # Confirmed and cancelled boarding passes
│
├── index.html               # Modern Swiss monochrome Web Application
├── styles.css               # Strict monochrome responsive CSS design system
├── app.js                   # Pure OOP JavaScript front-end engine
├── main.cpp                 # C++ main entry point & menu driver
├── Makefile                 # C++17 g++ build script (-O2 optimization)
├── VIVA_NOTES.md            # Comprehensive viva examination guide & UML
└── README.md                # Project documentation
```

---

## 🎨 Front-End UI/UX: Swiss Monochrome Design System

The front-end user interface has been completely redesigned with an editorial, Swiss-inspired minimalist aesthetic:
- **Strict Monochrome Palette**: `#000000`, `#111111`, `#333333`, `#666666`, `#999999`, `#E5E5E5`, `#F5F5F5`, `#FFFFFF`.
- **Modern Typography**: Clear visual hierarchy using `Plus Jakarta Sans` and `Space Mono`.
- **Multi-Step Reservation Wizard**:
  1. *Search & Filter*: Real-time route and flight category filtering.
  2. *Flight Selection*: High-contrast flight cards displaying route vectors and dynamic fares.
  3. *Passenger & Class Details*: Profile management with live benefit badges (Economy passengers can select Chartered Flights).
  4. *Seat Map Allocation*: Interactive 3-abreast aircraft seating layout with visual occupancy markers.
  5. *Boarding Pass Verification*: Swiss ticket layout with tear-off barcode stub.
  6. *Confirmation Screen*: Instant booking reference generation with print capability.
- **Responsive Layout**: Fluid experience across Desktop, Laptop, Tablet, and Mobile screens.

---

## 🛠️ Compilation & Execution

### Option 1: C++ Terminal Application

#### 1. Compile the C++ system (g++ C++17)
```powershell
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude src/Flight.cpp src/DomesticFlight.cpp src/InternationalFlight.cpp src/CharterFlight.cpp src/Passenger.cpp src/EconomyPassenger.cpp src/BusinessPassenger.cpp src/Ticket.cpp src/Airline.cpp src/UIHelper.cpp main.cpp -o SkyLinkSystem.exe
```

#### 2. Run the System Tests
```powershell
.\SkyLinkSystem.exe --test
```

#### 3. Launch Interactive Console Menu
```powershell
.\SkyLinkSystem.exe
```

---

### Option 2: Modern Web Application Front-End

Simply open `index.html` in any modern web browser:
```powershell
Start-Process index.html
```

---

## 🚀 Key Features

1. **Chartered Flight Integration**: Universal access allowing Economy passengers to book Chartered flights with exact fare calculation.
2. **Polymorphic Pricing & Benefits**: Distinct baggage limits, loyalty multipliers, and cancellation refund percentages resolved dynamically at runtime.
3. **Interactive & Auto Seat Allocation**: Automatic first-available seat assignment or interactive seat selection.
4. **Duplicate Booking Prevention**: Overloaded `operator==` ensures passengers cannot double-book identical flights.
5. **Real-time Analytics**: Fleet occupancy rates, departures today, and top revenue route leaderboards.
6. **Data Persistence**: Automatic serialization/deserialization to and from `data/` storage files and browser `localStorage`.
