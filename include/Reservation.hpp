// this file defines the Reservation class, which represents a flight reservation.
// it includes attributes such as reservation ID, client username, flight ID, date, and seat number.
// it also includes methods for getting reservation details and converting the reservation to a CSV format.

#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include <string>

using namespace std;

class Reservation {
private:
    string reservationID;
    string clientUsername;
    string flightID;
    string date;
    int seatNumber;

public:
    Reservation(const string& resID, const string& client, const string& flight, const string& bookingDate, int seat);

    string getReservationID() const;
    string getClientUsername() const;
    string getFlightID() const;
    string getDate() const;
    int getSeatNumber() const;

    string toCSV() const;
};

#endif // RESERVATION_HPP
