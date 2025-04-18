#ifndef FLIGHT_HPP
#define FLIGHT_HPP

#include <string>
#include <map>
#include <vector>

using namespace std;

class Flight
{
private:
	string flightID;
	string origin;
	string destination;
	string departureTime;
	string arrivalTime;
	string duration;
	int totalSeats;
	int availableSeats;
	int price;
	map<std::string, std::vector<bool>> seatMapByDate;

public:
	Flight(const string &id = "", const string &from = "", const string &to = "", const string &time = "", const string &arrival = "", const string &dur = "", int total = 0, int available = 0, int p = 0);

	string getFlightID() const;
	string getOrigin() const;
	string getDestination() const;
	string getDeparatureTime() const;
	string getArrivalTime() const;
	string getDuration() const;
	int getTotalSeats() const;
	int getPrice() const;
	int assignSeat();
	int getAvailableSeats() const;

	void bookSeat();
	void cancelSeat();
	void displayFlight() const;

	int assignSeatOnDate(const std::string& date);
    void cancelSeatOnDate(const std::string& date, int seatNumber);
    int getAvailableSeatsOnDate(const std::string& date) const;
};

#endif
