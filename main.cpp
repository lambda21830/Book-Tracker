#include "src/core/data-types/data-types.hpp"
#include "src/core/disk/disk-operations.hpp"
#include "src/cli/cli-operations.hpp"

const std::string DATA_FILE = "books.csv";

int main()
{
    BookCollection collection = loadCollection(DATA_FILE);

    runMenuLoop(collection, DATA_FILE);
    displayGoodbye();

    return 0;
}
