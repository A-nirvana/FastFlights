// All admin member variables and functions have been declared here

#ifndef ADMIN_HPP
#define ADMIN_HPP

#include "User.hpp"
#include "FlightManager.hpp"

using namespace std;

class Admin : public User {
private:
    FlightManager manager;

public:
    Admin(const string& uname = "", const string& pass = "", const string& mail = "");

    bool login(const string& uname, const string& pass) override;
    void logout() override;

    void addFlight();
    void removeFlight();
    void editFlight();
    void viewBookings(); 
    void viewAllFlights(); 
    void viewFlightFromAnOrigin();
    void exportBookingsReport();
};

#endif

