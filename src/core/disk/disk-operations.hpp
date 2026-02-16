#ifndef DISK_OPERATIONS_HPP
#define DISK_OPERATIONS_HPP

#include "../data-types/data-types.hpp"
#include <string>
#include <chrono>

std::string dateToString(std::chrono::year_month_day date);

std::chrono::year_month_day stringToDate(const std::string &str);

bool saveCollection(const BookCollection &collection, const std::string &filename);

BookCollection loadCollection(const std::string &filename);

#endif
