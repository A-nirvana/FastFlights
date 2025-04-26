#include "User.hpp"
#include "HashUtil.hpp"

// Constructor: Initializes the user with a username, password, and email.
User::User(const string& username, const string& password, const string& email)
    : username(username), password(password), email(email) {
    // Optionally, you can add any initialization code here.
}

// Virtual destructor allows safe polymorphic use.
User::~User() {
    // Cleanup code (if required)
}

// Getter for username.
string User::getUsername() const {
    return username;
}

// Getter for email.
string User::getEmail() const {
    return email;
}

// Setter for username.
void User::setUsername(const string& username) {
    this->username = username;
}

// Setter for password.
void User::setPassword(const string& password) {
    this->password = simpleHash(username+password); // Hash the password before storing it.
}
// Login function: checks input credentials against stored credentials.
bool User::login(const string& inputUsername, const string& inputPassword) {
    if (inputUsername == username && simpleHash(inputUsername + inputPassword) == password) {
        cout << "Login successful for user: " << username << endl;
        return true;
    } else {
        cout << "Login failed for user: " << inputUsername << simpleHash(inputUsername + inputPassword) << endl;
        return false;
    }
}

// Setter for email.
void User::setEmail(const string& email) {
    this->email = email;
}



// Logout function: for this example it simply outputs a message.
void User::logout() {
    cout << "User " << username << " has logged out." << endl;
}
