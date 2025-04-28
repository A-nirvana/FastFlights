#include "../include/Client.hpp"
#include "HashUtil.hpp"
#include "../include/Reservation.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> // for setw, setfill
#include <chrono>
#include <ctime>
#include <set>
#include <limits>

string getFormattedDateInput()
{
    int year, month, day;

    cout << "Enter Year (e.g., 2025): ";
    cin >> year;

    cout << "Enter Month (1-12): ";
    cin >> month;
    if (month < 1 || month > 12)
    {
        cout << " Invalid month entered.\n";
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
        cout << " Booking not allowed for today or past dates. Earliest allowed: " << minDate << "\n";
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
    cout << "      Please arrive 45 mins early.\n";
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
        cout << "\n No flights found for the given route and date.\n";
        return;
    }
    
    string flightID;
    cout << "\nEnter Flight ID to book: ";
    cin >> flightID;
    
    Flight *selected = manager.getFlightByID(flightID);
    if (!selected)
    {
        cout << " Invalid flight ID.\n";
        return;
    }
    
    int available = selected->getAvailableSeatsOnDate(date);
    if (available == 0)
    {
        cout << " No seats available on this date.\n";
        return;
    }
    
    int seatCount;
    cout << "How many seats would you like to book? (Max 5): ";
    cin >> seatCount;
    
    if (seatCount < 1 || seatCount > 5)
    {
        cout << " Invalid number of seats requested.\n";
        return;
    }
    
    if (seatCount > available)
    {
        cout << " Only " << available << " seat(s) available on this date.\n";
        return;
    }
    
    // 🧮 Calculate and Show Total Price BEFORE Booking
    float totalPrice = seatCount * selected->getPrice();
    
    cout << "\n===========================================\n";
    cout << " 🛒 Booking Summary\n";
    cout << "-------------------------------------------\n";
    cout << " From: " << from << "\n";
    cout << " To: " << to << "\n";
    cout << " Date: " << date << "\n";
    cout << " Flight ID: " << selected->getFlightID() << "\n";
    cout << " Seats: " << seatCount << "\n";
    cout << " Total Price: ₹" << fixed << setprecision(2) << totalPrice << "\n";
    cout << "===========================================\n";
    
    char confirm;
    cout << "Do you want to proceed with the booking? (Y/N): ";
    cin >> confirm;
    
    if (confirm != 'Y' && confirm != 'y')
    {
        cout << "\n❗ Booking canceled. Returning to main menu.\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }
    
    //  Proceed to actually book tickets
    ofstream out("data/reservations.csv", ios::app);
    if (!out)
    {
        cout << " Could not open reservations file.\n";
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
    
    cout << "\n " << seatCount << " ticket(s) booked successfully!\n";
    cout << "Thank you for choosing FastFlights\n";
    cout << "\nPress Enter to continue...";
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
    string date = getFormattedDateInput();
    if (date.empty())
        return;

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
    // 1. Load all reservations and group by date
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
        if (uname == username)
            bookingsByDate[date].push_back(r);
    }
    in.close();

    if (bookingsByDate.empty()) {
        std::cout << "\n❌ You have no bookings to cancel.\n";
        return;
    }

    // 2. Let user pick a date
    std::cout << "\n📅 Your Bookings by Date:\n";
    int idx = 1;
    std::vector<std::string> dateOptions;
    for (auto& p : bookingsByDate) {
        std::cout << idx << ". " << p.first << " (" << p.second.size() << " ticket(s))\n";
        dateOptions.push_back(p.first);
        ++idx;
    }

    int dateChoice;
    std::cout << "\nChoose a date to view bookings for cancellation: ";
    std::cin >> dateChoice;
    if (dateChoice < 1 || dateChoice > (int)dateOptions.size()) {
        std::cout << "\n❌ Invalid date selection.\n";
        return;
    }

    // 3. Build the list of reservations on that date
    std::string selectedDate = dateOptions[dateChoice - 1];
    auto& reservationsOnDate = bookingsByDate[selectedDate];

    // 4. Display them in a numbered table
    std::cout << "\n===============================================\n";
    std::cout << std::left
              << std::setw(5)  << "No."
              << std::setw(15) << "Flight ID"
              << std::setw(15) << "Origin"
              << std::setw(15) << "Destination"
              << std::setw(15) << "Seat No." << "\n";
    std::cout << "-----------------------------------------------\n";

    for (int i = 0; i < (int)reservationsOnDate.size(); ++i) {
        auto& res = reservationsOnDate[i];
        Flight* f = manager.getFlightByID(res.getFlightID());
        if (f) {
            std::cout << std::left
                      << std::setw(5)  << (i+1)
                      << std::setw(15) << f->getFlightID()
                      << std::setw(15) << f->getOrigin()
                      << std::setw(15) << f->getDestination()
                      << std::setw(15) << res.getSeatNumber() << "\n";
        }
    }
    std::cout << "===============================================\n";

    // 5. Read user’s list of ticket-numbers to cancel
    std::cout << "\nEnter the numbers of the tickets you want to cancel (e.g. 1 3 4): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string inputLine;
    std::getline(std::cin, inputLine);
    std::stringstream ss(inputLine);
    std::set<int> toCancel;
    int num;
    while (ss >> num) {
        if (num < 1 || num > (int)reservationsOnDate.size()) {
            std::cout << "\n❌ Invalid ticket number: " << num << ". Operation aborted.\n";
            return;
        }
        toCancel.insert(num-1);  // zero-based index
    }
    if (toCancel.empty()) {
        std::cout << "\n❌ No valid tickets selected.\n";
        return;
    }

    // 6. Perform cancellation by reservationID
    std::ofstream out("data/reservations.csv.tmp");
    int cancelled = 0;

    for (auto& r : allReservations) {
        bool cancelThis = false;
        if (r.getClientUsername() == username && r.getDate() == selectedDate) {
            // check if this reservation’s index (in reservationsOnDate) was selected
            for (int idx : toCancel) {
                if (reservationsOnDate[idx].getReservationID() == r.getReservationID()) {
                    cancelThis = true;
                    break;
                }
            }
        }
        if (cancelThis) {
            Flight* f = manager.getFlightByID(r.getFlightID());
            if (f) f->cancelSeatOnDate(r.getDate(), r.getSeatNumber());
            ++cancelled;
        } else {
            out << r.toCSV() << "\n";
        }
    }
    out.close();

    // 7. Replace file and persist flights
    std::remove("data/reservations.csv");
    std::rename("data/reservations.csv.tmp", "data/reservations.csv");
    manager.saveFlightsToFile("data/flights.csv");

    std::cout << "\n✅ " << cancelled << " booking(s) cancelled successfully!\n";
    std::cout << "Press Enter to continue...";
    std::cin.get();
}




