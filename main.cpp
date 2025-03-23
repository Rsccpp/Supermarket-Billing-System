#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <windows.h>
using namespace std;

class Bill {
private:
    string Item;
    int Rate, Quantity;

public:
    Bill() : Item(""), Rate(0), Quantity(0) {}

    void setItem(string item) { Item = item; }
    void setRate(int rate) { Rate = rate; }
    void setQuant(int quant) { Quantity = quant; }

    string getItem() { return Item; }
    int getRate() { return Rate; }
    int getQuant() { return Quantity; }
};

// Function to convert a string to lowercase
string toLower(string str) {
    for (char &c : str) c = tolower(c);
    return str;
}

// Function to trim whitespace from both ends of a string
string trim(string str) {
    str.erase(0, str.find_first_not_of(" "));  // Trim leading spaces
    str.erase(str.find_last_not_of(" ") + 1);  // Trim trailing spaces
    return str;
}

// Function to add items
void addItem(Bill &b) {
    bool close = false;
    while (!close) {
        int choice;
        cout << "\n1. Add Item\n2. Close\nEnter Choice: ";
        cin >> choice;

        if (choice == 1) {
            string item;
            int rate, quantity;

            cout << "Enter Item Name: ";
            cin >> ws;
            getline(cin, item);
            b.setItem(item);

            cout << "Enter Rate of Item: ";
            cin >> rate;
            b.setRate(rate);

            cout << "Enter Quantity of Item: ";
            cin >> quantity;
            b.setQuant(quantity);

            ofstream out("Data_File.txt", ios::app);
            if (!out) {
                cout << "Error: File Can't Open!\n";
            } else {
                out << item << " : " << rate << " : " << quantity << endl;
                cout << "Item Added Successfully!\n";
            }
            out.close();
        } else if (choice == 2) {
            close = true;
            cout << "Back to Main Menu!\n";
        }
    }
}

// Function to print the bill
void printBill() {
    int total = 0;
    bool close = false;

    while (!close) {
        int choice;
        cout << "\n1. Add to Bill\n2. Close Session\nEnter Choice: ";
        cin >> choice;

        if (choice == 1) {
            string item;
            int quant;

            cout << "Enter Item Name: ";
            cin >> ws;
            getline(cin, item);
            cout << "Enter Quantity: ";
            cin >> quant;

            ifstream in("Data_File.txt");
            ofstream out("Data_Temp.txt");
            string line;
            bool found = false;

            if (!in || !out) {
                cout << "Error: File Can't Open!\n";
                return;
            }

            while (getline(in, line)) {
                stringstream ss(line);
                string itemName;
                int itemRate, itemQuant;
                char delimiter;

                // Read item name properly
                getline(ss, itemName, ':');
                itemName = trim(itemName);

                // Read rate and quantity properly
                ss >> itemRate;
                ss.ignore(2, ':');  // Ignore the next ":"
                ss >> itemQuant;

                cout << "DEBUG: Entered Item: [" << item << "], File Item: [" << itemName << "]\n";

                // Convert both names to lowercase for case-insensitive matching
                if (toLower(item) == toLower(itemName)) {
                    found = true;
                    if (quant <= itemQuant) {
                        int amount = itemRate * quant;
                        cout << setw(10) << "Item" << setw(10) << "Rate" << setw(10) << "Qty" << setw(10) << "Amount" << endl;
                        cout << setw(10) << itemName << setw(10) << itemRate << setw(10) << quant << setw(10) << amount << endl;

                        itemQuant -= quant;
                        total += amount;
                        out << itemName << " : " << itemRate << " : " << itemQuant << endl;
                    } else {
                        cout << "Insufficient Stock for " << item << "!\n";
                    }
                } else {
                    out << line << endl;
                }
            }

            if (!found) {
                cout << "Item Not Found!\n";
            }

            in.close();
            out.close();

            remove("Data_File.txt");
            rename("Data_Temp.txt", "Data_File.txt");

        } else if (choice == 2) {
            close = true;
            cout << "\nCalculating Total Bill...\n";
        }
    }

    cout << "\nTotal Bill: " << total << endl;
    cout << "Thanks for Shopping!\n";
    Sleep(2000);
}


// Main Function

int main() {
    Bill b;
    bool exit = false;

    while (!exit) {
        int val;
        cout << "\nWelcome to Supermarket Billing System\n";
        cout << "**************************************\n";
        cout << "1. Add Item\n2. Print Bill\n3. Exit\nEnter Choice: ";
        cin >> val;

        if (val == 1) {
            addItem(b);
        } else if (val == 2) {
            printBill();
        } else if (val == 3) {
            exit = true;
            cout << "Goodbye!\n";
            Sleep(2000);
        }
    }

    return 0;
}
