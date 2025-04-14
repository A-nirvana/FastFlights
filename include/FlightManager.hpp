#ifndef FLIGHTMANAGER_HPP
#define FLIGHTMANAGER_HPP

#include <vector>
#include "Flight.hpp"

class FlightManager {
private:
    std::vector<Flight> flights;

public:
    FlightManager();

    void loadFlightsFromFile(const std::string& filename);
    void saveFlightsToFile(const std::string& filename);

    void addFlight(const Flight& flight);
    void removeFlight(const std::string& flightID);
    Flight* getFlightByID(const std::string& flightID);
    void displayAllFlights() const;
};

#endif
