#ifndef ADMIN_HPP
#define ADMIN_HPP

#include "User.hpp"
#include "FlightManager.hpp"

class Admin : public User {
private:
    FlightManager manager;

public:
    Admin(const std::string& uname = "", const std::string& pass = "", const std::string& mail = "");

    bool login(const std::string& uname, const std::string& pass) override;
    void logout() override;

    void addFlight();
    void removeFlight();
    void editFlight();
    void viewBookings(); 
    void viewAllFlights(); 
};

#endif

