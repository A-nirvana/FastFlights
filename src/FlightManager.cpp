#include "../include/FlightManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

FlightManager::FlightManager() {}

void FlightManager::loadFlightsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    flights.clear();
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id, from, to, time;
        int total, available, prc;

        getline(ss, id, ',');
        getline(ss, from, ',');
        getline(ss, to, ',');
        getline(ss, time, ',');
        ss >> total;
        ss.ignore(); 
        ss >> available;
        ss.ignore(); 
        ss >> prc;

        flights.emplace_back(id, from, to, time, total, available,prc);
    }

    file.close();
}

void FlightManager::saveFlightsToFile(const std::string& filename) {
    std::ofstream file(filename);

    for (const auto& flight : flights) {
        file << flight.getFlightID() << ","
             << flight.getOrigin() << ","
             << flight.getDestination() << ","
             << flight.getDeparatureTime() << ","
             << flight.getTotalSeats() << ","
             << flight.getAvailableSeats() << ","
             << flight.getPrice() << "\n";
    }

    file.close();
}

void FlightManager::addFlight(const Flight& flight) {
    flights.push_back(flight);
}

void FlightManager::removeFlight(const std::string& flightID) {
    for (auto it = flights.begin(); it != flights.end(); ++it) {
        if (it->getFlightID() == flightID) {
            flights.erase(it);
            break;
        }
    }
}

Flight* FlightManager::getFlightByID(const std::string& flightID) {
    for (auto& flight : flights) {
        if (flight.getFlightID() == flightID) {
            return &flight;
        }
    }
    return nullptr;
}

void FlightManager::displayAllFlights() const {
    for (const auto& flight : flights) {
        flight.displayFlight();
        std::cout << "---------------------------\n";
    }
}

