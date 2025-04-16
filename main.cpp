#include <iostream>
#include <string>
#include "Admin.hpp"
#include "Client.hpp"

void adminMenu(Admin &admin)
{
    int choice;
    do
    {
        std::cout << "\n--- Admin Menu ---\n";
        std::cout << "1. Add Flight\n";
        std::cout << "2. Remove Flight\n";
        std::cout << "3. Edit Flight\n";
        std::cout << "4. View Bookings\n";
        std::cout << "5. View All Flights\n";
        std::cout << "0. Logout\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            admin.addFlight();
            break;
        case 2:
            admin.removeFlight();
            break;
        case 3:
            admin.editFlight();
            break;
        case 4:
            admin.viewBookings();
            break;
        case 5:
            admin.viewAllFlights();
            break;
        case 0:
            admin.logout();
            break;
        default:
            std::cout << "Invalid choice!\n";
            break;
        }
    } while (choice != 0);
}

// void clientMenu(Client& client) {
//     int choice;
//     do {
//         std::cout << "\n--- Client Menu ---\n";
//         std::cout << "1. Search Flights\n";
//         std::cout << "2. Book Flight\n";
//         std::cout << "3. Cancel Booking\n";
//         std::cout << "4. View Available Flights\n";
//         std::cout << "0. Logout\n";
//         std::cout << "Enter choice: ";
//         std::cin >> choice;

//         switch (choice) {
//             case 1: client.searchFlights(); break;
//             case 2: client.bookFlight(); break;
//             case 3: client.cancelBooking(); break;
//             case 4: client.viewAvailableFlights(); break;
//             case 0: client.logout(); break;
//             default: std::cout << "Invalid choice!\n"; break;
//         }
//     } while (choice != 0);
// }

int main()
{
    int userType;
    std::cout << "Welcome to the Flight Reservation System\n";
    std::cout << "1. Admin Login\n";
    std::cout << "2. Client Login\n";
    std::cout << "Enter your choice: ";
    std::cin >> userType;

    std::string username, password, email;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (userType == 1)
    {
        Admin admin(username, password);
        if (admin.login(username, password))
        {
            std::cout << "Admin Login Successful!\n";
            adminMenu(admin);
        }
        else
        {
            std::cout << "Admin login failed.\n";
        }
    }
    else if (userType == 2) {
        Client client(username, password);
        if (client.login(username, password)) {
            std::cout << "Client Login Successful!\n";
    
            int choice;
            do {
                std::cout << "\n--- Client Menu ---\n";
                std::cout << "1. Search Flights\n";
                std::cout << "2. Book Flight\n";
                std::cout << "3. Cancel Booking\n";
                std::cout << "4. View Available Flights\n";
                std::cout << "0. Logout\n";
                std::cout << "Enter choice: ";
                std::cin >> choice;
    
                switch (choice) {
                    case 1: client.searchFlights(); break;
                    case 2: client.bookFlight(); break;
                    case 3: client.cancelBooking(); break;
                    case 4: client.viewAvailableFlights(); break;
                    case 0: client.logout(); break;
                    default: std::cout << "Invalid choice!\n"; break;
                }
            } while (choice != 0);
        } else {
            std::cout << "Client login failed.\n";
        }
    }
    
    else
    {
        std::cout << "Invalid user type.\n";
    }

    return 0;
}
