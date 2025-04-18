#include "../include/Reservation.hpp"

Reservation::Reservation(const std::string& resID, const std::string& client, const std::string& flight, const std::string& bookingDate, int seat)
    : reservationID(resID), clientUsername(client), flightID(flight), date(bookingDate), seatNumber(seat) {}

std::string Reservation::getReservationID() const {
    return reservationID;
}

std::string Reservation::getClientUsername() const {
    return clientUsername;
}

std::string Reservation::getFlightID() const {
    return flightID;
}

std::string Reservation::getDate() const {
    return date;
}

int Reservation::getSeatNumber() const {
    return seatNumber;
}

std::string Reservation::toCSV() const {
    return reservationID + "," + clientUsername + "," + flightID + "," + date + "," + std::to_string(seatNumber);
}
