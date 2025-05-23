#include "FlightManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

FlightManager::FlightManager() {}

void FlightManager::loadFlightsFromFile(const string &filename)
{
    ifstream file(filename);
    string line;

    flights.clear();
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, from, to, time, arrival, dur;
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

        flights.emplace_back(id, from, to, time, arrival, dur, total, available, prc);
    }

    file.close();
}

void FlightManager::saveFlightsToFile(const string &filename)
{
    ofstream file(filename);

    for (const auto &flight : flights)
    {
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

void FlightManager::addFlight(const Flight &flight)
{
    flights.push_back(flight);
}

void FlightManager::removeFlight(const string &flightID)
{
    for (auto it = flights.begin(); it != flights.end(); ++it)
    {
        if (it->getFlightID() == flightID)
        {
            flights.erase(it);
            break;
        }
    }
}

Flight *FlightManager::getFlightByID(const string &flightID)
{
    for (auto &flight : flights)
    {
        if (flight.getFlightID() == flightID)
        {
            return &flight;
        }
    }
    return nullptr;
}

void FlightManager::displayAllFlights() const
{
    if (flights.empty())
    {
        cout << "No flights available.\n";
        return;
    }

    // Print table header
    cout << left
         << setw(12) << "Flight ID"
         << setw(15) << "Origin"
         << setw(15) << "Destination"
         << setw(20) << "Departure Time"
         << setw(20) << "Arrival Time"
         << setw(15) << "Total Seats"
         << "\n";

    cout << string(97, '-') << "\n";

    // Print each flight row
    for (const auto &flight : flights)
    {
        cout << left
             << setw(12) << flight.getFlightID()
             << setw(15) << flight.getOrigin()
             << setw(15) << flight.getDestination()
             << setw(20) << flight.getDeparatureTime()
             << setw(20) << flight.getArrivalTime()
             << setw(15) << flight.getTotalSeats()
             << "\n";
    }
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}
const vector<Flight> &FlightManager::getAllFlights() const
{
    return flights;
}

void FlightManager::loadReservationsToUpdateSeats(const string &filename)
{
    ifstream file(filename);
    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string resID, username, flightID, date, seatStr;

        getline(ss, resID, ',');
        getline(ss, username, ',');
        getline(ss, flightID, ',');
        getline(ss, date, ',');
        getline(ss, seatStr, ',');

        Flight *flight = getFlightByID(flightID);
        if (flight)
        {
            flight->assignSeatOnDate(date); // Updates seat tracking for that date
        }
    }

    file.close();
}

bool FlightManager::hasScheduleConflict(const string &origin, const string &destination, const string &departureTime)
{
    for (const auto &flight : flights)
    {
        if (flight.getOrigin() == origin &&
            flight.getDestination() == destination &&
            flight.getDeparatureTime() == departureTime)
        {
            return true;
        }
    }
    return false;
}

bool FlightManager::isScheduledAtTheSameTime(const string &origin, const string &departureTime)
{
    for (const auto &flight : flights)
    {
        if (flight.getOrigin() == origin &&
            flight.getDeparatureTime() == departureTime)
        {
            return true;
        }
    }
    return false;
}

bool FlightManager::displayFlightsFromOriginSorted(const string &origin)
{
    vector<Flight> filtered;

    // Step 1: Filter flights by origin
    for (const auto &flight : flights)
    {
        if (flight.getOrigin() == origin)
        {
            filtered.push_back(flight);
        }
    }

    if (filtered.empty())
    {
        cout << "No flights found from " << origin << ".\n";
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        return false;
    }

    // Step 2: Sort filtered flights by departure time
    sort(filtered.begin(), filtered.end(), [](const Flight &a, const Flight &b)
         { return a.getDeparatureTime() < b.getDeparatureTime(); });

    // Step 3: Display in table format
    cout << left
         << setw(12) << "Flight ID"
         << setw(15) << "Origin"
         << setw(15) << "Destination"
         << setw(20) << "Departure Time"
         << setw(20) << "Arrival Time"
         << setw(10) << "Seats"
         << "\n";

    cout << string(90, '-') << "\n";

    for (const auto &flight : filtered)
    {
        cout << left
             << setw(12) << flight.getFlightID()
             << setw(15) << flight.getOrigin()
             << setw(15) << flight.getDestination()
             << setw(20) << flight.getDeparatureTime()
             << setw(20) << flight.getArrivalTime()
             << setw(10) << flight.getAvailableSeats()
             << "\n";
    }
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
    return true;
}

bool FlightManager::displayFlightsFromOriginToDestinationSorted(const string &origin, const string &destination, const std::string &date = "")
{
    vector<Flight> filtered;

    // Step 1: Filter flights by origin
    for (const auto &flight : flights)
    {
        if (flight.getOrigin() == origin && flight.getDestination() == destination)
        {
            filtered.push_back(flight);
        }
    }

    if (filtered.empty())
    {
        cout << "No flights found from " << origin << " to " << destination << ".\n";
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        return false;
    }

    sort(filtered.begin(), filtered.end(), [](const Flight &a, const Flight &b)
         { return a.getDeparatureTime() < b.getDeparatureTime(); });

    cout << left
         << setw(12) << "Flight ID"
         << setw(15) << "Origin"
         << setw(15) << "Destination"
         << setw(20) << "Departure Time"
         << setw(20) << "Arrival Time"
         << setw(20) << "Available Seats "
         << setw(10) << "Price "
         << "\n";

    cout << string(110, '-') << "\n";

    for (const auto &flight : filtered)
    {
        cout << left
             << setw(12) << flight.getFlightID()
             << setw(15) << flight.getOrigin()
             << setw(15) << flight.getDestination()
             << setw(20) << flight.getDeparatureTime()
             << setw(20) << flight.getArrivalTime()
             << setw(20) << flight.getAvailableSeatsOnDate(date)
             << setw(10) << flight.getPrice()
             << "\n";
    }
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
    return true;
}
