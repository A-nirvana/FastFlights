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
    const std::vector<Flight>& getAllFlights() const;
    void addFlight(const Flight& flight);
    void removeFlight(const std::string& flightID);
    Flight* getFlightByID(const std::string& flightID);
    void displayAllFlights() const;
    void loadReservationsToUpdateSeats(const std::string& filename);
    bool hasScheduleConflict(const std::string& origin, const std::string& destination, const std::string& departureTime);
    bool isScheduledAtTheSameTime(const std::string& origin, const std::string& departureTime);
    bool displayFlightsFromOriginSorted(const std::string& origin);



};

#endif
