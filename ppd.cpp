#include <iostream>
#include "LinkedList.h"
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>
#include <algorithm>
using std::string;
using std::vector;

bool checkFilesExist(string stockFile, string coinFile);
bool readStockData(string fileName, char delim);
bool readCoinData(string fileName, char delim);
vector<Stock> loadStockData(string fileName, char delim);
vector<Coin> loadCoinData(string fileName, char delim);


/**
 * manages the running of the program, initialises data structures, loads
 * data, display the main menu, and handles the processing of options. 
 * Make sure free memory and close all files before exiting the program.
 **/
int main(int argc, char **argv)
{
    /* validate command line arguments */
    // TODO
    if (argc==3){
        string stockFile(argv[1]);
        std::string coinFile(argv[2]);
        
        /*loading  data*/
        // check if both files exists, only then read data 
        if (checkFilesExist(stockFile,coinFile)){
            // check if data in both Files are valid, then only loads data 
            if( readStockData(stockFile, STOCK_DELIM) && readCoinData(coinFile, DELIM[0])){
                vector<Stock> stock = loadStockData(stockFile, STOCK_DELIM);
                vector<Coin> coins = loadCoinData(coinFile,DELIM[0]);
            }
        } 

    } else {
        std::cout << "Please make sure exactly 3 command line arguments are entered in the form:\n ./ppd <stockfile> <coinsfile>" << std::endl;
    }
    
    return EXIT_SUCCESS;
}

bool readStockData(string fileName, char delim){
    bool validData = true;

    std::ifstream file(fileName);
    std::string line;

    std::set<string> idSet;

    while (std::getline(file, line)) {
        std::stringstream linestream(line);
        std::string id, name, description, dollarsString, centsString, on_handString;
        unsigned int dollars;
        unsigned int cents;
        unsigned int on_hand;

        // checks all feilds are there, if all there then checks if string feilds are actually unsigned ints
        if(
            std::getline(linestream, id, delim) &&
            std::getline(linestream, name, delim) &&
            std::getline(linestream, description, delim) &&
            std::getline(linestream, dollarsString, PRICE_DELIM) &&
            std::getline(linestream, centsString, delim) &&
            std::getline(linestream, on_handString)
        ){
            // checks that all strings for dollars, cents and stock on hand strings are all unsigned ints 
            try {
                dollars = std::stoul(dollarsString);
                on_hand = std::stoul(on_handString);
                cents = std::stoul(centsString);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: invalid argument: " << e.what() << std::endl;
                //std::cout << "Invalid Data in Stock File" << std::endl;
                validData = false;
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: out of range: " << e.what() << std::endl;
                //std::cout << "Invalid Data in Stock File" << std::endl;
                validData = false;
            } 
            // additonally also checks that cents is divisible by 5 EVENLY
            if (cents % 5 != 0) {
                std::cout << "Invalid Price Data in Stock File: Cents Must be Valid" << std::endl;
                validData = false;
            }
            // checks all ids are unique:
            if(idSet.insert(id).second == false){
                std::cout << "Invalid Data in Stock File, Multiple Entries with the Same ID" << std::endl;
                validData = false;
            }
            // check length of cent String as it must not be >2 or <2
            if ( centsString.size() != CENTLEN ){
                std::cout << "Invalid Price Data in Stock File: Cents Must be specified to Exactly 2 Decimal Places" << std::endl;
                validData = false;
            }

        } else {
            std::cout << "Invalid Data in Stock File" << std::endl;
            validData = false;
        }
        
    }

    return validData;

}

bool readCoinData(string fileName, char delim){
    bool validData = true;

    std::ifstream file(fileName);
    std::string line;
    // creating a set as a set cannot input duplicate values
    std::set<int> denomSet;

    while (std::getline(file, line)) {
        std::stringstream linestream(line);
        std::string denominationString, quantityString;
        unsigned int denomination;
        unsigned int quantity;

        // checks all feilds are there, if all there then checks if string feilds are actually unsigned ints
        if(
            std::getline(linestream, denominationString, delim) &&
            std::getline(linestream, quantityString)
        ){
            // checks that strings are all unsigned ints 
            try {
                denomination = std::stoul(denominationString);
                quantity = std::stoul(quantityString);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: invalid argument: " << e.what() << std::endl;
                //std::cout << "Invalid Data in Stock File" << std::endl;
                validData = false;
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: out of range: " << e.what() << std::endl;
                //std::cout << "Invalid Data in Stock File" << std::endl;
                validData = false;
            } 

            // checks if duplicate value is being inserted 
            if (denomSet.insert(denomination).second == false) {
                std::cout << "Invalid Data in Coin File" << std::endl;
            }

        } else {
            // invalid number of feilds
            std::cout << "Invalid Data in Coin File" << std::endl;
            validData = false;
        }

    }
    // checks that the produced set is equal to the expected set 
    // (the operator '==' also checks length of set = 8)
    // TODO: maybe find a better way to do this that doesnt involve using hard-coded numbers?
    std::set<int> expectedSet = {5,10,20,50,100,200,500,1000};
    if (denomSet != expectedSet){
        std::cout << "Invalid Data in Coin File" << std::endl;
        validData = false;
    } 

    return validData;
}

vector<Stock> loadStockData(string fileName, char delim){
    vector<Stock> stocks;
    std::ifstream file(fileName);
    std::string line;

    std::string dollarsString, centsString, on_handString;
    unsigned int dollars;
    unsigned int cents;


    while (std::getline(file, line)) {
        std::stringstream linestream(line);
        Stock stock;

        std::getline(linestream, stock.id, delim);
        std::getline(linestream, stock.name, delim);
        std::getline(linestream, stock.description, delim);
        std::getline(linestream, dollarsString, PRICE_DELIM);
        std::getline(linestream, centsString, delim);
        std::getline(linestream, on_handString);
        
        //process data 
        dollars = std::stoul(dollarsString);
        cents = std::stoul(centsString);
        stock.on_hand = std::stoul(on_handString);

        stock.price.dollars = dollars;
        stock.price.cents = cents;

        stocks.push_back(stock);  
    }

    //sorts vector alphaetically by name, using lambda function
    std::sort(stocks.begin(), stocks.end(), [](const Stock& a, const Stock& b) {
        //compares the strings lexicographically aka alphabetically
        return a.name < b.name;
    });

    return stocks;
}

vector<Coin> loadCoinData(string fileName, char delim){
    vector<Coin> coins;
    std::ifstream file(fileName);
    std::string line;

    for (int i = 0; i < NUM_DENOMS; i++) {
        std::stringstream linestream(line);
        std::string denomString, countString;

        Coin coin;

        std::getline(linestream, denomString, delim);
        std::getline(linestream, countString);
        
        coin.denom = static_cast<Denomination>(i);
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
bool checkFilesExist(string stockFile, string coinFile){
    bool fileExists = false;

    std::ifstream file1(stockFile, std::ios::binary);
    std::ifstream file2(coinFile, std::ios::binary);

    if (file1.good() && file2.good()){
        fileExists = true;
    } else{
        std::cout << "Either the Coin File or Stock File provided does not exist!" << std::endl;
    }
    file1.close();
    file2.close();

    return fileExists;
}

