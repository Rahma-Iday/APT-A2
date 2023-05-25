#include <iostream>
#include "LinkedList.h"
#include "DoublyLinkedList.h"
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "Coin.h"
#include <dirent.h>
#include <memory>
#include <functional>
using std::string;
using std::vector;

bool readStockData(string fileName, char delim);
bool readCoinData(string fileName, char delim);
vector<Stock> loadStockData(string fileName, char delim);
vector<Coin> loadCoinData(string fileName, char delim);
void displayMenu();
string readInput();
bool isNumber(string s);
void printInvalidInput();
void getNewItem(LinkedList &list);
bool getPrice(unsigned int &x, unsigned int &y);
void printDebug();
void handleInput(LinkedList &list, string stockFilePath, string coinFilePath, vector<Coin> &coins);
void menu(LinkedList &list, bool &exitProgram, int &optionNo, string stockFilePath, string coinFilePath, vector<Coin> &coins);
void removeItem(LinkedList &list);
void saveAndExit(LinkedList &list, std::vector<Coin> &coins, string stockFilePath, string coinFilePath);
void displayCoins(std::vector<Coin> &coins);
void resetCoins(std::vector<Coin> &coins);
void makePurchase(vector<Coin> &coinVect, LinkedList &list);
bool enoughChange(double changeRequired, vector<Coin> &coins, vector<Coin> &userCoins);
void processMoney(double changeRequired, vector<Coin> &coins, vector<Coin> &userCoins);
void printAllCoins(vector<Coin> &coins);
bool readListStructure();
std::string findFilePath(const std::string& fileName, const std::string& currentDir = ".");


/*Command Design Pattern Enhancement*/
class Command {
public:
  virtual ~Command() {}
  virtual void execute() = 0;
};

// Concrete command class for displaying items 
class DisplayItemsCommand : public Command {
private:
  LinkedList& list;

public:
  DisplayItemsCommand(LinkedList& list) : list(list) {}

  void execute() override {
    list.print();
  }
};

// Concrete command class for purchasing Item 
class PurchaseItemCommand : public Command {
private:
  vector<Coin>& coins;
  LinkedList& list;

public:
  PurchaseItemCommand(vector<Coin>& coins, LinkedList& list): coins(coins), list(list) {}

  void execute() override {
    makePurchase(coins, list);
  }
};


// Concrete command class for saving and exiting
class SaveAndExitCommand : public Command {
private:
  LinkedList& list;
  std::string stockFilePath;
  std::string coinFilePath;
  std::vector<Coin>& coins;
  bool& exitProgram;

public:
  SaveAndExitCommand(LinkedList& list, std::string stockFilePath, std::string coinFilePath, std::vector<Coin>& coins, bool& exitProgram)
      : list(list), stockFilePath(stockFilePath), coinFilePath(coinFilePath), coins(coins), exitProgram(exitProgram) {}

  void execute() override {
    saveAndExit(list, coins, stockFilePath, coinFilePath);
    exitProgram = true;
  }
};

// Concrete command class for adding an item
class AddItemCommand : public Command {
private:
  LinkedList& list;

public:
  AddItemCommand(LinkedList& list) : list(list) {}

  void execute() override {
    getNewItem(list);
  }
};

// Concrete command class for removing an item
class RemoveItemCommand : public Command {
private:
  LinkedList& list;

public:
  RemoveItemCommand(LinkedList& list) : list(list) {}

  void execute() override {
    removeItem(list);
  }
};

// Concrete command class for displaying coins
class DisplayCoinsCommand : public Command {
private:
  std::vector<Coin>& coins;

public:
  DisplayCoinsCommand(std::vector<Coin>& coins) : coins(coins) {}

  void execute() override {
    displayCoins(coins);
  }
};

// Concrete command class for resetting stock
class ResetStockCommand : public Command {
private:
  LinkedList& list;

public:
  ResetStockCommand(LinkedList& list) : list(list) {}

  void execute() override {
    list.resetStock();
    std::cout << "\"All stock has been reset to " << DEFAULT_STOCK_LEVEL << "\"" << std::endl;
  }
};

// Concrete command class for resetting coins
class ResetCoinsCommand : public Command {
private:
  std::vector<Coin>& coins;

public:
  ResetCoinsCommand(std::vector<Coin>& coins) : coins(coins) {}

  void execute() override {
    resetCoins(coins);
  }
};

// Concrete command class for aborting the program (option 9)
class AbortProgramCommand : public Command {
private:
  bool& exitProgram;

public:
  AbortProgramCommand(bool& exitProgram) : exitProgram(exitProgram) {}

  void execute() override {
    exitProgram = true;
  }
};


// Implement the other concrete command classes for the remaining options.

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

        /*returns filePath from given file Names (to account for different directories)*/
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

                // asks user what type of structure they would like to use
                bool listType = readListStructure();
                if (listType)
                {
                    std::cout << "Using Doubly Linked List for Stock" << std::endl;
                    DoublyLinkedList list;
                    for (int i = 0; i < static_cast<int>(stock.size()); i++)
                    {
                        list.add(stock[i]);
                    }

                    handleInput(list, stockFilePath, coinFilePath, coins);
                }
                else 
                {
                    // put stock vector's stocks into linked list
                    std::cout << "Using Linked List for Stock" << std::endl;
                    LinkedList list;
                    for (int i = 0; i < static_cast<int>(stock.size()); i++)
                    {
                        list.add(stock[i]);
                    }

                    handleInput(list, stockFilePath, coinFilePath, coins);

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

/* asks for user input after displaying main menu*/
void handleInput(LinkedList &list, string stockFilePath, string coinFilePath, vector<Coin> &coins)
{

    // create a loop that executes the return to main menu functionality until exit options (3 or 9) are pressed
    // set a bool value to ensure main menu is displayed until valid input given

    bool exitProgram = false;

    while (!exitProgram)
    {
        bool validOption = false;
        int optionNo = 0;
        while (!validOption)
        {
            displayMenu();
            std::cout << "Please enter your choice: ";
            string input = readInput();

            if (std::cin.eof())
            {
                std::cout << "\nCtrl-D was pressed, terminating program." << std::endl;
                exitProgram = true;
                validOption = true;
            }
            else if (isNumber(input))
            {
                optionNo = std::stoi(input);
                if (optionNo > 0 && optionNo < 10)
                {
                    validOption = true;
                }
                else
                {
                    std::cout << "The number you entered is not within the options 1-9" << std::endl;
                }
            }
            else if (input == "help")
            {
                std::cout << "Help: You should enter a number between 1-9 to select an option" << std::endl;

            }
            else
            {
                std::cout << "Please enter an number between 1-9 to select an option" << std::endl;
            }
        }

        std::cout << std::endl;
        /* after a valid option 1-9 is gotten from user a command is created and executed based on this */
        menu(list, exitProgram, optionNo, stockFilePath, coinFilePath, coins);
        std::cout << std::endl;
    }
}

/*this function creates and executes commands of the main menu based on the command design pattern*/
void menu(LinkedList &list, bool &exitProgram, int &optionNo, string stockFilePath, string coinFilePath, vector<Coin> &coins)
{
    /*create command based on optionNo*/
    Command* command = nullptr;

    if (optionNo == 1) 
    {
        command = new DisplayItemsCommand(list);
    } 
    else if (optionNo == 2) 
    {
        command = new PurchaseItemCommand(coins, list);
    } 
    else if (optionNo == 3) 
    {
      command = new SaveAndExitCommand(list, stockFilePath, coinFilePath, coins, exitProgram);
    } 
    else if (optionNo == 4) 
    {
      command = new AddItemCommand(list);
    } 
    else if (optionNo == 5) 
    {
      command = new RemoveItemCommand(list);
    } 
    else if (optionNo == 6) 
    {
      command = new DisplayCoinsCommand(coins);
    } 
    else if (optionNo == 7) 
    {
      command = new ResetStockCommand(list);
    } 
    else if (optionNo == 8) 
    {
      command = new ResetCoinsCommand(coins);
    } 
    else if (optionNo == 9) {
        command = new AbortProgramCommand(exitProgram);
    }

    // Execute the command that was created (if created, otherwise could lead to memory issues)
    if (command != nullptr) {
      command->execute();
      delete command;
    }
}

/* returns True for DoublyLinked List and False for Linked List*/
bool readListStructure()
{
    bool choice =  false;
    bool validOption = false;

    while (!validOption)
    {
        std::cout << "Which Datastructure would you like to load Stock into: " << std::endl;
        std::cout << "\t1) Linked List" << std::endl;
        std::cout << "\t2) Doubly-Linked List" << std::endl;
        std::cout << "Enter the number of choice: ";

        string input = readInput();

        if (std::cin.eof())
        {
            std::cout << "\nCtrl-D was pressed, terminating program." << std::endl;
            validOption = true;
        }
        else if (input == "1")
        {
            choice = false;
            validOption = true;
        }
        else if (input == "2")
        {
            choice = true;
            validOption = true;
        }
        else if (input == "help")
        {
            std::cout << "Help: Enter 1 or 2 to select an option" << std::endl;
        }
        else
        {
            std::cout << "Enter 1 or 2 to select an option" << std::endl;
        }
    }

    return choice;
}


void saveAndExit(LinkedList &list, std::vector<Coin> &coins, string stockFilePath, string coinFilePath)
{
    std::ofstream outputStockFile(stockFilePath, std::ofstream::out);

    if (outputStockFile.is_open())
    {
        
        for (const std::string &id : list.idList)
        {
            outputStockFile << id << STOCK_DELIM;
            outputStockFile << list.getName(id) << STOCK_DELIM;
            outputStockFile << list.getDescription(id) << STOCK_DELIM;
            outputStockFile << list.getPrice(id).dollars << PRICE_DELIM;
            outputStockFile << std::setfill('0') << std::setw(2) << list.getPrice(id).cents << STOCK_DELIM;
            outputStockFile << list.getStockLevels(id) << "\n";
        }

        outputStockFile.close();
        std::cout << "Stock File saved successfully." << std::endl;
    }
    else
    {
        std::cout << "Error opening Stock file." << std::endl;
    }

    std::ofstream outputCoinFile(coinFilePath, std::ofstream::out);
    std::vector<int> expectedvector = {5, 10, 20, 50, 100, 200, 500, 1000};

    if (outputCoinFile.is_open())
    {
        for (int i = 0; i < static_cast<int>(coins.size()); i++)
        {

            outputCoinFile << expectedvector[i] << DELIM;
            outputCoinFile << coins[i].count << "\n";
        }

        outputCoinFile.close();
        std::cout << "Coin File saved successfully." << std::endl;
    }
    else
    {
        std::cout << "Error opening Coin file." << std::endl;
    }
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

        int cents, dollars, onHand;

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
                dollars = std::stoul(dollarsString);
                onHand = std::stoul(on_handString);
                cents = std::stoul(centsString);
                // additonally also checks that cents is divisible by 5 EVENLY
                if (cents % 5 != 0)
                {
                    std::cout << "Invalid Price Data in Stock File: Cents Must be Valid (end in 0 or 5)" << std::endl;
                    validData = false;
                }
                if (dollars < 0 || cents < 0)
                {
                    std::cout << "Invalid Price Data in Stock File: Price Must be Positive" << std::endl;
                    validData = false;
                }
                if (onHand < 0)
                {
                    std::cout << "Invalid Price Data in Stock File: Stock on Hand Must be Positive" << std::endl;
                    validData = false;
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Invalid Data in Stock File: Check that the Price and Stock on hand feilds are numerical" << std::endl;
                validData = false;
            }
            catch (const std::out_of_range &e)
            {
                std::cout << "Invalid Data in Stock File: Check that the Price and Stock on hand feilds are positive numerics" << std::endl;
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
            if (static_cast<int>(id.size()) != IDLEN )
            {
                std::cout << "Invalid Price Data in Stock File: Check ID is exactly "<< IDLEN << " characters long" << std::endl;
                validData = false;
            }
            if (static_cast<int>(name.length()) > NAMELEN )
            {
                std::cout << "Invalid Price Data in Stock File: Check that stock name is under "<< NAMELEN << " characters" << std::endl;
                validData = false;
            }
            if (static_cast<int>(description.length()) > DESCLEN )
            {
                std::cout << "Invalid Price Data in Stock File: Check that stock name is under "<< DESCLEN << " characters" << std::endl;
                validData = false;
            }
        }
        else
        {
            std::cout << "Incomplete Data in Stock File: Not all feilds of the Stock are present" << std::endl;
            validData = false;
        }
    }

    return validData;
}

/* validates coin data by reading file */
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
        int denomination, quantity;

        // checks all feilds are present, if all exist then checks if string feilds are actually unsigned ints
        if (
            std::getline(linestream, denominationString, delim) &&
            std::getline(linestream, quantityString))
        {
            // checks that strings are all unsigned ints
            try
            {
                denomination = std::stoul(denominationString);
                quantity = std::stoul(quantityString);

                // checks if duplicate value is being inserted
                if (denomSet.insert(denomination).second == false)
                {
                    std::cout << "Duplicate Denominations in Coin File" << std::endl;
                    validData = false;
                }
                if (quantity < 0 )
                {
                    std::cout << "Invalid Data in Coin File: Entry in Quantity feild is negative" << std::endl;
                    validData = false;
                }
                if (denomination < 0 )
                {
                    std::cout << "Invalid Data in Coin File: Denomination in Quantity feild is negative" << std::endl;
                    validData = false;
                }

            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Invalid Data in Coin File: check denominations and quantity are numerics" << std::endl;
                validData = false;
            }
            catch (const std::out_of_range &e)
            {
                std::cout << "Invalid Data in Coin File: check denominations and quantity are positive numerics" << std::endl;
                validData = false;
            }
        }
        else
        {
            // invalid number of feilds
            std::cout << "Invalid Data in Coin File: too many or too little feilds specified for a coin" << std::endl;
            validData = false;
        }
    }
    // checks that the produced set is equal to the expected set
    // (the operator '==' also checks length of set = 8)
    // TODO: maybe find a better way to do this that doesnt involve using hard-coded numbers?
    std::set<int> expectedSet = {5, 10, 20, 50, 100, 200, 500, 1000};
    if (denomSet != expectedSet)
    {
        std::cout << "Invalid Data in Coin File: The denominations entered to not match the set: " << std::endl;
        std::cout << "\t[5, 10, 20, 50, 100, 200, 500, 1000]" << std::endl;
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
*/
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



/**
 * Prints main menu
 **/
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

/**
 * Handles if a user selects purchase item / option 2
 **/
void makePurchase(vector<Coin> &coinVect, LinkedList &list)
{
    std::cout << "Purchase Item" << std::endl
              << "-------------" << std::endl;

    bool invalidItem = true;
    bool enoughInserted = false;
    while (invalidItem)
    {
        std::cout << "Please enter the id of the item you wish to purchase:";
        string itemToPurchase = readInput();
        // check the item does not exist
        if (itemToPurchase.length() == 0)
        {
            // exit don't want to purchase
            std::cout << "\nThe task Purchase Item failed to run successfully." << std::endl;
            std::cout << "Please come again." << std::endl;
            invalidItem = false;
        }
        else if (itemToPurchase == "help"){
            std::cout << "Help: Enter the ID of the item you wish to purchase." << std::endl;
            std::cout << "If unsure of ID, go back by pressing enter and select option 1 to display available items and check their IDs" << std::endl;
        }
        else if (list.getName(itemToPurchase) == "Not Found")
        {
            std::cout << "The item id you entered could not be found\n"
                      << std::endl;
        }
        else
        {
            // if there is no stock
            if (list.getStockLevels(itemToPurchase) == 0)
            {
                std::cout << "There is no stock available, please select another item\n"
                          << std::endl;
            }
            else
            {
                string itemName = list.getName(itemToPurchase);
                std::cout << "You have selected " << itemName
                          << " - " << list.getDescription(itemToPurchase)
                          << ". This will cost you $ ";
                list.getPrice(itemToPurchase).print();
                std::cout << std::endl;
                std::vector<int> expectedValues = {5, 10, 20, 50, 100, 200, 500, 1000};
                double totalInserted = 0.0;
                // something to keep track of user input
                double itemPrice = list.getPrice(itemToPurchase).getTotal();
                vector<Coin> userCoins;
                int enum_count = 0;

                while (enum_count < static_cast<int>(coinVect.size()))
                {
                    std::string denomString, countString;
                    Coin coin;
                    coin.denom = static_cast<Denomination>(enum_count);
                    enum_count++;
                    coin.count = 0;

                    userCoins.push_back(coin);
                }
                std::cout << "Please hand over the money - type in the value of each note/coin in cents." << std::endl;
                std::cout << "Press enter or ctrl-d on a new line to cancel this purchase:\n"
                          << std::endl;

                while (totalInserted < itemPrice) // take user input or exit transaction
                {
                    std::cout << "You currently owe $"
                              << std::fixed << std::setprecision(2) << std::setfill('0')
                              << (itemPrice - totalInserted) << ": ";
                    string currentCoin = readInput();
                    if (isNumber(currentCoin))
                    {
                        int currCoin = stoi(currentCoin);
                        int index = std::distance(expectedValues.begin(), std::find(expectedValues.begin(), expectedValues.end(), currCoin));
                        if (index != 8) // the denomination is valid
                        {
                            // add to userCoins count
                            // get index of currCoin in set
                            userCoins[index].count += 1;
                            double amountInserted = userCoins[index].getDollarValue();
                            std::cout << "You entered " << amountInserted
                                      << std::endl;
                            totalInserted += userCoins[index].getDollarValue();
                        }
                        else
                        {
                            std::cout << "Error: $"
                                      << std::fixed << std::setprecision(2) << std::setfill('0')
                                      << ((double)currCoin / 100)
                                      << " is not a valid denomination of money. Please try again."
                                      << std::endl;
                        }
                    }
                    else if (currentCoin == "")
                    {
                        // exit this we don't want to process transaction anymore
                        // make itemPrice = 0 and then exit loop
                        itemPrice = 0;
                        std::cout << "Change of mind - here is your change: ";
                        printAllCoins(userCoins);
                        // delete user coins
                    }
                    else if (currentCoin == "help")
                    {
                        std::cout << "Help: Enter the coin or not you are giving in cents." << std::endl;
                        std::cout << "Example: If you are giving a $5 note you must enter 500." << std::endl;
                        std::cout << "Be careful: You must only put valid coins or notes $3 is not a valid coin." << std::endl;
                        std::cout << "To give $3 you must first give $2 coin (200) and then give $1 coin (100)" << std::endl;

                    }
                    else
                    {
                        std::cout << "Error: you did not enter a valid integer. Please try again." << std::endl;
                    }
                    enoughInserted = totalInserted >= itemPrice;
                }
                if (enoughInserted && itemPrice != 0) // transaction is to be processed
                {
                    // first do we have enough change?
                    // then calculte change
                    double changeRequired = std::round((totalInserted - itemPrice) * 100) / 100.0; // round to 2dp
                    if (changeRequired == 0.00)                                                    // no change required
                    {
                        std::cout << "No change given, enjoy your " << itemName << std::endl;
                        processMoney(changeRequired, coinVect, userCoins);
                        // update stock
                        list.buy(itemToPurchase);
                        std::cout << "\nPlease come again." << std::endl;
                    }
                    else if (enoughChange(changeRequired, coinVect, userCoins)) // enough change
                    {
                        std::cout << "Here is your "
                                  << itemName
                                  << " and your change of $ "
                                  << std::fixed << std::setprecision(2) << std::setfill('0')
                                  << changeRequired
                                  << ": ";
                        processMoney(changeRequired, coinVect, userCoins);
                        // update stock
                        list.buy(itemToPurchase);
                        std::cout << "\nPlease come again." << std::endl;
                    }
                    else
                    {
                        // not enough change
                        std::cout << "Sorry, we don't have sufficient funds to process your transaction" << std::endl;
                        std::cout << "Here is your refund: ";
                        printAllCoins(userCoins);
                        std::cout << "\nHave a great day." << std::endl;
                    }
                }
                invalidItem = false;
            }
        }
    }
}

/*
Checks we have enough change in the coins list
*/
bool enoughChange(double changeRequired, vector<Coin> &coins, vector<Coin> &userCoins)
{
    double changeToGive = 0;
    double epsilon = 0.0001; // set a small epsilon value

    for (int i = static_cast<int>(coins.size()) - 1; i >= 0; i--)
    {
        unsigned int coinsUsed = 0;
        unsigned int userCoinsUsed = 0;
        bool continueOnSameCoin = (changeToGive + epsilon) <= changeRequired && coins[i].getDollarValue() <= (changeRequired - changeToGive + epsilon);
        while (continueOnSameCoin)
        {
            if (coins[i].count >= 1 && coins[i].count > coinsUsed)
            {
                changeToGive += coins[i].getDollarValue();
                coinsUsed++;
            }
            else if (userCoins[i].count >= 1 && userCoins[i].count > userCoinsUsed)
            {
                changeToGive += userCoins[i].getDollarValue();
                userCoinsUsed++;
            }
            else
            {
                continueOnSameCoin = false;
            }
            continueOnSameCoin = (changeToGive + epsilon) <= changeRequired && coins[i].getDollarValue() <= (changeRequired - changeToGive + epsilon);
        }
    }

    return abs(changeToGive - changeRequired) < epsilon;
}

/*
Processes the transaction and updates coins
*/
void processMoney(double changeRequired, vector<Coin> &coins, vector<Coin> &userCoins)
{
    double changeToGive = 0; // from vending machine to user
    double epsilon = 0.0001; // set a small epsilon value

    for (int i = static_cast<int>(coins.size()) - 1; i >= 0; i--)
    {
        bool coinLessThanChangeNeeded = coins[i].getDollarValue() <= (changeRequired - changeToGive + epsilon);
        bool changeCalculatedLessThanRequred = changeToGive + epsilon <= changeRequired;

        while (changeCalculatedLessThanRequred && coinLessThanChangeNeeded)
        {
            if (coins[i].count >= 1)
            {
                changeToGive += coins[i].getDollarValue();
                coins[i].count--;
                coins[i].print();
                std::cout << " ";
            }
            else if (userCoins[i].count >= 1)
            {
                changeToGive += userCoins[i].getDollarValue();
                userCoins[i].count--;
                coins[i].print();
                std::cout << " ";
            }
            else
            {
                coinLessThanChangeNeeded = false;
            }
            coinLessThanChangeNeeded = coins[i].getDollarValue() <= (changeRequired - changeToGive + epsilon);
            changeCalculatedLessThanRequred = changeToGive + epsilon <= changeRequired;
        }
    }

    // then add coins in user coins to coins and delete user coins
    for (int i = coins.size() - 1; i >= 0; i--)
    {
        coins[i].count = coins[i].count + userCoins[i].count;
    }
}

void printAllCoins(vector<Coin> &coins)
{
    for (int i = coins.size() - 1; i >= 0; i--)
    {
        if (coins[i].count > 0)
        {
            for (unsigned int j = 0; j < coins[i].count; j++)
            {
                coins[i].print();
                std::cout << " ";
            }
        }
    }
}

/*credit: A1 source code helper.cpp file*/
string readInput()
{
    string input;
    string result;

    if (std::getline(std::cin, input))
    {
        result = input;
    }
    else if (std::cin.eof())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        result = "";
    }
    else
    {
        result = "";
    }

    return result;
}

/*credit: A1 source code helper.cpp file*/
bool isNumber(string s)
{
    string::const_iterator it = s.begin();
    char dot = '.';
    int nb_dots = 0;
    while (it != s.end())
    {
        if (*it == dot)
        {
            nb_dots++;
            if (nb_dots > 1)
            {
                break;
            }
        }
        else if (!isdigit(*it))
        {
            break;
        }

        ++it;
    }
    return !s.empty() && s[0] != dot && it == s.end();
}

/*prints Invalid input*/
void printInvalidInput()
{
    std::cout << "Invalid input.\n" << std::endl;
}

std::string generateId(const std::vector<std::string> &idList)
{
    std::set<int> idSet;

    for (const std::string &id : idList)
    {
        int idNum = std::stoi(id.substr(1));
        idSet.insert(idNum);
    }

    int availableID = 1;
    
    while (idSet.find(availableID) != idSet.end())
    {
        availableID++;
    }

    std::ostringstream newId;
    newId << "I" << std::setfill('0') << std::setw(4) << availableID;

    return newId.str();
}

void printDebug()
{
    std::cout << "IM WORKING" << std::endl;
}

void getNewItem(LinkedList &list)
{
    bool itemAdded = false;
    std::string name = "";
    std::string description = "";
    std::string id = generateId(list.idList);
    if(id.size() > 5)
    {
        std::cout << "Error: ID limit reached cannot add anymore items" << std::endl;
        return;
    }
    std::cout << "The id of the new stock will be: " << id << std::endl;
    std::cout << "Please enter the name of the item: ";
    while (!itemAdded)
    {

        if (name != "" && description != "")
        {
            unsigned int cents;
            unsigned int dollars;
            bool keyboardInterupt = getPrice(dollars, cents);
            Price price = {dollars, cents};
            if (!keyboardInterupt)
            {
                Stock newStock = {id, name, description, price, DEFAULT_STOCK_LEVEL};
                list.add(newStock);
                std::cout << "This Item \"" << name << " - " << description << "\" has now been added to the menu" << std::endl;
            }
            itemAdded = true;
        }
        else
        {
            std::string input = readInput();

            if (input == "")
            {
                std::cout << "Item not added. Returning to main menu." << std::endl;
                itemAdded = true;
            }
            else if (input == "help")
            {
                if (name == "")
                {
                    std::cout << "Help: enter the name of the stock you want to add e.g. Lemon Pie" << std::endl;
                }
                else if (description == "")
                {
                    std::cout << "Help: enter the description of the item e.g. tasty lemon pie with coconut shavings" << std::endl;
                }
                else
                {
                    std::cout << "Help: enter the price of the item e.g. 5.75" << std::endl;
                }
            }
            else if (name == "")
            {
                if (input.size() <= NAMELEN || input.size() == 0)
                {
                    if (std::islower(input[0]))
                    {
                        input[0] = std::toupper(input[0]);
                    }
                    name = input;
                    input = "";
                    std::cout << "Please enter the description of the item: ";
                }
                else
                {
                    input.clear();
                    std::cout << "Name too long. Please enter a name less than " << NAMELEN << " characters long: ";
                }
            }
            else if (description == "")
            {
                if (input.size() <= DESCLEN)
                {
                    description = input;
                    std::cout << "Please enter the price of the item: ";
                }
                else
                {
                    input.clear();
                    std::cout << "Description too long. Please enter a description less than " << DESCLEN << " characters long: ";
                }
            }
        }
    }
}

bool getPrice(unsigned int &x, unsigned int &y)
{
    bool gotPrice = false;
    double input;
    bool interupt = false;

    while (!gotPrice)
    {
        std::string inputStr = readInput();
        size_t decimalPos = inputStr.find('.');
        if ((decimalPos != std::string::npos && (inputStr.length() - decimalPos == 3 || inputStr.length() - decimalPos == 2)) || isNumber(inputStr))
        {
            if (inputStr.length() - decimalPos == 2 && !isNumber(inputStr))
            {
                inputStr += "0";
            }

            try
            {
                input = std::stod(inputStr);
                if(input < 0){
                    throw std::invalid_argument("Invalid input. Please enter a valid price::");
                }
                x = static_cast<unsigned int>(input);
                y = static_cast<unsigned int>(round((input - x) * 100));

                if (y % 5 != 0)
                {
                    throw std::invalid_argument("Invalid input. Please enter a valid price:");
                }

                gotPrice = true;
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Invalid input. Please enter a valid price:";
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Invalid input. Please enter a valid price:";
            }
        }
        else if (inputStr == "")
        {
            std::cout << "Item not added. Returning to main menu." << std::endl;
            interupt = true;
            gotPrice = true;
        }
        else if (inputStr == "help")
        {
            std::cout << "Help: enter the price of the item e.g. 5.75" << std::endl;
        }
        else
        {
            std::cerr << "Invalid input. Please enter a number with exactly two digits after the decimal point:";
        }
    }
    return interupt;
}

void removeItem(LinkedList &list)
{
    std::cout << "Enter the item id of the item to remove from the menu: ";
    std::string id = readInput();
    if (id == "help")
    {
        std::cout << "Help: Enter the ID of the item you wish to remove." << std::endl;
        std::cout << "As you are unsure, please select option 1 to display available items first and find the ID from there" << std::endl;
    }
    else 
    {
        std::string name = list.getName(id);
        std::string description = list.getDescription(id);

        if (name != "Not Found")
        {
            std::cout << "\"" << id << " - " << name << " - " << description << "\" has been removed from the system." << std::endl;
            list.remove(id);
        }
        else
        {
            std::cout << "Item not found." << std::endl;
        }
    }
    
}

void displayCoins(std::vector<Coin> &coins)
{

    std::cout << "Coins Summary" << std::endl;
    std::cout << "-------------" << std::endl;
    std::cout << "Denomination    |    Count" << std::endl;
    std::cout << "---------------------------" << std::endl;

    std::vector<std::string> denominations;

    const std::string denomStrings[] = {
        "5 Cents",
        "10 Cents",
        "20 Cents",
        "50 Cents",
        "1 Dollar",
        "2 Dollars",
        "5 Dollars",
        "10 Dollars"};

    for (Coin coin : coins)
    {
        std::ostringstream denom;
        denom << denomStrings[coin.denom];

        if (coin.denom != ONE_DOLLAR && coin.denom != FIVE_DOLLARS)
        {
            denom << " ";
        }

        denominations.push_back(denom.str());
    }

    for (int i = 0; i < static_cast<int>(denominations.size()); i++)
    {
        std::cout << std::left << std::setfill(' ') << std::setw(15)
                  << denominations[i] << " |"
                  << std::right << std::setfill(' ') << std::setw(10) << coins[i].count << std::endl;
    }

    std::cout << "---------------------------" << std::endl;
}

void resetCoins(std::vector<Coin> &coins)
{
    for (int i = 0; i < static_cast<int>(coins.size()); i++)
    {
        coins[i].count = DEFAULT_COIN_COUNT;
    }
    std::cout << "\"All coins have been reset to the default level of " << DEFAULT_COIN_COUNT << "\"" << std::endl;
}