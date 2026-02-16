#ifndef DATA_TYPES_OPERATIONS_HPP
#define DATA_TYPES_OPERATIONS_HPP

#include "data-types.hpp"
#include <string>
#include <vector>
#include <chrono>

Book makeBook(const std::string &isbn, const std::string &title, const std::string &authors,
              unsigned int pages, std::chrono::year_month_day startDate);

void addBook(BookCollection &collection, const Book &book);

bool removeBook(BookCollection &collection, const std::string &isbn);

Book *findBook(BookCollection &collection, const std::string &isbn);

bool addPagesRead(BookCollection &collection, const std::string &isbn, unsigned int pages);

#endif