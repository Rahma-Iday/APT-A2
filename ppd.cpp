#include "Commands.h"
#include <map>


bool readStockData(string fileName, char delim);
bool readCoinData(string fileName, char delim);
vector<Stock> loadStockData(string fileName, char delim);
vector<Coin> loadCoinData(string fileName, char delim);
void displayMenu();
void handleInput(LinkedList &list, string stockFilePath, string coinFilePath, vector<Coin> &coins, bool &colour);
std::string findFilePath(const std::string& fileName, const std::string& currentDir = ".");

/**
 * manages the running of the program, initialises data structures, loads
 * data, display the main menu, and handles the processing of options.
 * Make sure free memory and close all files before exiting the program.
 **/
int main(int argc, char **argv)
{
    /* validate command line arguments */
    // TODO
    if (argc == 3)
    {
        string stockFile(argv[1]);
        std::string coinFile(argv[2]);

        string stockFilePath = findFilePath(stockFile);
        string coinFilePath = findFilePath(coinFile);

        /*loading  data*/
        // check if both files exists, only then read data
        if ( !stockFilePath.empty() && !coinFilePath.empty())
        {
            // check if both Files have valid data by reading, then only loads data
            if (readStockData(stockFilePath, STOCK_DELIM) && readCoinData(coinFilePath, DELIM[0]))
            {
                vector<Stock> stock = loadStockData(stockFilePath, STOCK_DELIM);
                vector<Coin> coins = loadCoinData(coinFilePath, DELIM[0]);

                bool colour = false;

                std::cout<< "Would you like to enable colour? press 'y' for yes (if anything else is entered it is disabled): " << std::endl;
                if(readInput() == "y")
                {
                    colour = true;
                    std::cout<< "Colour Enabled" << std::endl;
                    std::cout<<std::endl;
                }


                std::cout<< "Would you like to enable double linked list? press 'y' for yes(if anything else is entered it is disabled): ";
                if(readInput() == "y")
                {
                    std::cout << "Double Linked List Enabled" << std::endl;
                    std::cout << std::endl;


                    DoubleLinkedList list;
                    // put stock vector's stocks into linked list
                    for (int i = 0; i < static_cast<int>(stock.size()); i++)
                    {
                        list.add(stock[i]);
                    }
                    /*load coin into array data type?*/

                    handleInput(list, stockFilePath, coinFilePath, coins, colour);
                }else{
                    std::cout<< "Single Linked List Enabled" << std::endl;
                    std::cout << std::endl;


                    LinkedList list;
                    // put stock vector's stocks into linked list
                    for (int i = 0; i < static_cast<int>(stock.size()); i++)
                    {
                        list.add(stock[i]);
                    }
                    /*load coin into array data type?*/

                    handleInput(list, stockFilePath, coinFilePath, coins, colour);
                }
            }
        } 
        else 
        {
            std::cout << "Either the Coin File or Stock File provided does not exist! Retry with Different Files" << std::endl;
        }
    }
    else
    {
        std::cout << "Please make sure exactly 3 command line arguments are entered in the form:\n ./ppd <stockfile> <coinsfile>" << std::endl;
    }
    return EXIT_SUCCESS;
}

void handleInput(LinkedList &list, string stockFilePath, string coinFilePath, vector<Coin> &coins, bool &colour)
{

    // create a loop that executes the return to main menu functionality until exit options (3 or 9) are pressed
    // set a bool value to ensure main menu is displayed until valid input given

    bool exitProgram = false;
    std::map<int, std::unique_ptr<Command>> menuOptions;
    menuOptions[1] = std::make_unique<DisplayItemsCommand>(list);
    menuOptions[2] = std::make_unique<PurchaseItemCommand>(coins, list, colour);
    menuOptions[3] = std::make_unique<SaveAndExitCommand>(list, coins, stockFilePath, coinFilePath);
    menuOptions[4] = std::make_unique<AddItemCommand>(list);
    menuOptions[5] = std::make_unique<RemoveItemCommand>(list);
    menuOptions[6] = std::make_unique<DisplayCoinsCommand>(coins);
    menuOptions[7] = std::make_unique<ResetStockCommand>(list);
    menuOptions[8] = std::make_unique<ResetCoinsCommand>(coins);
    menuOptions[9] = std::make_unique<AbortCommand>();

    while (!exitProgram)
    {
        displayMenu();
        std::cout << "Please enter your choice: ";
        string input = readInput();

        if (std::cin.eof())
        {
            std::cout << "\nCtrl-D was pressed, terminating program." << std::endl;
            exitProgram = true;
        }else{
            try{
                int choice = std::stoi(input);
                auto it = menuOptions.find(std::stoi(input));
                if (it != menuOptions.end()) {
                    it->second->execute();
                    if (choice == 3 || choice == 9)
                    {
                        exitProgram = true;
                    }
                }
                else
                {
                    printInvalidInput();
                }
            }
            catch (std::invalid_argument &e)
            {
                printInvalidInput();
            }
        }
    }
}

void displayMenu()
{
    std::cout << "Main Menu:\n"
              << "    1. Display Items\n"
              << "    2. Purchase Items\n"
              << "    3. Save and Exit\n"
              << "Administrator-Only Menu:\n"
              << "    4. Add Item\n"
              << "    5. Remove Item\n"
              << "    6. Display Coins\n"
              << "    7. Reset Stock\n"
              << "    8. Reset Coins\n"
              << "    9. Abort Program\n"
              << "Select your option (1-9): " << std::endl;
}

bool readStockData(string fileName, char delim)
{
    bool validData = true;

    std::ifstream file(fileName);
    std::string line;

    std::set<string> idSet;

    while (std::getline(file, line))
    {
        std::stringstream linestream(line);
        std::string id, name, description, dollarsString, centsString, on_handString;

        unsigned int cents;

        // checks all feilds are there, if all there then checks if string feilds are actually unsigned ints
        if (
            std::getline(linestream, id, delim) &&
            std::getline(linestream, name, delim) &&
            std::getline(linestream, description, delim) &&
            std::getline(linestream, dollarsString, PRICE_DELIM) &&
            std::getline(linestream, centsString, delim) &&
            std::getline(linestream, on_handString))
        {
            // checks that all strings for dollars, cents and stock on hand strings are all unsigned ints
            try
            {
                std::stoul(dollarsString);
                std::stoul(on_handString);
                cents = std::stoul(centsString);
                // additonally also checks that cents is divisible by 5 EVENLY
                if (cents % 5 != 0)
                {
                    std::cout << "Invalid Price Data in Stock File: Cents Must be Valid" << std::endl;
                    validData = false;
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Invalid Data in Stock File" << std::endl;
                validData = false;
            }
            catch (const std::out_of_range &e)
            {
                std::cout << "Invalid Data in Stock File" << std::endl;
                validData = false;
            }

            // checks all ids are unique:
            if (idSet.insert(id).second == false)
            {
                std::cout << "Invalid Data in Stock File, Multiple Entries with the Same ID" << std::endl;
                validData = false;
            }
            // check length of cent String as it must not be >2 or <2
            if (centsString.size() != CENTLEN)
            {
                std::cout << "Invalid Price Data in Stock File: Cents Must be specified to Exactly 2 Decimal Places" << std::endl;
                validData = false;
            }
            // checks that ID len , name len and Description len are the correct size:
            if (static_cast<int>(id.size()) != IDLEN || static_cast<int>(name.length()) > NAMELEN || static_cast<int>(description.length()) > DESCLEN)
            {
                std::cout << "Invalid Price Data in Stock File: Check ID, name and description feilds are the correct size" << std::endl;
                validData = false;
            }
        }
        else
        {
            std::cout << "Invalid Data in Stock File" << std::endl;
            validData = false;
        }
    }

    return validData;
}

bool readCoinData(string fileName, char delim)
{
    bool validData = true;

    std::ifstream file(fileName);
    std::string line;
    // creating a set as a set cannot input duplicate values
    std::set<int> denomSet;

    while (std::getline(file, line))
    {
        std::stringstream linestream(line);
        std::string denominationString, quantityString;
        unsigned int denomination;

        // checks all feilds are there, if all there then checks if string feilds are actually unsigned ints
        if (
            std::getline(linestream, denominationString, delim) &&
            std::getline(linestream, quantityString))
        {
            // checks that strings are all unsigned ints
            try
            {
                denomination = std::stoul(denominationString);
                std::stoul(quantityString);

                // checks if duplicate value is being inserted
                if (denomSet.insert(denomination).second == false)
                {
                    std::cout << "Invalid Data in Coin File" << std::endl;
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Invalid Data in Coin File" << std::endl;
                validData = false;
            }
            catch (const std::out_of_range &e)
            {
                std::cout << "Invalid Data in Coin File" << std::endl;
                validData = false;
            }
        }
        else
        {
            // invalid number of feilds
            std::cout << "Invalid Data in Coin File" << std::endl;
            validData = false;
        }
    }
    // checks that the produced set is equal to the expected set
    // (the operator '==' also checks length of set = 8)
    // TODO: maybe find a better way to do this that doesnt involve using hard-coded numbers?
    std::set<int> expectedSet = {5, 10, 20, 50, 100, 200, 500, 1000};
    if (denomSet != expectedSet)
    {
        std::cout << "Invalid Data in Coin File" << std::endl;
        validData = false;
    }

    return validData;
}

vector<Stock> loadStockData(string fileName, char delim)
{
    vector<Stock> stocks;
    std::ifstream file(fileName);
    std::string line;

    std::string dollarsString, centsString, on_handString;
    unsigned int dollars;
    unsigned int cents;

    while (std::getline(file, line))
    {
        std::stringstream linestream(line);
        Stock stock;

        std::getline(linestream, stock.id, delim);
        std::getline(linestream, stock.name, delim);
        std::getline(linestream, stock.description, delim);
        std::getline(linestream, dollarsString, PRICE_DELIM);
        std::getline(linestream, centsString, delim);
        std::getline(linestream, on_handString);

        // process data
        dollars = std::stoul(dollarsString);
        cents = std::stoul(centsString);
        stock.on_hand = std::stoul(on_handString);

        stock.price.dollars = dollars;
        stock.price.cents = cents;

        stocks.push_back(stock);
    }

    // sorts vector alphabetically by name, using lambda function
    std::sort(stocks.begin(), stocks.end(), [](const Stock &a, const Stock &b)
              {
        //compares the strings lexicographically aka alphabetically
        return a.name < b.name; });

    return stocks;
}

vector<Coin> loadCoinData(string fileName, char delim)
{
    vector<Coin> coins;
    std::ifstream file(fileName);
    std::string line;
    std::vector<int> expectedValues = {5, 10, 20, 50, 100, 200, 500, 1000};
    int denomIndex = 0;

    while (std::getline(file, line))
    {
        std::stringstream linestream(line);
        std::string denomString, countString;
        int denom = 0;

        Coin coin;

        std::getline(linestream, denomString, delim);
        std::getline(linestream, countString);

        denom = std::stoul(denomString);
        for (int i = 0; i < static_cast<int>(expectedValues.size()); i++)
        {
            if (expectedValues[i] == denom)
            {
                denomIndex = i;
            }
        }

        coin.denom = static_cast<Denomination>(denomIndex);
        coin.count = std::stoul(countString);

        coins.push_back(coin);
    }

    // sorts coins vector by denom using lambda function before returning it
    std::sort(coins.begin(), coins.end(), [](const Coin &coin1, const Coin &coin2)
              { return coin1.denom < coin2.denom; });

    return coins;
}


/**
 *  This function was adapted and modified to fit our needs and to fit c++14 standards
 * from https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1046380353&id=1044780608
 * 
**/
// This function searches for a file with the given file name in the current directory
// and all its subdirectories. It returns the full path of the first occurrence of the
// file, or an empty string if the file was not found.
std::string findFilePath(const std::string& fileName, const std::string& currentDir)
{
    std::string result;  // This will hold the result (aka the full path of the file)

    // Opens the current directory using the opendir function 
    // (for the first run of this function the current directory will be the directory the ppd exectuable is in.) 
    // This returns a pointer to a DIR struct, which represents the directory stream.
    std::unique_ptr<DIR, std::function<int(DIR*)>> dir(opendir(currentDir.c_str()), closedir);

    // Checks if the directory was successfully opened
    if (dir == nullptr)
    {
        std::cerr << "Failed to open the current directory for some reason: " << currentDir << '\n';
    }
    else
    {
        // Loop over all entries in the current directory
        struct dirent* entry;
        while ((entry = readdir(dir.get())) != nullptr && result.empty())
        {
            // If the entry is a directory and not "."(a current directory) or ".."(a parent directory), recursively search it
            if (entry->d_type == DT_DIR)
            {
                // checks all subdirectories recursively
                if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..")
                {
                    std::string subdir = currentDir + "/" + entry->d_name;
                    result = findFilePath(fileName, subdir);
                }
            }
            // If the entry is a regular file and its name matches the given file name,
            // set the result to the full path of the file
            else if (entry->d_type == DT_REG && std::string(entry->d_name) == fileName)
            {
                result = currentDir + "/" + fileName;
            }
        }
    }

    // Return the result (which may be an empty string)
    return result;
}