# SkyLink Airline Reservation & Flight Management System Makefile
# Configured for g++ C++17 compilation on Windows and general environments

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

# Source files list
SRCS = src/Flight.cpp \
       src/DomesticFlight.cpp \
       src/InternationalFlight.cpp \
       src/CharterFlight.cpp \
       src/Passenger.cpp \
       src/EconomyPassenger.cpp \
       src/BusinessPassenger.cpp \
       src/Ticket.cpp \
       src/Airline.cpp \
       src/UIHelper.cpp \
       main.cpp

# Target executable name
TARGET = SkyLinkSystem.exe

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	@echo Cleaning up binary file...
	@if exist $(TARGET).exe del /F /Q $(TARGET).exe
	@if exist $(TARGET) del /F /Q $(TARGET)
