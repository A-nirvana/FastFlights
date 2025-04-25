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

int Flight::assignSeat()
{
    if (availableSeats > 0)
    {
        --availableSeats;
        return totalSeats - availableSeats; // Simple seat assignment logic
    }
    return -1; // No seats available
}

void Flight::bookSeat()
{
    if (availableSeats > 0)
        availableSeats--;
}

void Flight::displayFlight() const
{
    std::cout << "\n\nFlight ID: " << flightID
              << "\nFrom: " << origin
              << "\nTo: " << destination
              << "\nDeparture: " << departureTime
              << "\nArrival: " << arrivalTime
              << "\nDuration: " << duration
              << "\nTotal Seats " << totalSeats
              << "\nPrice: " << price << "\n";
}

int Flight::assignSeatOnDate(const std::string &date)
{
    auto &seatMap = seatMapByDate[date];

    if (seatMap.empty())
    {
        seatMap.resize(totalSeats, false); // all seats initially unbooked
    }

    for (int i = 0; i < totalSeats; ++i)
    {
        if (!seatMap[i])
        {
            seatMap[i] = true;
            return i + 1; // seat numbers start at 1
        }
    }

    return -1; // no seats available
}

void Flight::cancelSeatOnDate(const std::string &date, int seatNumber)
{
    auto &seatMap = seatMapByDate[date];
    if (seatNumber >= 1 && seatNumber <= totalSeats && seatMap.size() >= seatNumber)
    {
        seatMap[seatNumber - 1] = false;
    }
}

int Flight::getAvailableSeatsOnDate(const std::string &date) const
{
    auto it = seatMapByDate.find(date);
    if (it == seatMapByDate.end())
        return totalSeats;

    int available = 0;
    for (bool booked : it->second)
    {
        if (!booked)
            available++;
    }
    return available;
}
