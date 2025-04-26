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
    void loadReservationsToUpdateSeats(const string& filename);
    bool hasScheduleConflict(const string& origin, const string& destination, const string& departureTime);
    bool isScheduledAtTheSameTime(const string& origin, const string& departureTime);
    bool displayFlightsFromOriginSorted(const string& origin);



};

#endif
