# Contributing

Thanks for your interest in the Smart Digital Expense Tracker! This is an academic
project, but suggestions and improvements are welcome.

## Reporting issues
Open an issue with:
- What you did (menu options and inputs)
- What you expected vs. what happened
- Your compiler and OS (for example `g++ 13`, Ubuntu 24.04)

## Proposing changes
1. Fork the repository and create a branch: `git checkout -b fix/short-description`
2. Keep the existing 12 modules working; explain any behaviour change in your PR.
3. Build with warnings enabled and make sure there are none:
   ```bash
   g++ -std=c++11 -Wall -Wextra -pedantic -o expense_tracker src/expense_tracker.cpp
   ```
4. Test manually: register, login, add/search/delete an expense, view reports.
5. Never commit `users.txt`, `expenses_*.txt`, or any real credentials
   (they are listed in `.gitignore`).

## Commit messages
Use short, imperative messages with a type prefix, e.g.
`fix: handle end-of-input in readInt`, `docs: update README`, `feat: hash passwords`.

## Good first contributions
See "Known limitations" and "Future enhancements" in the README.
