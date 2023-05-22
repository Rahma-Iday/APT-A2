#include "Commands.h"
string readInput();
bool isNumber(string s);
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

void saveAndExit(LinkedList &list, std::vector<Coin> &coins, string stockFilePath, string coinFilePath)//save the stock and coins to the file
{
    std::ofstream outputStockFile(stockFilePath, std::ofstream::out);//open the stock file

    if (outputStockFile.is_open())//if the file is open write out every stock we have in the list
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
    else//if the file is not open print out an error message
    {
        std::cout << "Error opening Stock file." << std::endl;
    }

    std::ofstream outputCoinFile(coinFilePath, std::ofstream::out);//open the coin file
    std::vector<int> expectedvector = {5, 10, 20, 50, 100, 200, 500, 1000};

    if (outputCoinFile.is_open())//if the file is open write out every coin we have in the list
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
    while (invalidItem)//while the item is invalid
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
        else if (list.getName(itemToPurchase) == "Not Found")//if the item is not found
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
            else//there is stock and we continue with the purchase
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

                while (enum_count < static_cast<int>(coinVect.size()))//while the enum count is less than the size of the coin vector
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

                    string currentCoin = readInput();//read in the current coin
                    if (isNumber(currentCoin))//if the current coin is a number
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
                    else//the current coin is not a number
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

    for (int i = static_cast<int>(coins.size()) - 1; i >= 0; i--)//for each coin in the coins vector
    {
        unsigned int coinsUsed = 0;
        unsigned int userCoinsUsed = 0;
        bool continueOnSameCoin = (changeToGive + epsilon) <= changeRequired && coins[i].getDollarValue() <= (changeRequired - changeToGive + epsilon);
        while (continueOnSameCoin)//while we can continue on the same coin
        {
            if (coins[i].count >= 1 && coins[i].count > coinsUsed)//if the coin count is greater than 1 and the coin count is greater than the coins used
            {
                changeToGive += coins[i].getDollarValue();
                coinsUsed++;
            }
            else if (userCoins[i].count >= 1 && userCoins[i].count > userCoinsUsed)//if the user coin count is greater than 1 and the user coin count is greater than the user coins used
            {
                changeToGive += userCoins[i].getDollarValue();
                userCoinsUsed++;
            }
            else//otherwise we can't continue on the same coin
            {
                continueOnSameCoin = false;
            }
            continueOnSameCoin = (changeToGive + epsilon) <= changeRequired && coins[i].getDollarValue() <= (changeRequired - changeToGive + epsilon);
        }
    }

    return abs(changeToGive - changeRequired) < epsilon;//return true if the change to give is less than the change required
}

/*
Processes the transaction and updates coins
*/
void processMoney(double changeRequired, vector<Coin> &coins, vector<Coin> &userCoins)//processes the money
{
    double changeToGive = 0; // from vending machine to user
    double epsilon = 0.0001; // set a small epsilon value

    for (int i = static_cast<int>(coins.size()) - 1; i >= 0; i--)//for each coin in the coins vector
    {
        bool coinLessThanChangeNeeded = coins[i].getDollarValue() <= (changeRequired - changeToGive + epsilon);
        bool changeCalculatedLessThanRequred = changeToGive + epsilon <= changeRequired;

        while (changeCalculatedLessThanRequred && coinLessThanChangeNeeded)//while the change calculated is less than the change required 
        {
            if (coins[i].count >= 1)//if the coin count is greater than 1
            {
                changeToGive += coins[i].getDollarValue();
                coins[i].count--;
                coins[i].print();
                std::cout << " ";
            }
            else if (userCoins[i].count >= 1)//if the user coin count is greater than 1
            {
                changeToGive += userCoins[i].getDollarValue();
                userCoins[i].count--;
                coins[i].print();
                std::cout << " ";
            }
            else//otherwise we can't continue on the same coin
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

void printAllCoins(vector<Coin> &coins)//prints all the coins
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



std::string generateId(const std::vector<std::string> &idList)//generates an id
{
    std::set<int> idSet;

    for (const std::string &id : idList)//for each id in the id list we put it into the id  set
    {
        int idNum = std::stoi(id.substr(1));
        idSet.insert(idNum);
    }

    int availableID = 1;//available id is 1
    
    while (idSet.find(availableID) != idSet.end())//while the id set is not equal to the id set end
    {
        availableID++;
    }

    std::ostringstream newId;
    newId << "I" << std::setfill('0') << std::setw(4) << availableID;//set the new id to the available id

    return newId.str();
}

void printDebug()//prints debug
{
    std::cout << "IM WORKING" << std::endl;
}

void getNewItem(LinkedList &list)//gets a new item
{
    bool itemAdded = false;//item added is false and sets all the variables to empty
    std::string name = "";
    std::string description = "";
    std::string id = generateId(list.idList);//generates an id
    if(id.size() > 5)//if the id size is greater than 5
    {
        std::cout << "Error: ID limit reached cannot add anymore items" << std::endl;
        return;
    }
    std::cout << "The id of the new stock will be: " << id << std::endl;
    std::cout << "Please enter the name of the item: ";
    while (!itemAdded)//while the item is not added
    {

        if (name != "" && description != "")//if the name and decription is already there move onto the money
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
        else//we get teh name and description
        {
            std::string input = readInput();

            if (input == "")
            {
                std::cout << "Item not added. Returning to main menu." << std::endl;
                itemAdded = true;
            }
            else if (name == "")//if the name is empty
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
                else//otherwise the name is too long
                {
                    input.clear();
                    std::cout << "Name too long. Please enter a name less than " << NAMELEN << " characters long: ";
                }
            }
            else if (description == "")//if the description is empty
            {
                if (input.size() <= DESCLEN)
                {
                    description = input;
                    std::cout << "Please enter the price of the item: ";
                }
                else//otherwise the description is too long
                {
                    input.clear();
                    std::cout << "Description too long. Please enter a description less than " << DESCLEN << " characters long: ";
                }
            }
        }
    }
}

bool getPrice(unsigned int &x, unsigned int &y)//gets the price
{
    bool gotPrice = false;
    double input;
    bool interupt = false;

    while (!gotPrice)//while we don't have the price
    {
        std::string inputStr = readInput();
        size_t decimalPos = inputStr.find('.');//we check and see if there is a decimal point
        if ((decimalPos != std::string::npos && (inputStr.length() - decimalPos == 3 || inputStr.length() - decimalPos == 2)) || isNumber(inputStr))
        {
            if (inputStr.length() - decimalPos == 2 && !isNumber(inputStr))//if the length of the number after the decimal isn't 2
            {
                inputStr += "0";
            }

            try//we try to get the price
            {
                input = std::stod(inputStr);//we get the input
                if(input < 0){//if the input is less than 0
                    throw std::invalid_argument("Invalid input. Please enter a valid price that is not negative: ");
                }
                x = static_cast<unsigned int>(input);//we set the x and y
                y = static_cast<unsigned int>(round((input - x) * 100));

                if (y % 5 != 0)//if the cents is not divisible by 5
                {
                    throw std::invalid_argument("Invalid input. Make sure the cents is devisible by 5:");
                }//otherwise we have the price

                gotPrice = true;
            }
            catch (const std::invalid_argument &e)//catches the invalid argument
            {
                std::cerr << "Invalid input. Make sure there are two numbers after the decimal point that can be divided by 5:";
            }
            catch (const std::out_of_range &e)//catches the out of range
            {
                std::cerr << "Invalid input. Please enter a number that is not too big have does not have" <<
                                                                    "more than two decimal numbers:";
            }
        }
        else if (inputStr == "")//if the input is empty return to the main menu
        {
            std::cout << "Item not added. Returning to main menu." << std::endl;
            interupt = true;
            gotPrice = true;
        }
        else//otherwise the input is invalid
        {
            std::cerr << "Invalid input. Please enter a number with exactly two digits after the decimal point:";
        }
    }
    return interupt;
}

void removeItem(LinkedList &list)//removes an item
{
    std::cout << "Enter the item id of the item to remove from the menu: ";
    std::string id = readInput();
    std::string name = list.getName(id);
    std::string description = list.getDescription(id);

    if (name != "Not Found")//if the name is in the list we remove it
    {
        std::cout << "\"" << id << " - " << name << " - " << description << "\" has been removed from the system." << std::endl;
        list.remove(id);
    }
    else//otherwise the item is not found
    {
        std::cout << "Item not found." << std::endl;
    }
}

void displayCoins(std::vector<Coin> &coins) //displays the coins
{

    std::cout << "Coins Summary" << std::endl;
    std::cout << "-------------" << std::endl;
    std::cout << "Denomination    |    Count" << std::endl;
    std::cout << "---------------------------" << std::endl;

    std::vector<std::string> denominations;

    const std::string denomStrings[] = {//the denominations
        "5 Cents",
        "10 Cents",
        "20 Cents",
        "50 Cents",
        "1 Dollar",
        "2 Dollars",
        "5 Dollars",
        "10 Dollars"};

    for (Coin coin : coins)//we get the denominations
    {
        std::ostringstream denom;
        denom << denomStrings[coin.denom];

        if (coin.denom != ONE_DOLLAR && coin.denom != FIVE_DOLLARS)
        {
            denom << " ";
        }

        denominations.push_back(denom.str());
    }

    for (int i = 0; i < static_cast<int>(denominations.size()); i++)//we print out the denominations
    {
        std::cout << std::left << std::setfill(' ') << std::setw(15)
                  << denominations[i] << " |"
                  << std::right << std::setfill(' ') << std::setw(10) << coins[i].count << std::endl;
    }

    std::cout << "---------------------------" << std::endl;
}

void resetCoins(std::vector<Coin> &coins)//resets the coins
{
    for (int i = 0; i < static_cast<int>(coins.size()); i++)//we reset the coins 
    {
        coins[i].count = DEFAULT_COIN_COUNT;
    }
    std::cout << "\"All coins have been reset to the default level of " << DEFAULT_COIN_COUNT << "\"" << std::endl;
}

/*credit: A1 source code helper.cpp file*/
string readInput()//reads the input as an entire line
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