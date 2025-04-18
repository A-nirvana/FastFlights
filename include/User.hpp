#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>

using namespace std;

class User {
protected:
    string username;
    string password;
    string email;

public:
    // Constructor
    User(const string& username, const string& password, const string& email);

    // Virtual destructor in case of inheritance
    virtual ~User();

    // Getter functions
    string getUsername() const;
    string getEmail() const;

    // Setter functions
    void setUsername(const string& username);
    void setPassword(const string& password);
    void setEmail(const string& email);

    // Member functions for user actions
    // The login function compares provided credentials with stored ones.
    virtual bool login(const string& inputUsername, const string& inputPassword);

    // A simple logout function that could be overridden in derived classes
    virtual void logout();
};

#endif // USER_HPP
