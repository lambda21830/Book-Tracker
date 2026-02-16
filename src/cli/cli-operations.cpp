#include "cli-operations.hpp"
#include "../core/data-types/data-types-operations.hpp"
#include "../core/disk/disk-operations.hpp"
#include <iostream>
#include <limits>
#include <iomanip>
#include <cctype>

std::string getOS()
{
#ifdef _WIN32
    return "Windows";
#elif __APPLE__ || __MACH__
    return "Apple";
#elif __linux__
    return "Linux";
#else
    return "Other";
#endif
}

void clearScreen()
{
    getOS() == "Windows" ? system("cls") : system("clear");
}

void pressAKey()
{
    if (getOS() == "Windows")
    {
        system("pause");
        clearScreen();
    }

    else
    {
        std::cout << "Press a key to continue..." << std::flush;
        system("read -sn 1");
        clearScreen();
    }
}

void displayTitle()
{
    std::cout << "================================\n";
    std::cout << "          BOOK TRACKER          \n";
    std::cout << "================================\n";
    std::cout << "Track your reading progress!\n\n";
}

void displayMenu()
{
    displayTitle();

    std::cout << "1. Add a book\n";
    std::cout << "2. Remove a book\n";
    std::cout << "3. Print book details\n";
    std::cout << "4. List all books\n";
    std::cout << "5. Save collection to disk\n";
    std::cout << "6. Exit\n\n";
    std::cout << "Enter your choice: ";
}

void displayGoodbye()
{
    std::cout << "\n========================================\n";
    std::cout << "    Thank you for using Book Tracker!    \n";
    std::cout << "             Happy reading!              \n";
    std::cout << "========================================\n";
}

int getMenuChoice()
{
    int choice;

    while (true)
    {
        displayMenu();

        if (std::cin >> choice)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice >= 1 && choice <= 6)
            {
                clearScreen();
                return choice;
            }
        }

        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cerr << RED << "Error: " << NC << "Please enter a number between 1 and 6\n";
        pressAKey();
    }
}

static std::chrono::year_month_day readDate(const std::string &prompt)
{
    std::cout << prompt << " (YYYY-MM-DD, or 0 for none): ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "0" || input.empty())
    {
        return std::chrono::year_month_day{};
    }

    return stringToDate(input);
}

void handleAddBook(BookCollection &collection)
{
    std::cout << "\n--- ADD A BOOK ---\n";

    std::string isbn, title, authors;
    unsigned int pages;

    std::cout << "ISBN: ";
    std::getline(std::cin, isbn);

    std::cout << "Title: ";
    std::getline(std::cin, title);

    std::cout << "Authors: ";
    std::getline(std::cin, authors);

    std::cout << "Total pages: ";
    std::cin >> pages;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto startDate = readDate("Start date");

    Book book = makeBook(isbn, title, authors, pages, startDate);
    addBook(collection, book);

    std::cout << GREEN << "Book added successfully!\n"
              << NC;
}

void handleRemoveBook(BookCollection &collection)
{
    std::cout << "\n--- REMOVE A BOOK ---\n";

    if (collection.empty())
    {
        std::cerr << RED << "Error: " << NC << "No books in collection.\n";
    }

    else
    {
        std::cout << "Enter ISBN of book to remove: ";

        std::string isbn;
        std::getline(std::cin, isbn);

        if (removeBook(collection, isbn))
        {
            std::cout << GREEN << "Book removed successfully!\n"
                      << NC;
        }

        else
        {
            std::cerr << RED << "Error: " << NC << "Book not found with ISBN: " << isbn << "\n";
        }
    }
}

void handlePrintBookDetails(BookCollection &collection)
{
    std::cout << "\n--- BOOK DETAILS ---\n";

    if (collection.empty())
    {
        std::cerr << RED << "Error: " << NC << "No books in collection.\n";
    }

    else
    {
        std::cout << "Enter ISBN: ";

        std::string isbn;
        std::getline(std::cin, isbn);

        Book *book = findBook(collection, isbn);

        if (book == nullptr)
        {
            std::cerr << RED << "Error: " << NC << "Book not found with ISBN: " << isbn << "\n";
        }

        else
        {
            double progress = book->pages > 0 ? (100.0 * book->pagesRead / book->pages) : 0.0;

            std::cout << "\nTitle:      " << book->title << "\n";
            std::cout << "Authors:    " << book->authors << "\n";
            std::cout << "ISBN:       " << book->isbn << "\n";
            std::cout << "Pages:      " << book->pagesRead << " / " << book->pages
                      << " (" << std::fixed << std::setprecision(1) << progress << "%)\n";
            std::cout << "Start date: " << dateToString(book->startDate) << "\n";
            std::cout << "End date:   " << dateToString(book->endDate) << "\n";
        }
    }
}

void handleListAllBooks(const BookCollection &collection)
{
    std::cout << "\n--- ALL BOOKS ---\n";

    if (collection.empty())
    {
        std::cerr << RED << "Error: " << NC << "No books in collection.\n";
    }

    else
    {
        std::cout << "Total: " << collection.size() << " book(s)\n\n";

        for (const Book &book : collection)
        {
            double progress = book.pages > 0 ? (100.0 * book.pagesRead / book.pages) : 0.0;

            std::cout << "  [" << book.isbn << "] " << book.title << " by " << book.authors
                      << " - " << std::fixed << std::setprecision(1) << progress << "% complete\n";
        }
    }
}

void handleStoreCollection(const BookCollection &collection, const std::string &filename)
{
    if (saveCollection(collection, filename))
    {
        std::cout << GREEN << "Collection saved to " << filename << "\n"
                  << NC;
    }

    else
    {
        std::cerr << RED << "Error: " << NC << "Could not save collection to " << filename << "\n";
    }
}

bool handleExit(BookCollection &collection, const std::string &filename)
{
    char choice;

    std::cout << "\nSave collection before exiting? (y/n): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (tolower(choice) == 'y')
    {
        handleStoreCollection(collection, filename);
    }

    return true;
}

void runMenuLoop(BookCollection &collection, const std::string &filename)
{
    bool running = true;

    while (running)
    {
        int choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            handleAddBook(collection);
            pressAKey();
            break;
        case 2:
            handleRemoveBook(collection);
            pressAKey();
            break;
        case 3:
            handlePrintBookDetails(collection);
            pressAKey();
            break;
        case 4:
            handleListAllBooks(collection);
            pressAKey();
            break;
        case 5:
            handleStoreCollection(collection, filename);
            pressAKey();
            break;
        case 6:
            running = !handleExit(collection, filename);
            break;
        }
    }
}
