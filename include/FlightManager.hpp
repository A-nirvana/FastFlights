#ifndef FLIGHTMANAGER_HPP
#define FLIGHTMANAGER_HPP

#include <vector>
#include "Flight.hpp"

using namespace std;

class FlightManager {
private:
    vector<Flight> flights;

public:
    FlightManager();

    void loadFlightsFromFile(const string& filename);
    void saveFlightsToFile(const string& filename);
    const vector<Flight>& getAllFlights() const;
    void addFlight(const Flight& flight);
    void removeFlight(const string& flightID);
    Flight* getFlightByID(const string& flightID);
    void displayAllFlights() const;
    void loadReservationsToUpdateSeats(const std::string& filename);
    bool hasScheduleConflict(const std::string& origin, const std::string& destination, const std::string& departureTime);
    bool isScheduledAtTheSameTime(const std::string& origin, const std::string& departureTime);
    bool displayFlightsFromOriginSorted(const std::string& origin);
    bool displayFlightsFromOriginToDestinationSorted(const std::string& origin, const std::string& destination, const std::string &date);




};

#endif
