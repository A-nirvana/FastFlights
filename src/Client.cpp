#include "../include/Client.hpp"
#include "HashUtil.hpp"
#include "../include/Reservation.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> // for setw, setfill
#include <chrono>
#include <ctime>

string getFormattedDateInput()
{
    int year, month, day;

    cout << "Enter Year (e.g., 2025): ";
    cin >> year;

    cout << "Enter Month (1-12): ";
    cin >> month;
    if (month < 1 || month > 12)
    {
        cout << "❌ Invalid month entered.\n";
        return "";
    }

    cout << "Enter Day (1-31): ";
    cin >> day;

    // Construct the date string in YYYY-MM-DD format
    ostringstream dateStream;
    dateStream << year << "-"
               << setw(2) << setfill('0') << month << "-"
               << setw(2) << setfill('0') << day;
    string inputDate = dateStream.str();

    // Get current time + 1 day
    auto now = chrono::system_clock::now();
    auto tomorrow = now + chrono::hours(24);
    time_t t = chrono::system_clock::to_time_t(tomorrow);
    tm *tmTomorrow = localtime(&t);

    ostringstream tomorrowStream;
    tomorrowStream << (tmTomorrow->tm_year + 1900) << "-"
                   << setw(2) << setfill('0') << (tmTomorrow->tm_mon + 1) << "-"
                   << setw(2) << setfill('0') << tmTomorrow->tm_mday;

    string minDate = tomorrowStream.str();

    if (inputDate < minDate)
    {
        cout << "❌ Booking not allowed for today or past dates. Earliest allowed: " << minDate << "\n";
        return "";
    }

    return inputDate;
}

void printTicket(const Reservation& res, const Flight& flight, bool pause = true) {
    cout << "===========================================\n";
    cout << "         ✈️  FastFlights Boarding Pass ✈️\n";
    cout << "===========================================\n";
    cout << " Reservation ID : " << res.getReservationID() << "\n";
    cout << " Passenger Name : " << res.getClientUsername() << "\n";
    cout << " Flight ID      : " << res.getFlightID() << "\n";
    cout << " Route          : " << flight.getOrigin() << " ➡ " << flight.getDestination() << "\n";
    cout << " Date           : " << res.getDate() << "\n";
    cout << " Departure Time : " << flight.getDeparatureTime() << "\n";
    cout << " Seat Number    : " << res.getSeatNumber() << "\n";
    cout << "===========================================\n";
    cout << "     ✅ Please arrive 45 mins early.\n";
    cout << "===========================================\n";

    if (pause) {
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
}

bool Client::registerClient()
{
    string uname, pass;
    cout << "Enter a username: ";
    cin >> uname;
    cout << "Enter a password: ";
    cin >> pass;

    // Check if user already exists
    ifstream infile("data/clients.txt");
    string line, existingUname, existingPass;
    bool userExists = false;

    while (getline(infile, line))
    {
        istringstream iss(line);
        iss >> existingUname >> existingPass;

        if (existingUname == uname)
        {
            userExists = true;
            break;
        }
    }
    infile.close();

    if (userExists)
    {
        cout << "Username already exists.\n";
        return false;
    }

    // 🔐 Hash password with username as salt
    string hashedPass = simpleHash(uname + pass);

    // Append new user to file
    ofstream outfile("data/clients.txt", ios::app);
    if (outfile.is_open())
    {
        outfile << uname << " " << hashedPass << "\n";
        outfile.close();
        cout << "Client registered successfully.\n";
        return true;
    }
    else
    {
        cerr << "Error opening clients.txt for writing.\n";
        return false;
    }
}


void Client::bookFlight()
{
    string from, to;
    cout << "From: ";
    cin >> from;
    cout << "To: ";
    cin >> to;

    string date = getFormattedDateInput();
    if (date.empty())
        return;

    vector<const Flight *> matches;
    bool found = manager.displayFlightsFromOriginToDestinationSorted(from, to, date);

   

    if (!found)
    {
        cout << "No flights found.\n";
        return;
    }

    string flightID;
    cout << "\nEnter Flight ID to book: ";
    cin >> flightID;

    Flight *selected = manager.getFlightByID(flightID);
    if (!selected)
    {
        cout << "Invalid flight ID.\n";
        return;
    }

    int available = selected->getAvailableSeatsOnDate(date);
    if (available == 0)
    {
        cout << "No seats available on this date.\n";
        return;
    }

    int seatCount;
    cout << "How many seats would you like to book? (Max 5): ";
    cin >> seatCount;

    if (seatCount < 1 || seatCount > 5)
    {
        cout << "❌ Invalid number of seats.\n";
        return;
    }

    if (seatCount > available)
    {
        cout << "❌ Only " << available << " seat(s) available on this date.\n";
        return;
    }

    ofstream out("data/reservations.csv", ios::app);
    if (!out)
    {
        cout << "Could not open reservations file.\n";
        return;
    }

    for (int i = 0; i < seatCount; ++i)
    {
        int seatNumber = selected->assignSeatOnDate(date);
        string reservationID = "R" + to_string(time(nullptr)) + "_" + to_string(i);
        Reservation res(reservationID, username, flightID, date, seatNumber);
        out << res.toCSV() << "\n";
        printTicket(res, *selected, false); // no pause between tickets
    }

    out.close();
    manager.saveFlightsToFile("data/flights.csv");

    cout << "\n✅ " << seatCount << " ticket(s) booked successfully.\n";
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}


void Client::viewReservations()
{
    ifstream inFile("data/reservations.csv");
    if (!inFile.is_open())
    {
        cout << "No reservations found.\n";
        return;
    }

    string line;
    bool found = false;
    while (getline(inFile, line))
    {
        istringstream ss(line);
        string resID, client, flightID, date, seatStr;
        getline(ss, resID, ',');
        getline(ss, client, ',');
        getline(ss, flightID, ',');
        getline(ss, date, ',');
        getline(ss, seatStr, ',');

        if (client == username)
        {
            cout << "Reservation ID: " << resID
                      << ", Flight ID: " << flightID
                      << ", Date: " << date
                      << ", Seat Number: " << seatStr << "\n";
            found = true;
        }
    }

    if (!found)
    {
        cout << "No reservations found for user " << username << ".\n";
    }
}

Client::Client(const string &uname, const string &pass)
    : User(uname, pass, ""), manager()
{
    manager.loadFlightsFromFile("data/flights.csv");
    manager.loadReservationsToUpdateSeats("data/reservations.csv");
}



bool Client::login(const string &uname, const string &pass)
{
    ifstream file("data/clients.txt");
    string u, p;
    string hashedInput = simpleHash(uname + pass); 

    while (file >> u >> p)
    {
        if (u == uname && p == hashedInput)
            return true;
    }
    return false;
}

void Client::logout()
{
    cout << "Client " << username << " logged out.\n";
}

void Client::searchFlights()
{
    string from, to;
    cout << "From: ";
    cin >> from;
    cout << "To: ";
    cin >> to;
    cout<<"Date (YYYY-MM-DD): ";
    string date;
    cin >> date;

    cout << "Matching Flights:\n";
    bool found = manager.displayFlightsFromOriginToDestinationSorted(from, to, date);

    
    if(!found)
    {
        cout << "No flights found.\n";
        cout<< "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
    
    
}

void Client::viewAvailableFlights()
{
    manager.displayAllFlights();
}

void Client::cancelBooking() {
    ifstream in("data/reservations.csv");
    vector<Reservation> allReservations;
    map<string, vector<Reservation>> bookingsByDate;

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string resID, uname, flightID, date, seatStr;

        getline(ss, resID, ',');
        getline(ss, uname, ',');
        getline(ss, flightID, ',');
        getline(ss, date, ',');
        getline(ss, seatStr, ',');

        Reservation r(resID, uname, flightID, date, stoi(seatStr));
        allReservations.push_back(r);

        if (uname == username) {
            bookingsByDate[date].push_back(r);
        }
    }
    in.close();

    if (bookingsByDate.empty()) {
        cout << "You have no bookings to cancel.\n";
        return;
    }

    // Show date options
    cout << "\nYour bookings by date:\n";
    int idx = 1;
    vector<string> dateOptions;
    for (const auto& pair : bookingsByDate) {
        cout << idx++ << ". " << pair.first << " (" << pair.second.size() << " ticket(s))\n";
        dateOptions.push_back(pair.first);
    }

    int dateChoice;
    cout << "Choose a date to cancel tickets from: ";
    cin >> dateChoice;

    if (dateChoice < 1 || dateChoice > dateOptions.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    string selectedDate = dateOptions[dateChoice - 1];
    int maxCancellable = bookingsByDate[selectedDate].size();

    cout << "You have " << maxCancellable << " ticket(s) on " << selectedDate << ".\n";
    int cancelCount;
    cout << "How many would you like to cancel? ";
    cin >> cancelCount;

    if (cancelCount <= 0 || cancelCount > maxCancellable) {
        cout << "Invalid number.\n";
        return;
    }

    // Begin cancellation
    ofstream out("data/reservations.csv.tmp");
    int cancelled = 0;

    for (const auto& r : allReservations) {
        if (r.getClientUsername() == username && r.getDate() == selectedDate && cancelled < cancelCount) {
            // Cancel this one
            Flight* f = manager.getFlightByID(r.getFlightID());
            if (f) {
                f->cancelSeatOnDate(r.getDate(), r.getSeatNumber());
            }
            cancelled++;
            continue;
        }
        out << r.toCSV() << "\n";
    }

    out.close();
    remove("data/reservations.csv");
    rename("data/reservations.csv.tmp", "data/reservations.csv");

    manager.saveFlightsToFile("data/flights.csv");

    cout << "✅ " << cancelled << " booking(s) cancelled successfully.\n";
}

