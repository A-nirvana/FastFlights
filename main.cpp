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

void clientMenu(Client &client)
{
    int choice;
    do
    {
        std::cout << "\n--- Client Menu ---\n";
        std::cout << "1. Search Flights\n";
        std::cout << "2. Book Flight\n";
        std::cout << "3. Cancel Booking\n";
        std::cout << "4. View Available Flights\n";
        std::cout << "5. View My Reservations\n";
        std::cout << "0. Logout\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            client.searchFlights();
            break;
        case 2:
            client.bookFlight();
            break;
        case 3:
            client.cancelBooking();
            break;
        case 4:
            client.viewAvailableFlights();
            break;
        case 5:
            client.viewReservations();
            break;
        case 0:
            client.logout();
            break;
        default:
            std::cout << "Invalid choice!\n";
            break;
        }
    } while (choice != 0);
}

int main()
{
    int roleChoice;
    std::cout << "Welcome to the Flight Reservation System\n";
    std::cout << "Select role:\n";
    std::cout << "1. Admin\n";
    std::cout << "2. Client\n";
    std::cout << "Enter your choice: ";
    std::cin >> roleChoice;

    if (roleChoice == 1) // Admin
    {
        std::string username, password;
        std::cout << "Admin Login\n";
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;

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
    else if (roleChoice == 2) // Client
    {
        int clientAction;
        std::cout << "1. Login\n";
        std::cout << "2. Register\n";
        std::cout << "Enter your choice: ";
        std::cin >> clientAction;

        if (clientAction == 1) // Client Login
        {
            std::string username, password;
            std::cout << "Client Login\n";
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;

            Client client(username, password);
            if (client.login(username, password))
            {
                std::cout << "Client Login Successful!\n";
                clientMenu(client);
            }
            else
            {
                std::cout << "Client login failed.\n";
            }
        }
        else if (clientAction == 2)
        {
            if (Client::registerClient())
            {
                std::cout << "Registration successful. You can now log in.\n";
                std::string username, password;
                std::cout << "Client Login\n";
                std::cout << "Username: ";
                std::cin >> username;
                std::cout << "Password: ";
                std::cin >> password;

                Client client(username, password);
                if (client.login(username, password))
                {
                    std::cout << "Client Login Successful!\n";
                    clientMenu(client);
                }
                else
                {
                    std::cout << "Client login failed.\n";
                }
            }
        }
        else
        {
            std::cout << "Invalid action.\n";
        }
    }
    else
    {
        std::cout << "Invalid role selected.\n";
    }

    return 0;
}
