#include "Admin.hpp"
#include "HashUtil.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

Admin::Admin(const string &uname, const string &pass, const string &mail)
    : User(uname, pass, mail)
{
    manager.loadFlightsFromFile("data/flights.csv");
    manager.loadReservationsToUpdateSeats("data/reservations.csv");
}

bool Admin::login(const string &uname, const string &pass)
{
    ifstream file("data/admins.txt");
    string u, p;
    string hashedInput = simpleHash(uname + pass);

    while (file >> u >> p)
    {
        if (u == uname && p == hashedInput)
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
    string id, from, to, time, arrival, dur;
    int total, price;
    int flag = 0;
    do
    {
        cout << "Flight ID: ";
        cin >> id;
        Flight *flight = manager.getFlightByID(id);
        if (flight)
        {
            cout << "Flight ID already exists. Please enter a unique ID.\n";
            flag = 1;
            continue;
        }
        cout << "From: ";
        cin >> from;
        cout << "To: ";
        cin >> to;
        cout << "Departure Time: ";
        cin.ignore();
        getline(cin, time);
        while (manager.isScheduledAtTheSameTime(from, time))
        {
            cout << "Another flight is already scheduled from " << from << " at this time. Please enter a different time: ";
            getline(cin, time);
        }
        cout << "Arrival Time: ";
        getline(cin, arrival);
        cout << "Duration: ";
        getline(cin, dur);
        cout << "Total Seats: ";
        cin >> total;
        cout << "Price: ";
        cin >> price;
        flag = 0;
        if (manager.hasScheduleConflict(from, to, time))
        {
            cout << "A flight with the same route and time already exists.\n";
            cout << "Enter 1 to try again: ";
            cin >> flag;
            if (flag != 1)
            {
                cout << "Flight not added.\n";
                return;
            }
        }

    } while (flag);

    Flight newFlight(id, from, to, time, arrival, dur, total, total, price);
    manager.addFlight(newFlight);
    manager.saveFlightsToFile("data/flights.csv");

    cout << "Flight added successfully.\n";
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void Admin::removeFlight()
{
    string id;
    cout << "Enter Flight ID to remove: ";
    cin >> id;
    Flight *flight = manager.getFlightByID(id);
    if (!flight)
    {
        cout << "Flight not found.\n";
        return;
    }
    cout << "Flight Details:\n";
    flight->displayFlight();
    cout << "Enter 1 to confirm: ";
    int confirm;
    cin >> confirm;
    if (confirm != 1)
    {
        cout << "Flight not removed.\n";
        return;
    }
    manager.removeFlight(id);
    manager.saveFlightsToFile("data/flights.csv");

    cout << "Flight removed.\n";
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void Admin::editFlight()
{
    string id;
    cout << "Enter Flight ID to edit: ";
    cin >> id;

    Flight *flight = manager.getFlightByID(id);
    if (flight)
    {
        cout << "Current Flight Details:\n";
        flight->displayFlight();
        cout << "\nEnter new details:\n";
        string time, arrival, dur;
        int price;
        cout << "New Departure Time: ";
        cin.ignore();
        getline(cin, time);
        cout << "Arrival Time: ";
        getline(cin, arrival);
        cout << "Duration: ";
        cin.ignore();
        getline(cin, dur);
        cout << "New Price: ";
        cin >> price;

        *flight = Flight(flight->getFlightID(), flight->getOrigin(), flight->getDestination(), time, arrival, dur, flight->getTotalSeats(), flight->getTotalSeats(), price);
        manager.saveFlightsToFile("data/flights.csv");

        cout << "Flight updated.\n";
    }
    else
    {
        cout << "Flight not found.\n";
    }
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void Admin::viewBookings()
{
    ifstream resFile("data/reservations.csv");
    if (!resFile.is_open())
    {
        cout << "Error opening reservations.csv\n";
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    // Load flight data
    ifstream flightFile("data/flights.csv");
    vector<Flight> flightList;
    string fline;

    while (getline(flightFile, fline))
    {
        stringstream ss(fline);
        string id, from, to, time, arrival, dur;
        int total, available, price;

        getline(ss, id, ',');
        getline(ss, from, ',');
        getline(ss, to, ',');
        getline(ss, time, ',');
        getline(ss, arrival, ',');
        getline(ss, dur, ',');
        ss >> total;
        ss.ignore();
        ss >> available;
        ss >> price;

        flightList.emplace_back(id, from, to, time, arrival, dur, total, available, price);
    }

    flightFile.close();
    cout << endl;
    cout << "All Bookings:\n\n";
    // Header
    cout << left
         << setw(18) << "Reservation ID"
         << setw(15) << "User"
         << setw(12) << "Flight ID"
         << setw(20) << "Departure Time"
         << setw(20) << "Route"
         << setw(10) << "Seat"
         << "\n";

    cout << string(90, '-') << "\n";

    // Read each booking
    string rline;
    while (getline(resFile, rline))
    {
        stringstream ss(rline);
        string resID, user, flightID, date;
        int seat;

        getline(ss, resID, ',');
        getline(ss, user, ',');
        getline(ss, flightID, ',');
        getline(ss, date, ',');
        ss >> seat;

        // Find the flight
        string origin = "N/A", dest = "N/A", departure = "N/A";
        for (const auto &f : flightList)
        {
            if (f.getFlightID() == flightID)
            {
                origin = f.getOrigin();
                dest = f.getDestination();
                departure = f.getDeparatureTime();
                break;
            }
        }

        // Display row
        cout << left
             << setw(18) << resID
             << setw(15) << user
             << setw(12) << flightID
             << setw(20) << departure
             << setw(20) << (origin + " -> " + dest)
             << setw(10) << seat
             << "\n";
    }

    resFile.close();
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void Admin::viewAllFlights()
{
    cout << "All Flights:\n\n";
    manager.displayAllFlights();
    cout << "\n";
}

void Admin::viewFlightFromAnOrigin()
{
    string origin;
    cout << "Enter origin: ";
    cin >> origin;
    cout << "\n";
    bool found = manager.displayFlightsFromOriginSorted(origin);
    cout << "\n";
    if (found)
    {
        cout << "Do you want to edit any flight? (1 for Yes, 0 for No): ";
        int choice;
        cin >> choice;
        if (choice == 1)
        {
            editFlight();
        }
        else
        {
            cout << "No flight edited.\n";
        }
    }

    cout << "\n";

    return;
}