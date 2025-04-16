#include "Client.hpp"
#include <iostream>
#include <fstream>

using namespace std;

Client::Client(const std::string& uname, const std::string& pass)
    : User(uname, pass, ""), manager() {
    manager.loadFlightsFromFile("data/flights.csv");
}

bool Client::login(const std::string& uname, const std::string& pass) {
    ifstream file("data/clients.txt");
    string u, p;
    while (file >> u >> p) {
        if (u == uname && p == pass)
            return true;
    }
    return false;
}

void Client::logout() {
    cout << "Client " << username << " logged out.\n";
}

void Client::searchFlights() {
    string from, to;
    cout << "From: ";
    cin >> from;
    cout << "To: ";
    cin >> to;

    cout << "Matching Flights:\n";
    for (const auto& flight : manager.getAllFlights()){
        if (flight.getOrigin() == from && flight.getDestination() == to) {
            flight.displayFlight();
            cout << "----------------------\n";
        }
    }
}

void Client::viewAvailableFlights() {
    manager.displayAllFlights();
}

void Client::bookFlight() {
    string flightID;
    cout << "Enter Flight ID to book: ";
    cin >> flightID;

    Flight* flight = manager.getFlightByID(flightID);
    if (flight && flight->getAvailableSeats() > 0) {
        flight->bookSeat();

        ofstream out("data/reservations.csv", ios::app);
        out << username << "," << flight->getFlightID() << "\n";

        manager.saveFlightsToFile("data/flights.csv");
        cout << "Booking successful!\n";
    } else {
        cout << "Flight not found or no seats available.\n";
    }
}

void Client::cancelBooking() {
    string flightID;
    cout << "Enter Flight ID to cancel: ";
    cin >> flightID;

    Flight* flight = manager.getFlightByID(flightID);
    if (flight) {
        ifstream in("data/reservations.csv");
        ofstream temp("data/temp.csv");
        string line;
        bool cancelled = false;

        while (getline(in, line)) {
            if (line != username + "," + flightID) {
                temp << line << "\n";
            } else {
                cancelled = true;
                flight->cancelSeat();
            }
        }

        in.close();
        temp.close();
        remove("data/reservations.csv");
        rename("data/temp.csv", "data/reservations.csv");
        manager.saveFlightsToFile("data/flights.csv");

        if (cancelled)
            cout << "Booking cancelled.\n";
        else
            cout << "No such booking found.\n";
    } else {
        cout << "Flight not found.\n";
    }
}
