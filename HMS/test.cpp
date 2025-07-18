#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

// --------------------- Base Class: User ---------------------
class User {
public:
    string name;
    string password;

    User(string n, string p) : name(n), password(p) {}
    virtual void registeruser() = 0;
    virtual void loginuser(string p) = 0;
    virtual ~User() {}
};

// --------------------- Student Class ---------------------
class Student : public User {
    int roll_no;
    string department;

public:
    Student(string n, string p, int r, string d) : User(n, p), roll_no(r), department(d) {}

    void registeruser() override {
        ofstream file("student.txt", ios::app);
        if (file.is_open()) {
            file << "Name: " << name << "\n";
            file << "Password: " << password << "\n";
            file << "Roll.No: " << roll_no << "\n";
            file << "Department: " << department << "\n";
            file << "---\n";
            file.close();
            cout << "User registered: " << name << ", " << roll_no << endl;
        } else {
            cout << "Error opening file.\n";
        }
    }

    void loginuser(string p) override {
        ifstream file("student.txt");
        if (!file.is_open()) {
            cout << "File couldn't be opened.\n";
            return;
        }

        string line, uname, upass;
        while (getline(file, line)) {
            if (line.find("Name: ") == 0)
                uname = line.substr(6);
            else if (line.find("Password: ") == 0) {
                upass = line.substr(10);
                if (uname == name && upass == p) {
                    cout << "Login successful for " << name << endl;
                    file.close();
                    return;
                }
            }
        }

        file.close();
        cout << "Login failed.\n";
    }
};

// --------------------- Staff Class ---------------------
class Staff : public User {
    string designation;
    string department;

public:
    Staff(string n, string p, string r, string d) : User(n, p), designation(r), department(d) {}

    void registeruser() override {
        ofstream file("staff.txt", ios::app);
        if (file.is_open()) {
            file << "Name: " << name << "\n";
            file << "Password: " << password << "\n";
            file << "Designation: " << designation << "\n";
            file << "Department: " << department << "\n";
            file << "---\n";
            file.close();
            cout << "User registered: " << name << ", " << designation << endl;
        } else {
            cout << "Error opening file.\n";
        }
    }

    void loginuser(string p) override {
        ifstream file("staff.txt");
        if (!file.is_open()) {
            cout << "File couldn't be opened.\n";
            return;
        }

        string line, uname, upass;
        while (getline(file, line)) {
            if (line.find("Name: ") == 0)
                uname = line.substr(6);
            else if (line.find("Password: ") == 0) {
                upass = line.substr(10);
                if (uname == name && upass == p) {
                    cout << "Login successful for " << name << endl;
                    file.close();
                    return;
                }
            }
        }

        file.close();
        cout << "Login failed.\n";
    }
};

// --------------------- Admin Class ---------------------
class Admin : public User {
public:
    string email;

    Admin(string n, string p, string e) : User(n, p), email(e) {}

    void registeruser() override {
        ofstream file("admin.txt", ios::app);
        if (file.is_open()) {
            file << "Name: " << name << "\n";
            file << "Password: " << password << "\n";
            file << "Email: " << email << "\n";
            file << "---\n";
            file.close();
            cout << "Admin registered: " << name << endl;
        } else {
            cout << "Error opening file.\n";
        }
    }

    void loginuser(string password) override {
        ifstream file("admin.txt");
        string ss, ups, uname;
        while (getline(file, ss)) {
            if (ss.find("Name: ") == 0)
                uname = ss.substr(6);
            else if (ss.find("Password: ") == 0) {
                ups = ss.substr(10);
                if (uname == name && ups == password) {
                    cout << "Login Successful for admin: " << name << endl;
                    return;
                }
            }
        }
        cout << "Admin login failed.\n";
    }

    void addbook(string title, string author) {
        ifstream infile("books.txt");
        vector<string> lines;
        string line;
        bool found = false;
        int count = 0;

        while (getline(infile, line)) {
            if (line.find("Title: ") == 0 && line.substr(7) == title) {
                string nextLine;
                getline(infile, nextLine);
                if (nextLine.find("Author: ") == 0 && nextLine.substr(8) == author) {
                    string countLine;
                    getline(infile, countLine);
                    if (countLine.find("Count: ") == 0) {
                        count = stoi(countLine.substr(7));
                        count++;
                        lines.push_back("Title: " + title);
                        lines.push_back("Author: " + author);
                        lines.push_back("Count: " + to_string(count));
                        getline(infile, line); // skip separator
                        lines.push_back("---");
                        found = true;
                        continue;
                    }
                }
                lines.push_back(line);
                lines.push_back(nextLine);
            } else {
                lines.push_back(line);
            }
        }
        infile.close();

        if (!found) {
            lines.push_back("Title: " + title);
            lines.push_back("Author: " + author);
            lines.push_back("Count: 1");
            lines.push_back("---");
        }

        ofstream outfile("books.txt");
        for (string& l : lines) {
            outfile << l << "\n";
        }
        outfile.close();

        cout << (found ? "Book count updated.\n" : "Book registered.\n");
    }
};

// --------------------- Book Class ---------------------
class Book {
public:
    string author;
    string title;

    Book(string a, string t) : author(a), title(t) {}

    void borrowbook(string username) {
        ifstream infile("books.txt");
        ofstream temp("temp.txt");
        string line;
        bool found = false;
        int count = 0;

        while (getline(infile, line)) {
            if (line == "Title: " + title) {
                temp << line << "\n";
                getline(infile, line); // Author
                temp << line << "\n";
                getline(infile, line); // Count
                count = stoi(line.substr(7));
                if (count > 0) {
                    count--;
                    found = true;
                    temp << "Count: " << count << "\n";
                } else {
                    cout << "Book not available!\n";
                    temp << line << "\n";
                }
            } else {
                temp << line << "\n";
            }
        }

        infile.close();
        temp.close();
        remove("books.txt");
        rename("temp.txt", "books.txt");

        if (found) {
            ofstream bfile("borrowed.txt", ios::app);
            time_t now = time(0);
            string date = ctime(&now);
            date.pop_back();
            bfile << "Name: " << username << "\n";
            bfile << "Title: " << title << "\n";
            bfile << "Author: " << author << "\n";
            bfile << "Date: " << date << "\n";
            bfile << "---\n";
            bfile.close();
            cout << "Book borrowed successfully!\n";
        }
    }

    void returnbook(string username) {
        ifstream infile("books.txt");
        ofstream temp("temp.txt");
        string line;
        int count = 0;

        while (getline(infile, line)) {
            if (line == "Title: " + title) {
                temp << line << "\n";
                getline(infile, line); // Author
                temp << line << "\n";
                getline(infile, line); // Count
                count = stoi(line.substr(7));
                count++;
                temp << "Count: " << count << "\n";
            } else {
                temp << line << "\n";
            }
        }

        infile.close();
        temp.close();
        remove("books.txt");
        rename("temp.txt", "books.txt");

        cout << "Book returned successfully!\n";
    }

    void renewal(string username) {
        time_t now = time(0);
        string date = ctime(&now);
        date.pop_back();

        ofstream bfile("borrowed.txt", ios::app);
        bfile << "Name: " << username << "\n";
        bfile << "Title: " << title << "\n";
        bfile << "Author: " << author << "\n";
        bfile << "Renewed: " << date << "\n";
        bfile << "---\n";
        bfile.close();
        cout << "Book renewed!\n";
    }
};

// --------------------- Menus ---------------------
void studentMenu(string name) {
    while (true) {
        cout << "\n--- Student Menu ---\n";
        cout << "1. Borrow Book\n2. Return Book\n3. Renew Book\n4. Exit\nChoose: ";
        int ch;
        cin >> ch;
        cin.ignore();

        if (ch == 4) break;

        string title, author;
        cout << "Enter Title: "; getline(cin, title);
        cout << "Enter Author: "; getline(cin, author);

        Book b(author, title);

        switch (ch) {
            case 1: b.borrowbook(name); break;
            case 2: b.returnbook(name); break;
            case 3: b.renewal(name); break;
            default: cout << "Invalid.\n";
        }
    }
}

void staffMenu(string name) {
    studentMenu(name);
}

void adminMenu(string name) {
    while (true) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Book\n2. Exit\nChoose: ";
        int ch;
        cin >> ch;
        cin.ignore();

        if (ch == 2) break;

        string title, author;
        cout << "Enter Title: "; getline(cin, title);
        cout << "Enter Author: "; getline(cin, author);

        Admin a(name, "", ""); // Dummy object
        a.addbook(title, author);
    }
}

// --------------------- Main ---------------------
int main() {
    int roleChoice;
    cout << "======= Library Management System =======\n";
    cout << "Select Role:\n1. Student\n2. Staff\n3. Admin\nEnter choice: ";
    cin >> roleChoice;
    cin.ignore();

    string name, password;
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Password: ";
    getline(cin, password);

    int action;
    cout << "1. Register\n2. Login\nChoose: ";
    cin >> action;
    cin.ignore();

    bool success = false;

    if (roleChoice == 1) {
        int roll;
        string dept;
        cout << "Enter Roll No: ";
        cin >> roll;
        cin.ignore();
        cout << "Enter Department: ";
        getline(cin, dept);
        Student s(name, password, roll, dept);

        if (action == 1) {
            s.registeruser();
        } else if (action == 2) {
            while (!success) {
                ifstream file("student.txt");
                string line, uname, upass;
                while (getline(file, line)) {
                    if (line.find("Name: ") == 0)
                        uname = line.substr(6);
                    else if (line.find("Password: ") == 0) {
                        upass = line.substr(10);
                        if (uname == name && upass == password) {
                            s.loginuser(password);
                            studentMenu(name);
                            success = true;
                            break;
                        }
                    }
                }
                file.close();
                if (!success) {
                    cout << "Incorrect credentials. Try again.\n";
                    cout << "Enter Password: ";
                    getline(cin, password);
                }
            }
        }
    }
    else if (roleChoice == 2) {
        string desig, dept;
        cout << "Enter Designation: ";
        getline(cin, desig);
        cout << "Enter Department: ";
        getline(cin, dept);
        Staff st(name, password, desig, dept);

        if (action == 1) {
            st.registeruser();
        } else if (action == 2) {
            while (!success) {
                ifstream file("staff.txt");
                string line, uname, upass;
                while (getline(file, line)) {
                    if (line.find("Name: ") == 0)
                        uname = line.substr(6);
                    else if (line.find("Password: ") == 0) {
                        upass = line.substr(10);
                        if (uname == name && upass == password) {
                            st.loginuser(password);
                            staffMenu(name);
                            success = true;
                            break;
                        }
                    }
                }
                file.close();
                if (!success) {
                    cout << "Incorrect credentials. Try again.\n";
                    cout << "Enter Password: ";
                    getline(cin, password);
                }
            }
        }
    }
    else if (roleChoice == 3) {
        string email;
        cout << "Enter Email: ";
        getline(cin, email);
        Admin a(name, password, email);

        if (action == 1) {
            a.registeruser();
        } else if (action == 2) {
            while (!success) {
                ifstream file("admin.txt");
                string line, uname, upass;
                while (getline(file, line)) {
                    if (line.find("Name: ") == 0)
                        uname = line.substr(6);
                    else if (line.find("Password: ") == 0) {
                        upass = line.substr(10);
                        if (uname == name && upass == password) {
                            a.loginuser(password);
                            adminMenu(name);
                            success = true;
                            break;
                        }
                    }
                }
                file.close();
                if (!success) {
                    cout << "Incorrect credentials. Try again.\n";
                    cout << "Enter Password: ";
                    getline(cin, password);
                }
            }
        }
    } else {
        cout << "Invalid role choice.\n";
    }

    return 0;
}
