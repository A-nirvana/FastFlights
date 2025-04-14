#include <iostream>
#include <vector>
#include <memory>
#include "User.hpp"
// #include "Client.hpp"
// #include "Admin.hpp"

using namespace std;

void showMainMenu() {
    cout << "\n======= Flight Reservation System =======\n";
    cout << "1. Login as Client\n";
    cout << "2. Login as Admin\n";
    cout << "3. Exit\n";
    cout << "Choose your option: ";
}

int main() {
    // Demo users
    // vector<Client> clients = {
    //     Client("john_doe", "1234", "john@example.com")
    // };

    // vector<Admin> admins = {
    //     Admin("admin", "admin123", "admin@flights.com")
    // };

    // int choice;
    // while (true) {
    //     showMainMenu();
    //     cin >> choice;

    //     if (choice == 3) {
    //         cout << "Exiting system. Thank you!\n";
    //         break;
    //     }

    //     string uname, pass;
    //     cout << "Username: ";
    //     cin >> uname;
    //     cout << "Password: ";
    //     cin >> pass;

    //     bool loggedIn = false;

    //     if (choice == 1) {
    //         for (auto& c : clients) {
    //             if (c.login(uname, pass)) {
    //                 loggedIn = true;
    //                 c.clientMenu(); // show client options
    //                 break;
    //             }
    //         }
    //         if (!loggedIn) cout << "Invalid client credentials.\n";

    //     } else if (choice == 2) {
    //         for (auto& a : admins) {
    //             if (a.login(uname, pass)) {
    //                 loggedIn = true;
    //                 a.adminMenu(); // show admin options
    //                 break;
    //             }
    //         }
    //         if (!loggedIn) cout << "Invalid admin credentials.\n";
    //     } else {
    //         cout << "Invalid option.\n";
    //     }
    // }

    return 0;
}
