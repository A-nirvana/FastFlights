#include "../include/Client.hpp"
#include "HashUtil.hpp"
#include "../include/Reservation.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> // for std::setw, std::setfill
#include <chrono>
#include <ctime>

using namespace std;

std::string getFormattedDateInput()
{
    int year, month, day;

    std::cout << "Enter Year (e.g., 2025): ";
    std::cin >> year;

    std::cout << "Enter Month (1-12): ";
    std::cin >> month;
    if (month < 1 || month > 12)
    {
        std::cout << "❌ Invalid month entered.\n";
        return "";
    }

    std::cout << "Enter Day (1-31): ";
    std::cin >> day;

    // Construct the date string in YYYY-MM-DD format
    std::ostringstream dateStream;
    dateStream << year << "-"
               << std::setw(2) << std::setfill('0') << month << "-"
               << std::setw(2) << std::setfill('0') << day;
    std::string inputDate = dateStream.str();

    // Get current time + 1 day
    auto now = std::chrono::system_clock::now();
    auto tomorrow = now + std::chrono::hours(24);
    std::time_t t = std::chrono::system_clock::to_time_t(tomorrow);
    std::tm *tmTomorrow = std::localtime(&t);

    std::ostringstream tomorrowStream;
    tomorrowStream << (tmTomorrow->tm_year + 1900) << "-"
                   << std::setw(2) << std::setfill('0') << (tmTomorrow->tm_mon + 1) << "-"
                   << std::setw(2) << std::setfill('0') << tmTomorrow->tm_mday;

    std::string minDate = tomorrowStream.str();

    if (inputDate < minDate)
    {
        std::cout << "❌ Booking not allowed for today or past dates. Earliest allowed: " << minDate << "\n";
        return "";
    }

    return inputDate;
}

void printTicket(const Reservation& res, const Flight& flight, bool pause = true) {
    std::cout << "===========================================\n";
    std::cout << "         ✈️  FastFlights Boarding Pass ✈️\n";
    std::cout << "===========================================\n";
    std::cout << " Reservation ID : " << res.getReservationID() << "\n";
    std::cout << " Passenger Name : " << res.getClientUsername() << "\n";
    std::cout << " Flight ID      : " << res.getFlightID() << "\n";
    std::cout << " Route          : " << flight.getOrigin() << " ➡ " << flight.getDestination() << "\n";
    std::cout << " Date           : " << res.getDate() << "\n";
    std::cout << " Departure Time : " << flight.getDeparatureTime() << "\n";
    std::cout << " Seat Number    : " << res.getSeatNumber() << "\n";
    std::cout << "===========================================\n";
    std::cout << "     ✅ Please arrive 45 mins early.\n";
    std::cout << "===========================================\n";

    if (pause) {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
}

bool Client::registerClient()
{
    std::string uname, pass;
    std::cout << "Enter a username: ";
    std::cin >> uname;
    std::cout << "Enter a password: ";
    std::cin >> pass;

    // Check if user already exists
    std::ifstream infile("data/clients.txt");
    std::string line, existingUname, existingPass;
    bool userExists = false;

    while (getline(infile, line))
    {
        std::istringstream iss(line);
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
        std::cout << "Username already exists.\n";
        return false;
    }

    // 🔐 Hash password with username as salt
    std::string hashedPass = simpleHash(uname + pass);

    // Append new user to file
    std::ofstream outfile("data/clients.txt", std::ios::app);
    if (outfile.is_open())
    {
        outfile << uname << " " << hashedPass << "\n";
        outfile.close();
        std::cout << "Client registered successfully.\n";
        return true;
    }
    else
    {
        std::cerr << "Error opening clients.txt for writing.\n";
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

    for (auto &flight : manager.getAllFlights())
    {
        if (flight.getOrigin() == from && flight.getDestination() == to)
        {
            flight.displayFlight();
            cout << "Seats on " << date << ": " << flight.getAvailableSeatsOnDate(date) << endl;
            matches.push_back(&flight);
        }
    }

    if (matches.empty())
    {
        cout << "No flights found.\n";
        return;
    }

    string flightID;
    cout << "Enter Flight ID to book: ";
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
    std::ifstream inFile("data/reservations.csv");
    if (!inFile.is_open())
    {
        std::cout << "No reservations found.\n";
        return;
    }

    std::string line;
    bool found = false;
    while (std::getline(inFile, line))
    {
        std::istringstream ss(line);
        std::string resID, client, flightID, date, seatStr;
        std::getline(ss, resID, ',');
        std::getline(ss, client, ',');
        std::getline(ss, flightID, ',');
        std::getline(ss, date, ',');
        std::getline(ss, seatStr, ',');

        if (client == username)
        {
            std::cout << "Reservation ID: " << resID
                      << ", Flight ID: " << flightID
                      << ", Date: " << date
                      << ", Seat Number: " << seatStr << "\n";
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "No reservations found for user " << username << ".\n";
    }
}

Client::Client(const std::string &uname, const std::string &pass)
    : User(uname, pass, ""), manager()
{
    manager.loadFlightsFromFile("data/flights.csv");
    manager.loadReservationsToUpdateSeats("data/reservations.csv");
}



bool Client::login(const std::string &uname, const std::string &pass)
{
    std::ifstream file("data/clients.txt");
    std::string u, p;
    std::string hashedInput = simpleHash(uname + pass); 

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

    cout << "Matching Flights:\n";
    for (const auto &flight : manager.getAllFlights())
    {
        if (flight.getOrigin() == from && flight.getDestination() == to)
        {
            flight.displayFlight();
            cout << "----------------------\n";
        }
    }
}

void Client::viewAvailableFlights()
{
    manager.displayAllFlights();
}

void Client::cancelBooking() {
    std::ifstream in("data/reservations.csv");
    std::vector<Reservation> allReservations;
    std::map<std::string, std::vector<Reservation>> bookingsByDate;

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string resID, uname, flightID, date, seatStr;

        std::getline(ss, resID, ',');
        std::getline(ss, uname, ',');
        std::getline(ss, flightID, ',');
        std::getline(ss, date, ',');
        std::getline(ss, seatStr, ',');

        Reservation r(resID, uname, flightID, date, std::stoi(seatStr));
        allReservations.push_back(r);

        if (uname == username) {
            bookingsByDate[date].push_back(r);
        }
    }
    in.close();

    if (bookingsByDate.empty()) {
        std::cout << "You have no bookings to cancel.\n";
        return;
    }

    // Show date options
    std::cout << "\nYour bookings by date:\n";
    int idx = 1;
    std::vector<std::string> dateOptions;
    for (const auto& pair : bookingsByDate) {
        std::cout << idx++ << ". " << pair.first << " (" << pair.second.size() << " ticket(s))\n";
        dateOptions.push_back(pair.first);
    }

    int dateChoice;
    std::cout << "Choose a date to cancel tickets from: ";
    std::cin >> dateChoice;

    if (dateChoice < 1 || dateChoice > dateOptions.size()) {
        std::cout << "Invalid choice.\n";
        return;
    }

    std::string selectedDate = dateOptions[dateChoice - 1];
    int maxCancellable = bookingsByDate[selectedDate].size();

    std::cout << "You have " << maxCancellable << " ticket(s) on " << selectedDate << ".\n";
    int cancelCount;
    std::cout << "How many would you like to cancel? ";
    std::cin >> cancelCount;

    if (cancelCount <= 0 || cancelCount > maxCancellable) {
        std::cout << "Invalid number.\n";
        return;
    }

    // Begin cancellation
    std::ofstream out("data/reservations.csv.tmp");
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
    std::remove("data/reservations.csv");
    std::rename("data/reservations.csv.tmp", "data/reservations.csv");

    manager.saveFlightsToFile("data/flights.csv");

    std::cout << "✅ " << cancelled << " booking(s) cancelled successfully.\n";
}

