#include "Commands.h"
string readInput();
bool isNumber(string s);
void printInvalidInput();
void getNewItem(LinkedList &list);
bool getPrice(unsigned int &x, unsigned int &y);
void printDebug();
void removeItem(LinkedList &list);
void saveAndExit(LinkedList &list, std::vector<Coin> &coins, string stockFilePath, string coinFilePath);
void displayCoins(std::vector<Coin> &coins);
void resetCoins(std::vector<Coin> &coins);
void makePurchase(vector<Coin> &coinVect, LinkedList &list, bool &colour);
bool enoughChange(double changeRequired, vector<Coin> &coins, vector<Coin> &userCoins);
void processMoney(double changeRequired, vector<Coin> &coins, vector<Coin> &userCoins);
void printAllCoins(vector<Coin> &coins);


DisplayItemsCommand::DisplayItemsCommand(LinkedList& list) : list(list) {}

void DisplayItemsCommand::execute() {
    list.print();
}

PurchaseItemCommand::PurchaseItemCommand(vector<Coin>& coins, LinkedList& list, bool& colour) 
    : coins(coins), list(list), colour(colour) {}

void PurchaseItemCommand::execute() {
    makePurchase(coins, list, colour);
}

SaveAndExitCommand::SaveAndExitCommand(LinkedList& list, std::vector<Coin>& coins, std::string stockFilePath, std::string coinFilePath) 
    : list(list), coins(coins), stockFilePath(stockFilePath), coinFilePath(coinFilePath) {}

void SaveAndExitCommand::execute() {
    saveAndExit(list, coins, stockFilePath, coinFilePath);
    // handle program exit in main function
}

AddItemCommand::AddItemCommand(LinkedList& list) : list(list) {}

void AddItemCommand::execute() {
    getNewItem(list);
}

RemoveItemCommand::RemoveItemCommand(LinkedList& list) : list(list) {}

void RemoveItemCommand::execute() {
    removeItem(list);
}

DisplayCoinsCommand::DisplayCoinsCommand(std::vector<Coin>& coins) : coins(coins) {}

void DisplayCoinsCommand::execute() {
    displayCoins(coins);
}

ResetStockCommand::ResetStockCommand(LinkedList& list) : list(list) {}

void ResetStockCommand::execute() {
    list.resetStock();
}

ResetCoinsCommand::ResetCoinsCommand(std::vector<Coin>& coins) : coins(coins) {}

void ResetCoinsCommand::execute() {
    resetCoins(coins);
}

AbortCommand::AbortCommand() {}

void AbortCommand::execute() {
    // handle program exit in main function
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


/**
 * Handles if a user selects purchase item / option 2
 **/
void makePurchase(vector<Coin> &coinVect, LinkedList &list, bool &colour)
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

                    if(colour){
                        std::cout << "You currently owe \033[1;34m$"
                                  << std::fixed << std::setprecision(2) << std::setfill('0')
                                  << (itemPrice - totalInserted) << "\033[0m" << ": ";
                    }else{
                        std::cout << "You currently owe $"
                                  << std::fixed << std::setprecision(2) << std::setfill('0')
                                  << (itemPrice - totalInserted) << ": ";
                    }

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

/*prints Invalid input*/
void printInvalidInput()
{
    std::cout << "Invalid input.\n"
              << std::endl;
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