#include <iostream>
#include <string>
#include "Admin.hpp"
#include "Client.hpp"

void adminMenu(Admin &admin)
{
    int choice;
    do
    {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Flight\n";
        cout << "2. Remove Flight\n";
        cout << "3. Edit Flight\n";
        cout << "4. View Bookings\n";
        cout << "5. View All Flights from an Origin\n";
        cout << "6. View All Flights\n";
        cout << "7. Export Bookings report\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

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
            admin.viewFlightFromAnOrigin();
            break;
        case 6:
            admin.viewAllFlights();
            break;
        case 7:
            admin.exportBookingsReport();
            break;
        case 0:
            admin.logout();
            break;
        default:
            cout << "Invalid choice!\n";
            break;
        }
    } while (choice != 0);
}

void clientMenu(Client &client)
{
    int choice;
    do
    {
        cout << "\n--- Client Menu ---\n";
        cout << "1. Search Flights\n";
        cout << "2. Book Flight\n";
        cout << "3. Cancel Booking\n";
        cout << "4. View Available Flights\n";
        cout << "5. View My Reservations\n";
        cout << "0. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

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
            cout << "Invalid choice!\n";
            break;
        }
    } while (choice != 0);
}

int main()
{
    int roleChoice;
    cout << "Welcome to the Flight Reservation System\n";
    cout << "Select role:\n";
    cout << "1. Admin\n";
    cout << "2. Client\n";
    cout << "Enter your choice: ";
    cin >> roleChoice;

    if (roleChoice == 1) // Admin
    {
        string username, password;
        cout << "Admin Login\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        Admin admin(username, password);
        if (admin.login(username, password))
        {
            cout << "Admin Login Successful!\n";
            adminMenu(admin);
        }
        else
        {
            cout << "Admin login failed.\n";
        }
    }
    else if (roleChoice == 2) // Client
    {
        int clientAction;
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "Enter your choice: ";
        cin >> clientAction;

        if (clientAction == 1) // Client Login
        {
            string username, password;
            cout << "Client Login\n";
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;

            Client client(username, password);
            if (client.login(username, password))
            {
                cout << "Client Login Successful!\n";
                clientMenu(client);
            }
            else
            {
                cout << "Client login failed.\n";
            }
        }
        else if (clientAction == 2)
        {
            if (Client::registerClient())
            {
                cout << "Registration successful. You can now log in.\n";
                string username, password;
                cout << "Client Login\n";
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;

                Client client(username, password);
                if (client.login(username, password))
                {
                    cout << "Client Login Successful!\n";
                    clientMenu(client);
                }
                else
                {
                    cout << "Client login failed.\n";
                }
            }
        }
        else
        {
            cout << "Invalid action.\n";
        }
    }
    else
    {
        cout << "Invalid role selected.\n";
    }

    return 0;
}
