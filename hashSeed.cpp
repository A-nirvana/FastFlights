#include <iostream>
#include "HashUtil.hpp"

using namespace std;
int main() {
    int n;
    cin>> n;
    while(n--){
        string pass;
        cout << "Enter plaintext password: ";
        cin >> pass;
        cout << "Hashed: " << simpleHash(pass) << endl;
    }
    return 0;
}