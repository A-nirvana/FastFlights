#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include <string>

class Reservation {
private:
    std::string reservationID;
    std::string clientUsername;
    std::string flightID;
    std::string date;
    int seatNumber;

public:
    Reservation(const std::string& resID, const std::string& client, const std::string& flight, const std::string& bookingDate, int seat);

    std::string getReservationID() const;
    std::string getClientUsername() const;
    std::string getFlightID() const;
    std::string getDate() const;
    int getSeatNumber() const;

    std::string toCSV() const;
};

#endif // RESERVATION_HPP
