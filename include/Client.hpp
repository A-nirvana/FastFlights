#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "User.hpp"
#include "FlightManager.hpp"

class Client : public User {
private:
    FlightManager manager;

public:
    Client(const std::string& uname, const std::string& pass);

    bool login(const std::string& uname, const std::string& pass) override;
    void logout() override;

    void searchFlights();
    void bookFlight();
    void cancelBooking();
    void viewAvailableFlights();
};

#endif // CLIENT_HPP
