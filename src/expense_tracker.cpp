/*
 * Lab Sheet-09 : Smart Digital Expense Tracker with Budget Prediction
 * BCA I Semester (Session 2026-2027)
 *
 * Modules:
 *  1. User Registration      7. Budget Setting
 *  2. Login System           8. Budget Warning
 *  3. Add Daily Expense      9. Expense Prediction
 *  4. View Expense History  10. Search Expense
 *  5. Category-wise Expense 11. Delete Expense
 *  6. Monthly Report        12. File Saving
 *
 * Concepts used: variables, loops, arrays, structures, functions,
 *                file handling, strings, searching, sorting.
 *
 * Files created by the program:
 *   users.txt              -> username password budget
 *   expenses_<user>.txt    -> id|date|category|description|amount
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

// ---------------------------------------------------------------
// Constants
// ---------------------------------------------------------------
const int MAX_USERS = 50;
const int MAX_EXPENSES = 1000;
const int NUM_CATEGORIES = 6;
const string CATEGORIES[NUM_CATEGORIES] = {
    "Food", "Travel", "Shopping", "Education", "Entertainment", "Other"
};

// ---------------------------------------------------------------
// Structures
// ---------------------------------------------------------------
struct User {
    string username;
    string password;
    double monthlyBudget;
};

struct Expense {
    int id;
    string date;          // format DD-MM-YYYY
    string category;
    string description;
    double amount;
};

// ---------------------------------------------------------------
// Global data (arrays of structures)
// ---------------------------------------------------------------
User users[MAX_USERS];
int userCount = 0;

Expense expenses[MAX_EXPENSES];
int expenseCount = 0;

int currentUser = -1;   // index of logged-in user in users[]
int nextExpenseId = 1;

// ---------------------------------------------------------------
// Input helper functions
// ---------------------------------------------------------------
string readLine(const string &prompt) {
    string line;
    cout << prompt;
    getline(cin, line);
    return line;
}

// Reads an integer between low and high (keeps asking until valid)
int readInt(const string &prompt, int low, int high) {
    while (true) {
        string line = readLine(prompt);
        istringstream iss(line);
        int value;
        if ((iss >> value) && (iss >> ws).eof() && value >= low && value <= high)
            return value;
        cout << "  Invalid input! Enter a number between " << low << " and " << high << ".\n";
    }
}

// Reads a positive amount (keeps asking until valid)
double readAmount(const string &prompt) {
    while (true) {
        string line = readLine(prompt);
        istringstream iss(line);
        double value;
        if ((iss >> value) && (iss >> ws).eof() && value > 0)
            return value;
        cout << "  Invalid amount! Enter a number greater than 0.\n";
    }
}

// ---------------------------------------------------------------
// String / date helper functions
// ---------------------------------------------------------------
string toLower(string s) {
    for (size_t i = 0; i < s.length(); i++)
        s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

bool containsIgnoreCase(const string &text, const string &word) {
    return toLower(text).find(toLower(word)) != string::npos;
}

bool hasSpace(const string &s) {
    return s.find(' ') != string::npos;
}

bool isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int daysInMonth(int m, int y) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && isLeapYear(y)) return 29;
    return days[m - 1];
}

void getToday(int &d, int &m, int &y) {
    time_t now = time(0);
    tm *t = localtime(&now);
    d = t->tm_mday;
    m = t->tm_mon + 1;
    y = t->tm_year + 1900;
}

string makeDate(int d, int m, int y) {
    ostringstream oss;
    oss << setfill('0') << setw(2) << d << "-" << setw(2) << m << "-" << setw(4) << y;
    return oss.str();
}

string makeMonthKey(int m, int y) {      // MM-YYYY
    ostringstream oss;
    oss << setfill('0') << setw(2) << m << "-" << setw(4) << y;
    return oss.str();
}

string todayString() {
    int d, m, y;
    getToday(d, m, y);
    return makeDate(d, m, y);
}

string currentMonthKey() {
    int d, m, y;
    getToday(d, m, y);
    return makeMonthKey(m, y);
}

// Checks DD-MM-YYYY format and real calendar validity
bool isValidDate(const string &s) {
    if (s.length() != 10 || s[2] != '-' || s[5] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit((unsigned char)s[i])) return false;
    }
    int d = stoi(s.substr(0, 2));
    int m = stoi(s.substr(3, 2));
    int y = stoi(s.substr(6, 4));
    if (y < 2000 || y > 2100 || m < 1 || m > 12) return false;
    return d >= 1 && d <= daysInMonth(m, y);
}

bool isValidMonthKey(const string &s) {
    if (s.length() != 7 || s[2] != '-') return false;
    for (int i = 0; i < 7; i++) {
        if (i == 2) continue;
        if (!isdigit((unsigned char)s[i])) return false;
    }
    int m = stoi(s.substr(0, 2));
    return m >= 1 && m <= 12;
}

// "DD-MM-YYYY" -> YYYYMMDD number (used for sorting)
int dateKey(const string &date) {
    int d = stoi(date.substr(0, 2));
    int m = stoi(date.substr(3, 2));
    int y = stoi(date.substr(6, 4));
    return y * 10000 + m * 100 + d;
}

// "DD-MM-YYYY" -> "MM-YYYY"
string monthOf(const string &date) {
    return date.substr(3, 7);
}

string readDate() {
    while (true) {
        string d = readLine("Enter date (DD-MM-YYYY) or press Enter for today: ");
        if (d.empty()) return todayString();
        if (isValidDate(d)) return d;
        cout << "  Invalid date! Use DD-MM-YYYY (example: 05-10-2026).\n";
    }
}

string readMonth() {
    while (true) {
        string m = readLine("Enter month (MM-YYYY) or press Enter for current month: ");
        if (m.empty()) return currentMonthKey();
        if (isValidMonthKey(m)) return m;
        cout << "  Invalid month! Use MM-YYYY (example: 10-2026).\n";
    }
}

string chooseCategory() {
    cout << "\nSelect Category:\n";
    for (int i = 0; i < NUM_CATEGORIES; i++)
        cout << "  " << (i + 1) << ". " << CATEGORIES[i] << "\n";
    int c = readInt("Enter category number: ", 1, NUM_CATEGORIES);
    return CATEGORIES[c - 1];
}

// ---------------------------------------------------------------
// File handling
// ---------------------------------------------------------------
void loadUsers() {
    ifstream fin("users.txt");
    userCount = 0;
    while (userCount < MAX_USERS &&
           fin >> users[userCount].username >> users[userCount].password
               >> users[userCount].monthlyBudget) {
        userCount++;
    }
    fin.close();
}

void saveUsers() {
    ofstream fout("users.txt");
    for (int i = 0; i < userCount; i++)
        fout << users[i].username << " " << users[i].password << " "
             << users[i].monthlyBudget << "\n";
    fout.close();
}

string expenseFileName() {
    return "expenses_" + users[currentUser].username + ".txt";
}

void loadExpenses() {
    expenseCount = 0;
    nextExpenseId = 1;
    ifstream fin(expenseFileName().c_str());
    string line;
    while (expenseCount < MAX_EXPENSES && getline(fin, line)) {
        if (line.empty()) continue;
        string parts[5];
        int p = 0;
        string cur;
        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] == '|' && p < 4) { parts[p++] = cur; cur = ""; }
            else cur += line[i];
        }
        parts[p] = cur;
        if (p != 4) continue;                       // skip damaged lines
        Expense e;
        e.id = stoi(parts[0]);
        e.date = parts[1];
        e.category = parts[2];
        e.description = parts[3];
        e.amount = stod(parts[4]);
        expenses[expenseCount++] = e;
        if (e.id >= nextExpenseId) nextExpenseId = e.id + 1;
    }
    fin.close();
}

void saveExpenses() {
    ofstream fout(expenseFileName().c_str());
    for (int i = 0; i < expenseCount; i++)
        fout << expenses[i].id << "|" << expenses[i].date << "|"
             << expenses[i].category << "|" << expenses[i].description << "|"
             << expenses[i].amount << "\n";
    fout.close();
}

// ---------------------------------------------------------------
// Calculation helpers
// ---------------------------------------------------------------
double monthTotal(const string &monthKey) {
    double total = 0;
    for (int i = 0; i < expenseCount; i++)
        if (monthOf(expenses[i].date) == monthKey)
            total += expenses[i].amount;
    return total;
}

void printHeader() {
    cout << left << setw(5) << "ID" << setw(13) << "Date" << setw(15) << "Category"
         << setw(22) << "Description" << right << setw(10) << "Amount" << "\n";
    cout << string(65, '-') << "\n";
}

void printExpense(const Expense &e) {
    string desc = e.description.length() > 20 ? e.description.substr(0, 20) : e.description;
    cout << left << setw(5) << e.id << setw(13) << e.date << setw(15) << e.category
         << setw(22) << desc << right << setw(10) << fixed << setprecision(2)
         << e.amount << "\n";
}

// ---------------------------------------------------------------
// Module 1 : User Registration
// ---------------------------------------------------------------
void registerUser() {
    cout << "\n--- USER REGISTRATION ---\n";
    if (userCount >= MAX_USERS) {
        cout << "User limit reached. Cannot register more users.\n";
        return;
    }
    string name;
    while (true) {
        name = readLine("Choose username (no spaces): ");
        if (name.empty() || hasSpace(name)) {
            cout << "  Username cannot be empty or contain spaces.\n";
            continue;
        }
        bool exists = false;
        for (int i = 0; i < userCount; i++)
            if (users[i].username == name) exists = true;
        if (exists) { cout << "  Username already taken. Try another.\n"; continue; }
        break;
    }
    string pass;
    while (true) {
        pass = readLine("Choose password (no spaces): ");
        if (pass.empty() || hasSpace(pass)) {
            cout << "  Password cannot be empty or contain spaces.\n";
            continue;
        }
        break;
    }
    double budget = readAmount("Set your monthly budget (Rs): ");

    users[userCount].username = name;
    users[userCount].password = pass;
    users[userCount].monthlyBudget = budget;
    userCount++;
    saveUsers();
    cout << "Registration successful! Please login.\n";
}

// ---------------------------------------------------------------
// Module 2 : Login System
// ---------------------------------------------------------------
bool loginUser() {
    cout << "\n--- LOGIN ---\n";
    for (int attempt = 1; attempt <= 3; attempt++) {
        string name = readLine("Username: ");
        string pass = readLine("Password: ");
        for (int i = 0; i < userCount; i++) {
            if (users[i].username == name && users[i].password == pass) {
                currentUser = i;
                loadExpenses();
                cout << "Login successful. Welcome, " << name << "!\n";
                return true;
            }
        }
        cout << "Wrong username or password. Attempts left: " << (3 - attempt) << "\n";
    }
    return false;
}

// ---------------------------------------------------------------
// Module 8 : Budget Warning
// ---------------------------------------------------------------
void budgetWarning(const string &monthKey) {
    double budget = users[currentUser].monthlyBudget;
    double spent = monthTotal(monthKey);
    double percent = (budget > 0) ? (spent / budget) * 100 : 0;

    cout << "\n--- BUDGET STATUS (" << monthKey << ") ---\n";
    cout << fixed << setprecision(2);
    cout << "Monthly Budget : Rs " << budget << "\n";
    cout << "Spent So Far   : Rs " << spent << " (" << percent << "%)\n";

    if (spent > budget)
        cout << "!! WARNING: Budget EXCEEDED by Rs " << (spent - budget) << " !!\n";
    else if (percent >= 90)
        cout << "!! ALERT: You have used 90% or more of your budget !!\n";
    else if (percent >= 75)
        cout << "! Caution: You have used 75% or more of your budget.\n";
    else
        cout << "You are within budget. Remaining: Rs " << (budget - spent) << "\n";
}

// ---------------------------------------------------------------
// Module 3 : Add Daily Expense
// ---------------------------------------------------------------
void addExpense() {
    cout << "\n--- ADD EXPENSE ---\n";
    if (expenseCount >= MAX_EXPENSES) {
        cout << "Expense storage is full!\n";
        return;
    }
    Expense e;
    e.id = nextExpenseId++;
    e.date = readDate();
    e.category = chooseCategory();
    e.description = readLine("Enter description: ");
    if (e.description.empty()) e.description = "-";
    for (size_t i = 0; i < e.description.length(); i++)   // '|' is used as separator
        if (e.description[i] == '|') e.description[i] = '-';
    e.amount = readAmount("Enter amount (Rs): ");

    expenses[expenseCount++] = e;
    saveExpenses();
    cout << "Expense added and saved (ID " << e.id << ").\n";

    // Automatic warning for the month of this expense
    if (monthTotal(monthOf(e.date)) > users[currentUser].monthlyBudget)
        cout << "!! WARNING: This month's budget has been exceeded !!\n";
    else if (monthTotal(monthOf(e.date)) >= 0.9 * users[currentUser].monthlyBudget)
        cout << "!! ALERT: You have used 90% or more of this month's budget !!\n";
}

// ---------------------------------------------------------------
// Module 4 : View Expense History (with sorting)
// ---------------------------------------------------------------
void viewHistory() {
    cout << "\n--- EXPENSE HISTORY ---\n";
    if (expenseCount == 0) {
        cout << "No expenses recorded yet.\n";
        return;
    }
    cout << "Sort by:\n  1. Date (oldest first)\n  2. Date (newest first)\n"
         << "  3. Amount (highest first)\n  4. Order of entry\n";
    int choice = readInt("Enter choice: ", 1, 4);

    // order[] holds indexes; we sort the indexes (Bubble Sort)
    int order[MAX_EXPENSES];
    for (int i = 0; i < expenseCount; i++) order[i] = i;

    for (int i = 0; i < expenseCount - 1; i++) {
        for (int j = 0; j < expenseCount - i - 1; j++) {
            const Expense &a = expenses[order[j]];
            const Expense &b = expenses[order[j + 1]];
            bool swapNeeded = false;
            if (choice == 1) swapNeeded = dateKey(a.date) > dateKey(b.date);
            else if (choice == 2) swapNeeded = dateKey(a.date) < dateKey(b.date);
            else if (choice == 3) swapNeeded = a.amount < b.amount;
            if (swapNeeded) {
                int temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }

    cout << "\n";
    printHeader();
    double total = 0;
    for (int i = 0; i < expenseCount; i++) {
        printExpense(expenses[order[i]]);
        total += expenses[order[i]].amount;
    }
    cout << string(65, '-') << "\n";
    cout << "Total expenses: Rs " << fixed << setprecision(2) << total << "\n";
}

// ---------------------------------------------------------------
// Module 5 : Category-wise Expense
// ---------------------------------------------------------------
void categoryWise() {
    cout << "\n--- CATEGORY-WISE EXPENSE ---\n";
    string monthKey = readLine("Enter month (MM-YYYY) or press Enter for ALL time: ");
    if (!monthKey.empty() && !isValidMonthKey(monthKey)) {
        cout << "Invalid month format.\n";
        return;
    }

    double catTotal[NUM_CATEGORIES] = {0};
    double grand = 0;
    for (int i = 0; i < expenseCount; i++) {
        if (!monthKey.empty() && monthOf(expenses[i].date) != monthKey) continue;
        for (int c = 0; c < NUM_CATEGORIES; c++) {
            if (expenses[i].category == CATEGORIES[c]) catTotal[c] += expenses[i].amount;
        }
        grand += expenses[i].amount;
    }

    cout << "\n" << left << setw(16) << "Category" << right << setw(12) << "Total (Rs)"
         << setw(10) << "Share" << "\n";
    cout << string(38, '-') << "\n";
    cout << fixed << setprecision(2);
    for (int c = 0; c < NUM_CATEGORIES; c++) {
        double share = (grand > 0) ? (catTotal[c] / grand) * 100 : 0;
        cout << left << setw(16) << CATEGORIES[c] << right << setw(12) << catTotal[c]
             << setw(9) << share << "%\n";
    }
    cout << string(38, '-') << "\n";
    cout << left << setw(16) << "TOTAL" << right << setw(12) << grand << "\n";
}

// ---------------------------------------------------------------
// Module 6 : Monthly Expense Report
// ---------------------------------------------------------------
void monthlyReport() {
    cout << "\n--- MONTHLY EXPENSE REPORT ---\n";
    string monthKey = readMonth();

    int count = 0;
    int highest = -1;
    cout << "\nReport for " << monthKey << " (user: " << users[currentUser].username << ")\n\n";
    printHeader();
    for (int i = 0; i < expenseCount; i++) {
        if (monthOf(expenses[i].date) != monthKey) continue;
        printExpense(expenses[i]);
        count++;
        if (highest == -1 || expenses[i].amount > expenses[highest].amount) highest = i;
    }
    if (count == 0) {
        cout << "No expenses found for this month.\n";
        return;
    }
    double total = monthTotal(monthKey);
    double budget = users[currentUser].monthlyBudget;
    cout << string(65, '-') << "\n" << fixed << setprecision(2);
    cout << "Number of expenses : " << count << "\n";
    cout << "Total spent        : Rs " << total << "\n";
    cout << "Monthly budget     : Rs " << budget << "\n";
    if (total > budget) cout << "Status             : EXCEEDED by Rs " << (total - budget) << "\n";
    else cout << "Status             : Within budget, Rs " << (budget - total) << " left\n";
    cout << "Highest expense    : Rs " << expenses[highest].amount
         << " (" << expenses[highest].description << ")\n";
}

// ---------------------------------------------------------------
// Module 7 : Budget Setting
// ---------------------------------------------------------------
void setBudget() {
    cout << "\n--- BUDGET SETTING ---\n";
    cout << fixed << setprecision(2);
    cout << "Current monthly budget: Rs " << users[currentUser].monthlyBudget << "\n";
    users[currentUser].monthlyBudget = readAmount("Enter new monthly budget (Rs): ");
    saveUsers();
    cout << "Budget updated and saved.\n";
}

// ---------------------------------------------------------------
// Module 9 : Expense Prediction
// ---------------------------------------------------------------
void predictExpense() {
    cout << "\n--- EXPENSE PREDICTION (current month) ---\n";
    int day, month, year;
    getToday(day, month, year);
    string monthKey = makeMonthKey(month, year);
    int totalDays = daysInMonth(month, year);

    double spent = monthTotal(monthKey);
    double budget = users[currentUser].monthlyBudget;
    double dailyAverage = spent / day;
    double predicted = dailyAverage * totalDays;

    cout << fixed << setprecision(2);
    cout << "Today is day " << day << " of " << totalDays << " (" << monthKey << ")\n";
    cout << "Spent so far            : Rs " << spent << "\n";
    cout << "Average spending per day: Rs " << dailyAverage << "\n";
    cout << "Predicted month total   : Rs " << predicted << "\n";
    cout << "Monthly budget          : Rs " << budget << "\n\n";

    if (predicted > budget) {
        cout << "!! PREDICTION: You are likely to EXCEED your budget by Rs "
             << (predicted - budget) << " !!\n";
        int daysLeft = totalDays - day;
        if (spent < budget && daysLeft > 0)
            cout << "Tip: To stay in budget, spend at most Rs "
                 << (budget - spent) / daysLeft << " per day for the remaining "
                 << daysLeft << " days.\n";
        else if (spent >= budget)
            cout << "You have already crossed your budget. Avoid extra spending.\n";
    } else {
        cout << "Good news: At this rate you will stay within budget (Rs "
             << (budget - predicted) << " expected to be left).\n";
    }
}

// ---------------------------------------------------------------
// Module 10 : Search Expense (linear search)
// ---------------------------------------------------------------
void searchExpense() {
    cout << "\n--- SEARCH EXPENSE ---\n";
    cout << "Search by:\n  1. Keyword (description or category)\n  2. Date\n  3. Amount range\n";
    int choice = readInt("Enter choice: ", 1, 3);

    string keyword, date;
    double low = 0, high = 0;
    if (choice == 1) {
        keyword = readLine("Enter keyword: ");
    } else if (choice == 2) {
        while (true) {
            date = readLine("Enter date (DD-MM-YYYY): ");
            if (isValidDate(date)) break;
            cout << "  Invalid date!\n";
        }
    } else {
        low = readAmount("Minimum amount: ");
        high = readAmount("Maximum amount: ");
        if (low > high) { double t = low; low = high; high = t; }
    }

    bool found = false;
    for (int i = 0; i < expenseCount; i++) {
        bool match = false;
        if (choice == 1)
            match = containsIgnoreCase(expenses[i].description, keyword) ||
                    containsIgnoreCase(expenses[i].category, keyword);
        else if (choice == 2)
            match = (expenses[i].date == date);
        else
            match = (expenses[i].amount >= low && expenses[i].amount <= high);

        if (match) {
            if (!found) { cout << "\n"; printHeader(); }
            printExpense(expenses[i]);
            found = true;
        }
    }
    if (!found) cout << "No matching expense found.\n";
}

// ---------------------------------------------------------------
// Module 11 : Delete Expense
// ---------------------------------------------------------------
void deleteExpense() {
    cout << "\n--- DELETE EXPENSE ---\n";
    if (expenseCount == 0) {
        cout << "No expenses to delete.\n";
        return;
    }
    printHeader();
    for (int i = 0; i < expenseCount; i++) printExpense(expenses[i]);

    int id = readInt("\nEnter ID of the expense to delete: ", 1, 1000000);
    int index = -1;
    for (int i = 0; i < expenseCount; i++)
        if (expenses[i].id == id) { index = i; break; }

    if (index == -1) {
        cout << "No expense with ID " << id << " found.\n";
        return;
    }
    string sure = readLine("Delete this expense? (y/n): ");
    if (sure == "y" || sure == "Y") {
        for (int i = index; i < expenseCount - 1; i++)   // shift left
            expenses[i] = expenses[i + 1];
        expenseCount--;
        saveExpenses();
        cout << "Expense deleted and file updated.\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
}

// ---------------------------------------------------------------
// User menu (after login)
// ---------------------------------------------------------------
void userMenu() {
    int choice;
    do {
        cout << "\n========= MAIN MENU (" << users[currentUser].username << ") =========\n";
        cout << " 1. Add Daily Expense\n";
        cout << " 2. View Expense History\n";
        cout << " 3. Category-wise Expense\n";
        cout << " 4. Monthly Expense Report\n";
        cout << " 5. Set Monthly Budget\n";
        cout << " 6. Budget Warning / Status\n";
        cout << " 7. Expense Prediction\n";
        cout << " 8. Search Expense\n";
        cout << " 9. Delete Expense\n";
        cout << "10. Save & Logout\n";
        choice = readInt("Enter choice: ", 1, 10);

        switch (choice) {
            case 1: addExpense(); break;
            case 2: viewHistory(); break;
            case 3: categoryWise(); break;
            case 4: monthlyReport(); break;
            case 5: setBudget(); break;
            case 6: budgetWarning(currentMonthKey()); break;
            case 7: predictExpense(); break;
            case 8: searchExpense(); break;
            case 9: deleteExpense(); break;
            case 10:
                saveExpenses();      // Module 12 : File Saving
                saveUsers();
                cout << "Data saved. Logged out successfully.\n";
                break;
        }
    } while (choice != 10);
    currentUser = -1;
}

// ---------------------------------------------------------------
// main
// ---------------------------------------------------------------
int main() {
    loadUsers();
    int choice;
    do {
        cout << "\n===== SMART DIGITAL EXPENSE TRACKER =====\n";
        cout << "1. Register\n2. Login\n3. Exit\n";
        choice = readInt("Enter choice: ", 1, 3);
        if (choice == 1) registerUser();
        else if (choice == 2) {
            if (loginUser()) userMenu();
            else cout << "Too many failed attempts. Returning to main menu.\n";
        }
    } while (choice != 3);

    cout << "Thank you for using Expense Tracker. Goodbye!\n";
    return 0;
}
