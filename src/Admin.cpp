#include "Admin.hpp"
#include <iostream>
#include <fstream>

using namespace std;

Admin::Admin(const std::string &uname, const std::string &pass, const std::string &mail)
    : User(uname, pass, mail)
{
    manager.loadFlightsFromFile("data/flights.csv");
    manager.loadReservationsToUpdateSeats("data/reservations.csv");
}

bool Admin::login(const std::string &uname, const std::string &pass)
{
    ifstream file("data/admins.txt");
    string u, p;
    while (file >> u >> p)
    {
        if (u == uname && p == pass)
            return true;
    }
    return false;
}

void Admin::logout()
{
    cout << "Logged out successfully.\n";
    manager.saveFlightsToFile("data/flights.csv");
}

void Admin::addFlight()
{
    std::string id, from, to, time, arrival, dur;
    int total, price;

    cout << "Flight ID: ";
    std::cin >> id;
    cout << "From: ";
    std::cin >> from;
    cout << "To: ";
    std::cin >> to;
    cout << "Departure Time: ";
    std::cin.ignore();
    getline(std::cin, time);
    cout << "Arrival Time: ";
    getline(std::cin, arrival);
    cout << "Duration: ";
    getline(std::cin, dur);
    cout << "Total Seats: ";
    std::cin >> total;
    cout << "Price: ";
    std::cin >> price;

    Flight newFlight(id, from, to, time, arrival, dur, total, total, price);
    manager.addFlight(newFlight);
    manager.saveFlightsToFile("data/flights.csv");

    cout << "Flight added successfully.\n";
}

void Admin::removeFlight()
{
    string id;
    cout << "Enter Flight ID to remove: ";
    cin >> id;

    manager.removeFlight(id);
    manager.saveFlightsToFile("data/flights.csv");

    cout << "Flight removed.\n";
}

void Admin::editFlight()
{
    string id;
    cout << "Enter Flight ID to edit: ";
    cin >> id;

    Flight *flight = manager.getFlightByID(id);
    if (flight)
    {
        string time, arrival, dur;
        int seats;
        cout << "New Departure Time: ";
        cin.ignore();
        getline(std::cin, time);
        cout << "Arrival Time: ";
        std::cin.ignore();
        getline(std::cin, arrival);
        cout << "Duration: ";
        std::cin.ignore();
        getline(std::cin, dur);
        cout << "New Available Seats: ";
        cin >> seats;

        *flight = Flight(flight->getFlightID(), flight->getOrigin(), flight->getDestination(), time, arrival, dur, flight->getTotalSeats(), seats, flight->getPrice());
        manager.saveFlightsToFile("data/flights.csv");

        cout << "Flight updated.\n";
    }
    else
    {
        cout << "Flight not found.\n";
    }
}

void Admin::viewBookings()
{
    ifstream file("data/reservations.csv");
    string line;
    cout << "All Reservations:\n";
    while (getline(file, line))
    {
        cout << line << "\n";
    }
    file.close();
}

void Admin::viewAllFlights()
{
    cout << "All Flights:\n\n";
    manager.displayAllFlights();
    cout << "\n";
}