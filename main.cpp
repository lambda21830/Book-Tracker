#include "src/core/data-types/data-types.hpp"
#include "src/core/disk/disk-operations.hpp"
#include "src/cli/cli-operations.hpp"
#include <filesystem>
#include <iostream>

int main()
{
    std::string folder = "data";
    std::string filename = folder + "/books.csv";

    try
    {
        if (!std::filesystem::exists(folder))
        {
            std::filesystem::create_directory(folder);
        }
    }

    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << RED << "Error: " << NC << "Can't create folder: " << e.what() << std::endl;
    }

    BookCollection collection = loadCollection(filename);

    runMenuLoop(collection, filename);
    displayGoodbye();

    return 0;
}
