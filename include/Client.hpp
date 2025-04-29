// All client member variables and functions have been declared here

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "User.hpp"
#include "FlightManager.hpp"

using namespace std;

class Client : public User
{
private:
    FlightManager manager;

public:
    Client(const string &uname, const string &pass);

    bool login(const string &uname, const string &pass) override;
    void logout() override;

    static bool registerClient();
    void searchFlights();
    void bookFlight();
    void cancelBooking();
    void viewAvailableFlights();

    void viewReservations();
};

#endif // CLIENT_HPP
