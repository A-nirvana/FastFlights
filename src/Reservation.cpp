#include "../include/Reservation.hpp"

Reservation::Reservation(const string& resID, const string& client, const string& flight, const string& bookingDate, int seat)
    : reservationID(resID), clientUsername(client), flightID(flight), date(bookingDate), seatNumber(seat) {}

string Reservation::getReservationID() const {
    return reservationID;
}

string Reservation::getClientUsername() const {
    return clientUsername;
}

string Reservation::getFlightID() const {
    return flightID;
}

string Reservation::getDate() const {
    return date;
}

int Reservation::getSeatNumber() const {
    return seatNumber;
}

string Reservation::toCSV() const {
    return reservationID + "," + clientUsername + "," + flightID + "," + date + "," + to_string(seatNumber);
}
