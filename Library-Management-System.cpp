#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class User {
protected:
    string name;
    string email;
public:
    User(string n = "", string e = "") : name(n), email(e) {}
};

class Member : public User {
public:
    int id;
    Member(int id, string n, string e) : User(n, e), id(id) {}
    void printMember() const {
        cout << "Member ID: " << id << " | Name: " << name << " | Email: " << email << "\n";
    }
};

class Book {
public:
    int ISBN;
    string title;
    string author;
    string genre;
    int totalCopies;
    int availableCopies;

    void printBook() const {
        cout << "----------------------------------------\n";
        cout << " ISBN      : " << ISBN << "\n";
        cout << " Title     : " << title << "\n";
        cout << " Author    : " << author << "\n";
        cout << " Genre     : " << genre << "\n";
        cout << " Copies    : " << availableCopies << " Available (Total: " << totalCopies << ")\n";
        cout << "----------------------------------------\n";
    }
};

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void loadBooksFromFile(vector<Book>& books) {
    ifstream inFile("library_data.txt");
    if (!inFile) return;

    books.clear();
    Book b;
    while (inFile >> b.ISBN) {
        inFile.ignore();
        getline(inFile, b.title);
        getline(inFile, b.author);
        getline(inFile, b.genre);
        inFile >> b.totalCopies;
        inFile >> b.availableCopies;
        books.push_back(b);
    }
    inFile.close();
}

void saveBooksToFile(const vector<Book>& books) {
    ofstream outFile("library_data.txt");
    for (const auto& b : books) {
        outFile << b.ISBN << "\n" << b.title << "\n" << b.author << "\n"
                << b.genre << "\n" << b.totalCopies << "\n" << b.availableCopies << "\n";
    }
    outFile.close();
}

void addBook(vector<Book>& books) {
    clearScreen();
    cout << "\n=== [ Add New Book ] ===\n";
    Book newBook;
    
    cout << "Enter Book ISBN (numbers only): ";
    cin >> newBook.ISBN;
    cin.ignore();

    cout << "Enter Book Title: ";
    getline(cin, newBook.title);

    cout << "Enter Author Name: ";
    getline(cin, newBook.author);

    cout << "Enter Book Genre (e.g., Psychology, Science): ";
    getline(cin, newBook.genre);

    cout << "Enter Total Copies: ";
    cin >> newBook.totalCopies;
    newBook.availableCopies = newBook.totalCopies; 

    books.push_back(newBook);
    saveBooksToFile(books);

    cout << "\n[✔] Book added and saved successfully!\n";
    cout << "\nPress Enter to return to main menu...";
    cin.ignore();
    cin.get();
}

void viewAllBooks(const vector<Book>& books) {
    clearScreen();
    cout << "\n=== [ Library Inventory ] ===\n";
    if (books.empty()) {
        cout << "\n[!] The library is currently empty.\n";
    } else {
        for (const auto& b : books) {
            b.printBook();
        }
    }
    cout << "\nPress Enter to return...";
    cin.ignore();
    cin.get();
}

void borrowBook(vector<Book>& books) {
    clearScreen();
    cout << "\n=== [ Borrow a Book ] ===\n";
    if (books.empty()) {
        cout << "\n[!] No books available to borrow.\n\nPress Enter to return...";
        cin.ignore(); cin.get(); return;
    }

    int searchISBN;
    cout << "Enter Book ISBN to borrow: ";
    cin >> searchISBN;

    bool found = false;
    for (auto& b : books) {
        if (b.ISBN == searchISBN) {
            found = true;
            if (b.availableCopies > 0) {
                b.availableCopies--;
                saveBooksToFile(books); 
                cout << "\n[✔] Success! You borrowed '" << b.title << "'.\n";
                cout << "Copies left: " << b.availableCopies << "\n";
            } else {
                cout << "\n[✗] Sorry, all copies of '" << b.title << "' are currently borrowed.\n";
            }
            break;
        }
    }

    if (!found) cout << "\n[✗] Book with ISBN " << searchISBN << " not found.\n";
    
    cout << "\nPress Enter to return...";
    cin.ignore(); cin.get();
}

void returnBook(vector<Book>& books) {
    clearScreen();
    cout << "\n=== [ Return a Book ] ===\n";
    
    int searchISBN;
    cout << "Enter Book ISBN to return: ";
    cin >> searchISBN;

    bool found = false;
    for (auto& b : books) {
        if (b.ISBN == searchISBN) {
            found = true;
            if (b.availableCopies < b.totalCopies) {
                b.availableCopies++;
                saveBooksToFile(books);
                cout << "\n[✔] Thank you for returning '" << b.title << "'.\n";
            } else {
                cout << "\n[!] Error: All copies of this book are already in the library.\n";
            }
            break;
        }
    }

    if (!found) cout << "\n[✗] Book with ISBN " << searchISBN << " not found in our system.\n";
    
    cout << "\nPress Enter to return...";
    cin.ignore(); cin.get();
}

int main() {
    vector<Book> libraryBooks;
    loadBooksFromFile(libraryBooks);

    int choice;
    do {
        clearScreen();
        cout << "========================================\n";
        cout << "   ADVANCED LIBRARY MANAGEMENT SYSTEM   \n";
        cout << "========================================\n";
        cout << " 1. Add a new book                      \n";
        cout << " 2. View all books                      \n";
        cout << " 3. Borrow a book                       \n";
        cout << " 4. Return a book                       \n";
        cout << " 5. Exit System                         \n";
        cout << "========================================\n";
        cout << "Enter your choice (1-5): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1: addBook(libraryBooks); break;
            case 2: viewAllBooks(libraryBooks); break;
            case 3: borrowBook(libraryBooks); break;
            case 4: returnBook(libraryBooks); break;
            case 5: 
                clearScreen();
                cout << "\nSaving data... Goodbye!\n\n"; 
                break;
            default:
                cout << "\n[!] Invalid choice.\nPress Enter to continue...";
                cin.ignore(); cin.get();
        }
    } while (choice != 5);

    return 0;
}
