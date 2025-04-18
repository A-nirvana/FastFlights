#include "FlightManager.hpp"
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
        std::string id, from, to, time, arrival, dur;
        int total, available, prc;

        getline(ss, id, ',');
        getline(ss, from, ',');
        getline(ss, to, ',');
        getline(ss, time, ',');
        getline(ss, arrival, ',');
        getline(ss, dur, ',');
        ss >> total;
        ss.ignore(); 
        ss >> available;
        ss.ignore(); 
        ss >> prc;

        flights.emplace_back(id, from, to, time, arrival, dur, total, available,prc);
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
             << flight.getArrivalTime() << ","
             << flight.getDuration() << ","
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
const std::vector<Flight>& FlightManager::getAllFlights() const {
    return flights;
}

void FlightManager::loadReservationsToUpdateSeats(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string resID, username, flightID, date, seatStr;

        std::getline(ss, resID, ',');
        std::getline(ss, username, ',');
        std::getline(ss, flightID, ',');
        std::getline(ss, date, ',');
        std::getline(ss, seatStr, ',');

        Flight* flight = getFlightByID(flightID);
        if (flight) {
            flight->assignSeatOnDate(date);  // Updates seat tracking for that date
        }
    }

    file.close();
}



