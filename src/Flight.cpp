#include "Flight.hpp"
#include <iostream>

using namespace std;

Flight::Flight(const std::string &id, const std::string &from, const std::string &to, const std::string &time, const string &arrival, const string &dur, int total, int available, int p)
    : flightID(id), origin(from), destination(to), departureTime(time), arrivalTime(arrival), duration(dur), totalSeats(total), availableSeats(available), price(p) {}

string Flight::getFlightID() const { return flightID; }
string Flight::getOrigin() const { return origin; }
string Flight::getDestination() const { return destination; }
string Flight::getDeparatureTime() const { return departureTime; }
string Flight::getArrivalTime() const { return arrivalTime; }
string Flight::getDuration() const { return duration; }
int Flight::getTotalSeats() const { return totalSeats; }
int Flight::getAvailableSeats() const { return availableSeats; }
int Flight::getPrice() const { return price; }

void Flight::bookSeat()
{
    if (availableSeats > 0)
        availableSeats--;
}

void Flight::cancelSeat()
{
    if (availableSeats < totalSeats)
        availableSeats++;
}

void Flight::displayFlight() const
{
    std::cout << "Flight ID: " << flightID
              << "\nFrom: " << origin
              << "\nTo: " << destination
              << "\nDeparture: " << departureTime
              << "\nArrival: " << arrivalTime
              << "\nDuration: " << duration
              << "\nSeats Available: " << availableSeats << "/" << totalSeats
              << "\nPrice: " << price << "\n";
}
