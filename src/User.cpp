#include "User.hpp"

// Constructor: Initializes the user with a username, password, and email.
User::User(const std::string& username, const std::string& password, const std::string& email)
    : username(username), password(password), email(email) {
    // Optionally, you can add any initialization code here.
}

// Virtual destructor allows safe polymorphic use.
User::~User() {
    // Cleanup code (if required)
}

// Getter for username.
std::string User::getUsername() const {
    return username;
}

// Getter for email.
std::string User::getEmail() const {
    return email;
}

// Setter for username.
void User::setUsername(const std::string& username) {
    this->username = username;
}

// Setter for password.
void User::setPassword(const std::string& password) {
    this->password = password;
}

// Setter for email.
void User::setEmail(const std::string& email) {
    this->email = email;
}

// Login function: checks input credentials against stored credentials.
bool User::login(const std::string& inputUsername, const std::string& inputPassword) {
    if (inputUsername == username && inputPassword == password) {
        std::cout << "Login successful for user: " << username << std::endl;
        return true;
    } else {
        std::cout << "Login failed for user: " << inputUsername << std::endl;
        return false;
    }
}


// Logout function: for this example it simply outputs a message.
void User::logout() {
    std::cout << "User " << username << " has logged out." << std::endl;
}
