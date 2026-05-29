# SkyLink Airline Reservation & Flight Management System
## 🎓 Comprehensive Viva Preparation & Architectural Blueprint

This guide serves as an in-depth viva-ready preparation sheet, detailing every Object-Oriented Programming (OOP) construct, architectural decision, design pattern, and C++17 component utilized in the SkyLink framework.

---

## 🏗️ Part 1: Detailed OOP Principles Explained

### 1. Abstraction
* **Theory**: Exposing only the essential interface of an object while hiding its background complex implementation details. This reduces cognitive overhead and simplifies overall system design.
* **In SkyLink**:
  - The base class `Flight` is an abstract base class (ABC) representing a general flight. It defines pure virtual functions (`calculateBaseFare() = 0`, `displayDetails() = 0`, `getFlightType() = 0`), meaning you cannot instantiate `Flight` directly.
  - The system controller (`Airline`) manipulates flights using `std::shared_ptr<Flight>` pointers. It does not need to know the specific type of flight when calculating base fares or listing details.

### 2. Encapsulation
* **Theory**: Wrapping state (member variables) and behavior (member functions) into a single logical unit (class) and restricting direct access to the state via access specifiers (`private`/`protected`), exposing controlled modification via public getters and setters.
* **In SkyLink**:
  - All variables in `Flight`, `Passenger`, and `Ticket` are declared `private` or `protected`.
  - Properties like `availableSeats` in a flight cannot be randomly decremented or set to illegal states by other components; they must go through the public member functions `bookSeat()` and `releaseSeat()`, which strictly validate capacity constraints.
  - Constructor parameters are validated immediately. If a base price is negative or an email is blank, the constructor throws an `InvalidInputException` preventing the instantiation of malformed objects.

### 3. Inheritance
* **Theory**: Establishing an "is-a" relationship between a base class (parent) and a derived class (child), allowing the derived class to inherit common attributes and behaviors while implementing its specific properties, promoting code reusability.
* **In SkyLink**:
  - `Flight` acts as a parent for `DomesticFlight`, `InternationalFlight`, and `CharterFlight`.
  - `Passenger` acts as a parent for `EconomyPassenger`, `BusinessPassenger`, and `FirstClassPassenger`.
  - This structure avoids code duplication. Common attributes (like `name`, `passengerId`, `email` in a passenger) are declared once in `Passenger` and inherited by all subclasses.

### 4. Runtime Polymorphism (Dynamic Binding)
* **Theory**: Enabling a single interface to behave differently depending on the actual runtime type of the object it points to. Dynamic binding is achieved in C++ through virtual member functions and virtual method tables (Vtables).
* **In SkyLink**:
  - Pointers of type `std::shared_ptr<Flight>` are stored in `std::vector<std::shared_ptr<Flight>>`.
  - When `flight->calculateBaseFare()` is invoked, the C++ runtime resolves the virtual call using the Vtable, calling `DomesticFlight::calculateBaseFare()` if it points to a domestic flight, or `InternationalFlight::calculateBaseFare()` for international ones.
  - **Virtual Destructors**: The virtual destructor `virtual ~Flight() = default;` is crucial. It ensures that when a base pointer `std::shared_ptr<Flight>` is deleted, the destructor of the derived class (e.g., `InternationalFlight`) is also called, preventing resource leaks.

---

## 🏛️ Part 2: Justification of Classes & Architecture

### 1. Flight Sub-system Hierarchy
* **Flight**: Abstract base class defining general flight state (`flightNumber`, `origin`, `destination`, etc.).
* **DomesticFlight**: Inherits `Flight`. Adds domestic tax calculations.
* **InternationalFlight**: Inherits `Flight`. Adds international fuel surcharges, custom taxes, and visa requirement flags.
* **CharterFlight**: Inherits `Flight`. Features corporate chartered hourly rate models.
* *Why Inheritance?* Common states (flight number, route, capacity) are defined once in the base `Flight` class, making adding new flight categories trivial.
* *Why Virtual Functions?* Dynamic pricing logic requires that each flight compute its fare polymorphically at runtime during tickets booking.

### 2. Passenger Sub-system Hierarchy
* **Passenger**: Abstract base class defining common traveler state (`passengerId`, `name`, `email`).
* **EconomyPassenger**: Inherits `Passenger`. Sets 20kg baggage limit, 1.0x loyalty discount, and 50% refund policy.
* **BusinessPassenger**: Inherits `Passenger`. Sets 35kg baggage limit, 1.5x loyalty discount, and 75% refund policy.
* **FirstClassPassenger**: Inherits `Passenger`. Sets 50kg baggage limit, 2.0x loyalty discount, and 90% refund policy.
* *Why Inheritance?* Allows storing any traveler type in a generic passenger collection while keeping class-specific baggage and cancellation refund rates encapsulated.
* *Why Virtual Functions?* Ensures cancellation refund percentages (`getCancellationRefundPercentage()`) are computed dynamically at runtime depending on the passenger's class type.

### 3. Ticket & The Rule of Five
* **Ticket**: Links a `Passenger` to a `Flight`. Encapsulates the seat allocated, the fare paid, and active status.
* **Rule of Five**: Since `Ticket` owns dependencies, we explicitly define:
  1. Destructor
  2. Copy Constructor
  3. Copy Assignment Operator
  4. Move Constructor
  5. Move Assignment Operator
  This demonstrates correct memory management and transfer of ownership values in standard modern C++.
* **Friend Operators / Operator Overloading**:
  - `operator==` is overloaded to check if a passenger is already booked on the same flight, enforcing duplicate booking rejection.
  - `operator<<` is overloaded as a `friend` function in `Ticket`, `Flight`, and `Passenger` to support clean stream insertion. In `Flight` and `Passenger`, we implement virtual `print` functions called from inside the friend `operator<<`, representing a **polymorphic stream insertion pattern**.

### 4. Airline (Controller Class)
* Aggregates collections of `Flight`, `Passenger`, and `Ticket` objects using STL containers. Provides API for booking, cancellation, file serialization (saving/loading), and live business analytics.

---

## 🛠️ Part 3: Memory Safety, Templates, Exceptions, and Files

### 1. Smart Pointer Memory Safety & Reference Loops
* We use `std::shared_ptr` to manage shared ownership of flights, passengers, and tickets.
* **Breaking Circular References**: If `Passenger` stored a list of `std::shared_ptr<Ticket>` and `Ticket` stored a `std::shared_ptr<Passenger>`, a reference cycle would occur. Neither object would ever be deleted, creating memory leaks. We resolve this by declaring `std::vector<std::weak_ptr<Ticket>> bookingHistory;` inside `Passenger.h`. The passenger holds a weak reference to the ticket, breaking the circular link.

### 2. STL (Standard Template Library) & Templates
* **Containers**:
  - `std::vector`: The primary sequential container used for flights, passengers, and tickets due to its dynamic sizing and contiguous memory efficiency.
  - `std::map`: Used to aggregate sales keying off flight numbers in revenue reports.
* **Algorithms**:
  - `std::find_if`: Used to search lists using custom lambda predicates (e.g., finding flights by ID).
  - `std::sort`: Used to sort flight revenues in descending order for monthly revenue reports.
* **Generic Template Search Utility**:
  - In `SearchTemplate.h`, we define an iterator-based generic template search utility:
    ```cpp
    template <typename InputIt, typename OutputIt, typename Predicate>
    OutputIt searchItems(InputIt first, InputIt last, OutputIt d_first, Predicate pred);
    ```
    This utility leverages standard library iterators and `std::copy_if` under the hood, enabling reuse across flights, passengers, or tickets.

### 3. Custom Exceptions
* Custom exceptions (`FlightFullException`, `DuplicateBookingException`, `InvalidCancellationException`, `InvalidInputException`, `DatabaseException`) inherit from `std::exception`. They provide specific exception handling, allowing the UI to catch them and show warning dialogs instead of crashing.

### 4. File Handling & Serialization
* Data is stored in plaintext files (`flights.txt`, `passengers.txt`, `tickets.txt`) inside `data/`.
* When saving, the polymorphic types are downcast using `std::dynamic_pointer_cast` to retrieve derived-specific fields.
* When loading, the type prefix token (e.g. `Domestic`) is parsed to construct the appropriate subclass. In the event of missing or corrupted save files, the database triggers default initialization and rebuilds sample records.

---

## 📊 Part 4: Mermaid UML Class Diagram

```mermaid
classDiagram
    class Flight {
        <<Abstract>>
        # string flightNumber
        # string origin
        # string destination
        # string departureTime
        # int totalSeats
        # int availableSeats
        + Flight(string, string, string, string, int, int)
        + ~Flight()*
        + calculateBaseFare() double*
        + displayDetails()* void*
        + getFlightType() string*
        + print(ostream&) void
        + getFlightNumber() string
        + getOrigin() string
        + getDestination() string
        + getDepartureTime() string
        + getTotalSeats() int
        + getAvailableSeats() int
        + setAvailableSeats(int) void
        + bookSeat() bool
        + releaseSeat() bool
    }

    class DomesticFlight {
        - double basePrice
        - double domesticTax
        + DomesticFlight(string, string, string, string, int, int, double, double)
        + calculateBaseFare() double
        + displayDetails() void
        + getFlightType() string
        + print(ostream&) void
        + getBasePrice() double
        + getDomesticTax() double
    }

    class InternationalFlight {
        - double basePrice
        - double intlTax
        - double fuelSurcharge
        - bool requiresVisa
        + InternationalFlight(string, string, string, string, int, int, double, double, double, bool)
        + calculateBaseFare() double
        + displayDetails() void
        + getFlightType() string
        + print(ostream&) void
        + getBasePrice() double
        + getIntlTax() double
        + getFuelSurcharge() double
        + getRequiresVisa() bool
    }

    class CharterFlight {
        - double hourlyRate
        - double flightHours
        - double overheadFee
        + CharterFlight(string, string, string, string, int, int, double, double, double)
        + calculateBaseFare() double
        + displayDetails() void
        + getFlightType() string
        + print(ostream&) void
        + getHourlyRate() double
        + getFlightHours() double
        + getOverheadFee() double
    }

    Flight <|-- DomesticFlight
    Flight <|-- InternationalFlight
    Flight <|-- CharterFlight

    class Passenger {
        <<Abstract>>
        # string passengerId
        # string name
        # string email
        # vector<weak_ptr<Ticket>> bookingHistory
        + Passenger(string, string, string)
        + ~Passenger()*
        + getPassengerId() string
        + getName() string
        + getEmail() string
        + getBaggageAllowance() double*
        + getLoyaltyMultiplier() double*
        + getCancellationRefundPercentage() double*
        + getPassengerType() string*
        + addTicketToHistory(shared_ptr<Ticket>) void
        + getBookingHistory() vector<shared_ptr<Ticket>>
        + displayDetails() void*
        + print(ostream&) void
    }

    class EconomyPassenger {
        + EconomyPassenger(string, string, string)
        + getBaggageAllowance() double
        + getLoyaltyMultiplier() double
        + getCancellationRefundPercentage() double
        + getPassengerType() string
        + print(ostream&) void
    }

    class BusinessPassenger {
        + BusinessPassenger(string, string, string)
        + getBaggageAllowance() double
        + getLoyaltyMultiplier() double
        + getCancellationRefundPercentage() double
        + getPassengerType() string
        + print(ostream&) void
    }

    class FirstClassPassenger {
        + FirstClassPassenger(string, string, string)
        + getBaggageAllowance() double
        + getLoyaltyMultiplier() double
        + getCancellationRefundPercentage() double
        + getPassengerType() string
        + print(ostream&) void
    }

    Passenger <|-- EconomyPassenger
    Passenger <|-- BusinessPassenger
    Passenger <|-- FirstClassPassenger

    class Ticket {
        - string ticketId
        - shared_ptr<Passenger> passenger
        - shared_ptr<Flight> flight
        - int seatNumber
        - double farePaid
        - string bookingStatus
        + Ticket(string, shared_ptr<Passenger>, shared_ptr<Flight>, int, double, string)
        + ~Ticket()
        + Ticket(const Ticket&)
        + operator=(const Ticket&) Ticket&
        + Ticket(Ticket&&)
        + operator=(Ticket&&) Ticket&
        + getTicketId() string
        + getPassenger() shared_ptr<Passenger>
        + getFlight() shared_ptr<Flight>
        + getSeatNumber() int
        + getFarePaid() double
        + getBookingStatus() string
        + setBookingStatus(string) void
        + operator==(const Ticket&) bool
    }

    class Airline {
        - vector<shared_ptr<Flight>> flights
        - vector<shared_ptr<Passenger>> passengers
        - vector<shared_ptr<Ticket>> tickets
        + Airline()
        + addFlight(shared_ptr<Flight>) void
        + removeFlight(string) bool
        + findFlight(string) shared_ptr<Flight>
        + listFlights() void
        + registerPassenger(shared_ptr<Passenger>) void
        + removePassenger(string) bool
        + findPassenger(string) shared_ptr<Passenger>
        + listPassengers() void
        + bookTicket(string, string, int) shared_ptr<Ticket>
        + showSeatMap(string) void
        + cancelTicket(string) void
        + listTickets() void
        + findTicket(string) shared_ptr<Ticket>
        + showTodayDepartures(string) void
        + showOccupancyPercentage() void
        + showTopRevenueFlights() void
        + showMonthlyRevenueReport(string) void
        + saveData(string, string, string) void
        + loadData(string, string, string) void
    }

    Airline "1" *-- "many" Flight : aggregates
    Airline "1" *-- "many" Passenger : aggregates
    Airline "1" *-- "many" Ticket : aggregates
    Ticket "many" o-- "1" Flight : references
    Ticket "many" o-- "1" Passenger : references
    Passenger "1" o-- "many" Ticket : history (weak references)
```

---

## 💬 Part 5: 16 Viva Questions with Top-Scoring Answers

#### Q1: What is the main difference between a Virtual Function and a Pure Virtual Function?
> **Answer**: A virtual function has a default implementation in the base class and can be optionally overridden in derived classes. A pure virtual function is declared with `= 0` at the end, has no implementation in the base class, and **must** be overridden by non-abstract derived classes.

#### Q2: What is an Abstract Class? Can we instantiate it?
> **Answer**: An abstract class is a class that contains at least one pure virtual function. It cannot be instantiated (we cannot create an object of this class). However, we can create pointers and references of its type to achieve runtime polymorphism.

#### Q3: Why is the destructor in your `Flight` base class declared `virtual`?
> **Answer**: If we delete a derived class object through a base class pointer and the destructor is not virtual, only the base class destructor will execute. The derived class destructor will be bypassed, resulting in resource leaks. Declaring it virtual ensures the derived class destructor runs first, followed by the base class destructor.

#### Q4: Explain the difference between `std::shared_ptr` and raw pointers.
> **Answer**: A raw pointer (`T*`) requires manual memory management (`new` and `delete`), which is prone to memory leaks and dangling pointers. `std::shared_ptr<T>` is a smart pointer that implements reference counting. It automatically deletes the managed object when the last `std::shared_ptr` pointing to it goes out of scope.

#### Q5: How did you implement duplicate booking prevention?
> **Answer**: I overloaded the `operator==` in the `Ticket` class. When a booking request is made, `Airline::bookTicket` constructs a temporary ticket and uses the `std::find_if` algorithm to scan the tickets collection. If a match is found, the system throws a `DuplicateBookingException`.

#### Q6: What is the VTable and VPtr? How is polymorphism resolved?
> **Answer**: Every class with virtual functions has a Virtual Table (VTable), which is a static array of function pointers. Every object of that class contains a hidden pointer called `vptr` pointing to the VTable. When a virtual function is called at runtime, the compiler follows `vptr` to resolve the function address dynamically.

#### Q7: Why did you use `std::dynamic_pointer_cast` in `saveData()`?
> **Answer**: When writing flights to disk, we must save derived-specific attributes (like fuel surcharges for international flights). Since they are stored as base pointers (`std::shared_ptr<Flight>`), we use `std::dynamic_pointer_cast` to safely downcast the pointer to the derived class type at runtime.

#### Q8: Explain what a Lambda function is and where you used it.
> **Answer**: A lambda function is an anonymous inline function. I used lambdas in combination with standard algorithms like `std::find_if` and inside my custom `searchItems` template to define the search filtering predicate directly at the call site.

#### Q9: What is exception safety? How does your system guarantee it?
> **Answer**: Exception safety ensures that when an error occurs and an exception is thrown, the system doesn't leak memory or leave the program in an inconsistent state. Since we use `std::shared_ptr` (RAII) and robust try-catch blocks, our system naturally avoids memory leaks even when exceptions are thrown.

#### Q10: Why is `std::vector` preferred over standard arrays?
> **Answer**: Vector is a dynamic container that handles resizing automatically, manages memory under the hood, provides bounds checking via `at()`, and keeps elements contiguous in memory, ensuring CPU cache friendliness and fast access.

#### Q11: Explain how the generic search template works.
> **Answer**: The generic `searchItems` template takes standard STL input and output iterators along with a unary predicate. It utilizes `std::copy_if` to copy matching objects to the target container. This allows it to search flights, passengers, or tickets generically.

#### Q12: How are files parsed during data loading?
> **Answer**: We read files line by line using `std::getline` and split them using a delimiter (`|`). The first token determines the type (e.g. `Economy`). Based on this type token, we call the appropriate constructor to instantiate the correct derived object.

#### Q13: What does the `noexcept` specifier mean? Where is it used?
> **Answer**: The `noexcept` specifier tells the compiler that a function will not throw an exception. It is used in custom exceptions on the overridden `const char* what() const noexcept` method to guarantee standard-compliant exception messages.

#### Q14: How does encapsulation protect system integrity?
> **Answer**: By marking variables like `availableSeats` as `protected` and exposing `bookSeat()` as a public method, we prevent outside classes from illegally modifying seat counts without performing proper capacity checks.

#### Q15: What is the advantage of utilizing `std::make_shared` over `new`?
> **Answer**: `std::make_shared` performs a single memory allocation for both the control block (reference count) and the managed object, whereas `new` requires two separate allocations. It is faster and improves memory locality.

#### Q16: What is a reference cycle and how is it broken in your code?
> **Answer**: A reference cycle occurs when two objects reference each other using `std::shared_ptr` (e.g., Passenger points to Ticket, and Ticket points to Passenger). Because the reference count never drops to zero, the objects are never deleted. We break this cycle by having the Passenger class store tickets in a `std::vector<std::weak_ptr<Ticket>>`.
