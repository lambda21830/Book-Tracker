#ifndef DATA_TYPES_HPP
#define DATA_TYPES_HPP

#include <string>
#include <vector>
#include <chrono>

struct Book
{
    std::string isbn;
    std::string title;
    std::vector<std::string> authors;
    unsigned int pages;
    unsigned int pagesRead;
    std::chrono::year_month_day startDate;
    std::chrono::year_month_day endDate;
};

using BookCollection = std::vector<Book>;

#endif