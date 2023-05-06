#include <iostream>
#include "LinkedList.h"
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <stdexcept>
using std::string;
using std::vector;

bool checkFilesExist(string stockFile, string coinFile);
bool readStockData(string fileName, char delim);
bool readCoinData(string fileName, char delim);
vector<Stock> loadStockData(string fileName, char delim);
vector<Coin> loadCoinData(string fileName, char delim);
void makePurchase();
void displayMenu();
string readInput();
bool isNumber(string s);
void printInvalidInput();
Stock getNewItem(LinkedList &list);
void getPrice(unsigned int& x, unsigned int& y);
void printDebug();
void handleInput(LinkedList &list, string stockFile, string coinFile, vector<Coin>& coins);
void handleOptions(LinkedList &list, bool &exitProgram, int &optionNo, string stockFile, string coinFile, vector<Coin>& coins);
void removeItem(LinkedList &list);
void saveAndExit(LinkedList &list, string stockFile, string coinFile);
void displayCoins(std::vector<Coin> coins);
void resetCoins(std::vector<Coin> &coins);




/**
 * manages the running of the program, initialises data structures, loads
 * data, display the main menu, and handles the processing of options.
 * Make sure free memory and close all files before exiting the program.
 **/
int main(int argc, char **argv)
{
    /* validate command line arguments */
    // TODO
    if (argc == 3){
        string stockFile(argv[1]);
        std::string coinFile(argv[2]);

        /*loading  data*/
        // check if both files exists, only then read data
        if (checkFilesExist(stockFile, coinFile))
        {
            // check if both Files have valid data by reading, then only loads data
            if (readStockData(stockFile, STOCK_DELIM) && readCoinData(coinFile, DELIM[0]))
            {
                vector<Stock> stock = loadStockData(stockFile, STOCK_DELIM);
                vector<Coin> coins = loadCoinData(coinFile, DELIM[0]);

                // put stock vector's stocks into linked list 
                LinkedList list;
                for (int i=0; i<static_cast<int>(stock.size()); i++)
                {
                    list.add(stock[i]);
                }
                /*load coin into array data type?*/
                
                handleInput(list, stockFile, coinFile, coins);
            }
        }
    }
    else{
        std::cout << "Please make sure exactly 3 command line arguments are entered in the form:\n ./ppd <stockfile> <coinsfile>" << std::endl;
    }
    return EXIT_SUCCESS;
}


void handleInput(LinkedList &list, string stockFile, string coinFile, vector<Coin>& coins){

    // create a loop that executes the return to main menu functionality until exit options (3 or 9) are pressed
    // set a bool value to ensure main menu is displayed until valid input given
    displayMenu();
    bool exitProgram = false;

    while (!exitProgram)
    {
        bool validOption = false;
        int optionNo = 0;
        while(!validOption)
        {
            std::cout << "Please enter your choice: ";
            string input = readInput();

            if (std::cin.eof()){
                std::cout << "\nCtrl-D was pressed, terminating program." << std::endl;
                exitProgram = true;
                validOption = true;

            } else if (isNumber(input)){
                optionNo = std::stoi(input);
                if (optionNo>0 && optionNo<10){
                    validOption = true; 
                }else {
                    printInvalidInput();
                }
                
            }else if(input==""){
                displayMenu();
            } else {
                printInvalidInput();
            }
        }
        std::cout << std::endl;
        handleOptions(list, exitProgram, optionNo, stockFile, coinFile, coins);
        std::cout << std::endl;
    }
}


void handleOptions(LinkedList &list, bool &exitProgram, int &optionNo, string stockFile, string coinFile, vector<Coin>& coins){
    if (optionNo == 1){
    // Display items 
    list.print();
    // no exiting program, thus re-displays main menu
    } else if (optionNo == 2){// Purchase Item
        
    
    } else if (optionNo == 3){// Save and Exit
        saveAndExit(list, stockFile, coinFile);
        exitProgram = true; // only if method returns true tho
    
    } else if (optionNo == 4){// Add item
        Stock newItem = getNewItem(list);
        list.add(newItem);

        std::cout<< "This Item \"" << newItem.name << " - " << newItem.description << 
        "\" has now been added to the menu" <<std::endl;
    
    } else if (optionNo == 5){// Remove Item
        removeItem(list);
    } else if (optionNo == 6){// Display Coins
        displayCoins(coins);   
    } else if (optionNo == 7){// Reset Stock
        list.resetStock();
        std::cout << "\"All stock has been reset to " << DEFAULT_STOCK_LEVEL << "\"" << std::endl;
    } else if (optionNo == 8){// Reset Coins
        resetCoins(coins);
    } else if (optionNo == 9){// Abort the Program
        exitProgram = true; 
    }
}


void saveAndExit(LinkedList &list, string stockFile, string coinFile)
{
    std::ofstream outputStockFile(stockFile, std::ofstream::out);

    if (outputStockFile.is_open()) {
        std::vector<std::string> idList = list.idList;
        for (const std::string& id : idList) {
            outputStockFile << id << STOCK_DELIM;
            outputStockFile << list.getName(id) << STOCK_DELIM;
            outputStockFile << list.getDescription(id) << STOCK_DELIM;
            outputStockFile << list.getPrice(id).dollars << PRICE_DELIM;
            outputStockFile << std::setfill('0') << std::setw(2)<< list.getPrice(id).cents << STOCK_DELIM;
            outputStockFile << list.getStockLevels(id) << "\n";
        }

        outputStockFile.close();
        std::cout << "Stock File saved successfully." << std::endl;
    } else {
        std::cout << "Error opening file." << std::endl;
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
                std::cerr << "Error: invalid argument: " << e.what() << std::endl;
                // std::cout << "Invalid Data in Stock File" << std::endl;
                validData = false;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Error: out of range: " << e.what() << std::endl;
                // std::cout << "Invalid Data in Stock File" << std::endl;
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
            if ( static_cast<int>(id.size()) != IDLEN || static_cast<int>(name.length()) > NAMELEN || static_cast<int>(description.length()) > DESCLEN){
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
                std::cerr << "Error: invalid argument: " << e.what() << std::endl;
                // std::cout << "Invalid Data in Stock File" << std::endl;
                validData = false;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Error: out of range: " << e.what() << std::endl;
                // std::cout << "Invalid Data in Stock File" << std::endl;
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

    // sorts vector alphaetically by name, using lambda function
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
    int enum_count = 0;

    while (std::getline(file, line))
    {
        std::stringstream linestream(line);
        std::string denomString, countString;

        Coin coin;

        std::getline(linestream, denomString, delim);
        std::getline(linestream, countString);

        coin.denom = static_cast<Denomination>(enum_count);
        enum_count++;
        coin.count = std::stoul(countString);

        coins.push_back(coin);
    }

    return coins;
}

/**
 * Checks that both provided files exist.
 * If both exists, returns true.
 * If not returns false.
 **/
bool checkFilesExist(string stockFile, string coinFile)
{
    bool fileExists = false;

    std::ifstream file1(stockFile, std::ios::binary);
    std::ifstream file2(coinFile, std::ios::binary);

    if (file1.good() && file2.good())
    {
        fileExists = true;
    }
    else
    {
        std::cout << "Either the Coin File or Stock File provided does not exist!" << std::endl;
    }
    file1.close();
    file2.close();

    return fileExists;
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

void makePurchase()
{}
/*credit: A1 source code helper.cpp file*/
string readInput()
{
    string input;
    std::getline(std::cin, input);

    return input;
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
            if (nb_dots>1)
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

std::string generateId(const std::vector<std::string>& idList){
    int maxID = -1;

    for(const std::string& id : idList){
        int idNum = std::stoi(id.substr(1));
        if(idNum > maxID){
            maxID = idNum;
        }
    }

    std::ostringstream newId;
    newId << "I" << std::setfill('0') << std::setw(4) << (maxID + 1);
    
    return newId.str();
}

void printDebug(){
    std::cout << "IM WORKING" << std::endl;
}


Stock getNewItem(LinkedList &list){
    std::string id = generateId(list.idList);
    std::cout << "The id of the new stock will be: " << id << std::endl;
    std::cout << "Please enter the name of the item: ";
    std::string name = readInput();
    if (!name.empty() && std::islower(name[0])) {
    name[0] = std::toupper(name[0]);
    }
    std::cout << "Please enter the description of the item: ";
    std::string description = readInput();
    std::cout << "Please enter the price of the item: ";
    unsigned int cents;
    unsigned int dollars;
    getPrice(dollars, cents);
    Price price = {dollars, cents};
    Stock newStock = {id, name, description, price, DEFAULT_STOCK_LEVEL};
    return newStock;
}

void getPrice(unsigned int& x, unsigned int& y) {
    bool gotPrice = false;
    double input;

    while (!gotPrice) {
        std::string inputStr = readInput();
        size_t decimalPos = inputStr.find('.');
        if ((decimalPos != std::string::npos && (inputStr.length() - decimalPos == 3 || inputStr.length() - decimalPos == 2))|| isNumber(inputStr)) {
            if (inputStr.length() - decimalPos == 2) {
                inputStr += "0";
            }

            try {
                input = std::stod(inputStr);
                x = static_cast<unsigned int>(input);
                y = static_cast<unsigned int>(round((input - x) * 100));

                if (y % 5 != 0) {
                    throw std::invalid_argument("Invalid input. Please enter a valid price:");
                }

                gotPrice = true;

            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid input. Please enter a valid price:";
            } catch (const std::out_of_range& e) {
                std::cerr << "Invalid input. Please enter a valid price:";
            }
        } else {
            std::cerr << "Invalid input. Please enter a number with exactly two digits after the decimal point:";
        }
    }
}

void removeItem(LinkedList &list){
    std::cout << "Enter the item id of the item to remove from the menu: ";
    std::string id = readInput();
    std::string name = list.getName(id);
    std::string description = list.getDescription(id);
    
    if(name != "Not Found"){
        std::cout <<"\""<< id << " - " << name << " - " << description << "\" has been removed from the system."<< std::endl;
        list.remove(id);
    } else {
        std::cout << "Item not found." << std::endl;
    }
}

#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "Coin.h"

void displayCoins(std::vector<Coin> coins) {
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
        "10 Dollars"
    };

    for (Coin coin : coins) {
        std::ostringstream denom;
        denom << denomStrings[coin.denom];

        if (coin.denom != ONE_DOLLAR && coin.denom != FIVE_DOLLARS) {
            denom << " ";
        }

        denominations.push_back(denom.str());
    }

    for (int i = 0; i < denominations.size(); i++) {
        std::cout << std::left << std::setw(15) << denominations[i] << " |" << std::right << std::setw(10) << coins[i].count << std::endl;
    }

    

    std::cout << "---------------------------" << std::endl;
}

void resetCoins(std::vector<Coin> &coins){
    for (int i = 0; i < coins.size(); i++) {
        coins[i].count = DEFAULT_COIN_COUNT;
    }
    std::cout << "\"All coins have been reset to the default level of "<< DEFAULT_COIN_COUNT << "\"" << std::endl;
}

