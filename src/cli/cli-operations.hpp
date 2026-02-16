#ifndef CLI_OPERATIONS_HPP
#define CLI_OPERATIONS_HPP

#define NC "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

#include "../core/data-types/data-types.hpp"
#include <string>

std::string getOS();

void clearScreen();

void pressAKey();

void displayTitle();

void displayMenu();

void displayGoodbye();

int getMenuChoice();

void handleAddBook(BookCollection &collection);

void handleRemoveBook(BookCollection &collection);

void handlePrintBookDetails(BookCollection &collection);

void handleListAllBooks(const BookCollection &collection);

void handleStoreCollection(const BookCollection &collection, const std::string &filename);

bool handleExit(BookCollection &collection, const std::string &filename);

void runMenuLoop(BookCollection &collection, const std::string &filename);

#endif
