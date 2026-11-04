# Smart Digital Expense Tracker with Budget Prediction

A menu-driven **C++ console application** that lets students record daily expenses,
categorize spending, watch a monthly budget and **predict whether the budget will be
exceeded** before the month ends.

> Academic project: Lab Sheet-09, BCA I Semester (Session 2026-2027), COER University, Roorkee.

---

## Table of Contents
- [Features](#features)
- [The 12 Modules](#the-12-modules)
- [Technologies Used](#technologies-used)
- [C++ Concepts Demonstrated](#c-concepts-demonstrated)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Compilation](#compilation)
- [Running the Application](#running-the-application)
- [Sample Terminal Output](#sample-terminal-output)
- [Data Storage](#data-storage)
- [Security Notice](#security-notice)
- [Known Limitations](#known-limitations)
- [Future Enhancements](#future-enhancements)
- [Academic Information](#academic-information)
- [Author](#author)

## Features
- Multi-user registration and login (3 attempts per login)
- Daily expense entry with date validation (real calendar dates, leap years handled)
- Six categories: Food, Travel, Shopping, Education, Entertainment, Other
- Expense history sorted by date (oldest/newest), amount, or order of entry
- Category-wise totals with percentage share (for one month or all time)
- Monthly report with highest expense and budget status
- Budget warnings at 75%, 90% and when exceeded
- **Month-end spending prediction** with a suggested daily limit
- Search by keyword (case-insensitive), exact date, or amount range
- Delete an expense with confirmation
- Automatic saving to text files after every change

## The 12 Modules

| # | Module | Description | Menu option |
|---|--------|-------------|-------------|
| 1 | User Registration | Create an account with username, password and monthly budget | Start menu: 1 |
| 2 | Login System | Authenticate with up to 3 attempts | Start menu: 2 |
| 3 | Add Daily Expense | Date, category, description, amount | 1 |
| 4 | View Expense History | List all expenses with sorting options | 2 |
| 5 | Category-wise Expense | Totals and percentage share per category | 3 |
| 6 | Monthly Expense Report | All expenses for a month, totals, highest expense | 4 |
| 7 | Budget Setting | Change the monthly budget | 5 |
| 8 | Budget Warning | Caution (75%), alert (90%), exceeded (100%+) | 6 (also shown automatically after adding) |
| 9 | Expense Prediction | Projects the month-end total from the daily average | 7 |
| 10 | Search Expense | Keyword, date or amount-range search | 8 |
| 11 | Delete Expense | Remove an expense by ID | 9 |
| 12 | File Saving | Persist users and expenses to text files | Automatic; also on option 10 (Save & Logout) |

### How prediction works
```
Daily average   = spent so far this month / today's day number
Predicted total = daily average x days in this month
```
If the predicted total is above the budget, the program warns you and suggests a maximum
daily spend for the remaining days.

## Technologies Used
- **Language:** C++ (standard C++11 or later)
- **Standard library only:** `iostream`, `fstream`, `sstream`, `string`, `iomanip`, `ctime`
- **Storage:** plain text files
- **Compiler:** any standard-compliant compiler (g++, clang++, MSVC, MinGW)

## C++ Concepts Demonstrated
Variables, loops, arrays, structures (`User`, `Expense`), functions, file handling
(`ifstream` / `ofstream`), strings, input validation, linear search, and bubble sort
(sorting an index array so the stored data stays in entry order).

## Project Structure
```
Smart-Digital-Expense-Tracker/
├── src/
│   └── expense_tracker.cpp
├── docs/
│   └── Expense_Tracker_Presentation.pptx
├── README.md
├── .gitignore
├── CONTRIBUTING.md
└── LICENSE            (added once the licensing choice is confirmed)
```

## Installation
1. Install a C++ compiler:
   - **Windows:** [MinGW-w64](https://www.mingw-w64.org/) or Visual Studio
   - **macOS:** `xcode-select --install`
   - **Linux (Debian/Ubuntu):** `sudo apt install g++`
2. Clone the repository:
   ```bash
   git clone https://github.com/Ankush6396/Smart-Digital-Expense-Tracker.git
   cd Smart-Digital-Expense-Tracker
   ```
   (The repository is currently private, so you must be signed in to GitHub with access.)

## Compilation
```bash
g++ -std=c++11 -Wall -Wextra -o expense_tracker src/expense_tracker.cpp
```
The code compiles with no warnings under `-Wall -Wextra -pedantic`.

## Running the Application
```bash
./expense_tracker          # Linux / macOS
expense_tracker.exe        # Windows
```
Run it from the folder where you want your data files (`users.txt`,
`expenses_<username>.txt`) to be created.

## Sample Terminal Output
Real output from a test session (budget Rs 5000, two expenses added):

```text
===== SMART DIGITAL EXPENSE TRACKER =====
1. Register
2. Login
3. Exit

--- BUDGET STATUS (11-2026) ---
Monthly Budget : Rs 5000.00
Spent So Far   : Rs 1450.00 (29.00%)
You are within budget. Remaining: Rs 3550.00

--- EXPENSE HISTORY ---
ID   Date         Category       Description               Amount
-----------------------------------------------------------------
1    01-11-2026   Food           Lunch                     250.00
2    01-11-2026   Travel         Bus pass                 1200.00
-----------------------------------------------------------------
Total expenses: Rs 1450.00

--- CATEGORY-WISE EXPENSE ---
Category          Total (Rs)     Share
--------------------------------------
Food                  250.00    17.24%
Travel               1200.00    82.76%
Shopping                0.00     0.00%
Education               0.00     0.00%
Entertainment           0.00     0.00%
Other                   0.00     0.00%
--------------------------------------
TOTAL                1450.00
```

## Data Storage
| File | Format |
|------|--------|
| `users.txt` | `username password budget` (one user per line) |
| `expenses_<username>.txt` | `id\|date\|category\|description\|amount` (one expense per line) |

These files are created at runtime and are excluded by `.gitignore`.

## Security Notice
**Passwords are stored in plain text** in `users.txt` and compared directly at login.
This is acceptable only for a classroom exercise and **must not be used for real
accounts**. Do not reuse a real password, and never commit `users.txt` to a repository.
A production version should store salted hashes (for example bcrypt or Argon2).

## Known Limitations
Found during code review (see the project's final review notes):
- Plain-text password storage (above).
- If standard input is closed (Ctrl+D, or piped input that ends early), the input
  loops repeat forever instead of exiting.
- A damaged line in `expenses_<user>.txt` can crash the program on load
  (`stoi`/`stod` exceptions are not caught).
- Usernames are used in file names without sanitizing; only spaces are rejected.
- The prediction is a simple linear projection, so it can look inflated early in the
  month.
- Fixed capacity: 50 users and 1000 expenses per user.

## Future Enhancements
- Password hashing and safer input/file handling (fixes for the limitations above)
- AI/trend-based spending prediction
- Mobile app version
- QR bill scanner
- Cloud backup
- Charts and CSV export

## Academic Information
| | |
|---|---|
| **Project** | Smart Digital Expense Tracker with Budget Prediction |
| **Assignment** | Lab Sheet-09 |
| **Program** | BCA, I Semester (Session 2026-2027) |
| **Institution** | COER University, Roorkee |

The accompanying presentation is in [`docs/Expense_Tracker_Presentation.pptx`](docs/Expense_Tracker_Presentation.pptx).

## Author
**kartik saini** - GitHub: [@kartiksaini95108](https://github.com/kartiksaini95108)

## License
License to be added after the author confirms the intended choice. Until then, all
rights are reserved by the author.

## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md).
"# Smart-Digital-Expense-Tracker-with-Budget-Prediction" 
