#include "data-types-operations.hpp"
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

Book makeBook(const std::string &isbn, const std::string &title, const std::string &authors,
              unsigned int pages, std::chrono::year_month_day startDate)
{
    return Book{isbn, title, authors, pages, 0, startDate, {}};
}

void addBook(BookCollection &collection, const Book &book)
{
    collection.push_back(book);
}

bool removeBook(BookCollection &collection, const std::string &isbn)
{
    auto it = std::find_if(collection.begin(), collection.end(), [&isbn](const Book &book)
                           { return book.isbn == isbn; });

    if (it != collection.end())
    {
        collection.erase(it);
        return true;
    }

    return false;
}

Book *findBook(BookCollection &collection, const std::string &isbn)
{
    auto it = std::find_if(collection.begin(), collection.end(), [&isbn](const Book &book)
                           { return book.isbn == isbn; });

    if (it != collection.end())
    {
        return &(*it);
    }

    return nullptr;
}

bool addPagesRead(BookCollection &collection, const std::string &isbn, unsigned int pages)
{
    Book *book = findBook(collection, isbn);

    if (book == nullptr)
    {
        return false;
    }

    book->pagesRead += pages;

    if (book->pagesRead > book->pages)
    {
        book->pagesRead = book->pages;
    }

    return true;
}