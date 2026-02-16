#include "disk-operations.hpp"
#include <fstream>
#include <sstream>

std::string dateToString(std::chrono::year_month_day date)
{
    std::ostringstream oss;
    oss << static_cast<int>(date.year()) << "-"
        << static_cast<unsigned>(date.month()) << "-"
        << static_cast<unsigned>(date.day());

    return oss.str();
}

std::chrono::year_month_day stringToDate(const std::string &str)
{
    if (str.empty() || str == "0-0-0")
    {
        return std::chrono::year_month_day{};
    }

    int year, month, day;
    char dash1, dash2;
    std::istringstream iss(str);

    iss >> year >> dash1 >> month >> dash2 >> day;

    return std::chrono::year_month_day{
        std::chrono::year{year},
        std::chrono::month{static_cast<unsigned>(month)},
        std::chrono::day{static_cast<unsigned>(day)}};
}

static std::string escapeCSV(const std::string &field)
{
    if (field.find(',') != std::string::npos ||
        field.find('"') != std::string::npos ||
        field.find('\n') != std::string::npos)
    {
        std::string escaped = "\"";

        for (char c : field)
        {
            if (c == '"')
            {
                escaped += "\"\"";
            }

            else
            {
                escaped += c;
            }
        }

        escaped += "\"";

        return escaped;
    }

    return field;
}

static std::string parseCSVField(std::istringstream &stream)
{
    std::string field;
    char c;

    if (stream.peek() == '"')
    {
        stream.get();

        while (stream.get(c))
        {
            if (c == '"')
            {
                if (stream.peek() == '"')
                {
                    stream.get();
                    field += '"';
                }

                else
                {
                    break;
                }
            }

            else
            {
                field += c;
            }
        }

        if (stream.peek() == ',')
        {
            stream.get();
        }
    }

    else
    {
        std::getline(stream, field, ',');
    }

    return field;
}

bool saveCollection(const BookCollection &collection, const std::string &filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    file << "isbn,title,authors,pages,pagesRead,startDate,endDate\n";

    for (const Book &book : collection)
    {
        file << escapeCSV(book.isbn) << ","
             << escapeCSV(book.title) << ","
             << escapeCSV(book.authors) << ","
             << book.pages << ","
             << book.pagesRead << ","
             << dateToString(book.startDate) << ","
             << dateToString(book.endDate) << "\n";
    }

    file.close();

    return true;
}

BookCollection loadCollection(const std::string &filename)
{
    BookCollection collection;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return collection;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream stream(line);

        std::string isbn = parseCSVField(stream);
        std::string title = parseCSVField(stream);
        std::string authors = parseCSVField(stream);
        std::string pagesStr = parseCSVField(stream);
        std::string pagesReadStr = parseCSVField(stream);
        std::string startDateStr = parseCSVField(stream);
        std::string endDateStr = parseCSVField(stream);

        Book book{
            isbn,
            title,
            authors,
            std::stoul(pagesStr),
            std::stoul(pagesReadStr),
            stringToDate(startDateStr),
            stringToDate(endDateStr)};

        collection.push_back(book);
    }

    file.close();

    return collection;
}
