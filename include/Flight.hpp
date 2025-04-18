#ifndef FLIGHT_HPP
#define FLIGHT_HPP

#include <string>

using namespace std;

class Flight{
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
		
	public:
		Flight(const string& id="", const string& from="", const string& to="", const string& time="", const string& arrival="", const string& dur="", int total=0, int available=0, int p=0);
		
		string getFlightID() const;
		string getOrigin() const;
		string getDestination() const;
		string getDeparatureTime() const;
		string getArrivalTime() const;
		string getDuration() const;
		int getTotalSeats() const;
		int getAvailableSeats() const;
		int getPrice() const;
		
		void bookSeat();
    	void cancelSeat();
		void displayFlight() const;
};  

#endif
