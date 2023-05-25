#ifndef NODE_H
#define NODE_H
#include <string>
#include "Coin.h"
#include <iomanip>
#include <iostream>
#include <memory>

// The length of the id string not counting the nul terminator
#define IDLEN 5

// The maximum length of a product name not counting the nul terminator
#define NAMELEN 40

// The maximum length of a product description not counting the nul terminator.
#define DESCLEN 255

// The default coin level to reset the coins to on request
#define DEFAULT_COIN_COUNT 20

// The default stock level that all new stock should start at and that we should reset to on restock
#define DEFAULT_STOCK_LEVEL 20

// The number of denominations of currency available in the system
#define NUM_DENOMS 8

// Stock delimiter
#define STOCK_DELIM '|'

// Price delimeter
#define PRICE_DELIM '.'

#define CENTLEN 2

/**
 * a structure to represent a price. One of the problems with the floating
 * point formats in C++ like float and double is that they have minor issues
 * of inaccuracy due to rounding. In the case of currency this really is
 * not acceptable so we introduce our own type to keep track of currency.
 **/
class Price
{
public:
    // The dollar value, and the cents value for some price
    unsigned dollars, cents;
    void print();
    double getTotal();
};

/**
 * data structure to represent a stock item within the system
 **/
class Stock
{
public:
    // the unique id for this item
    std::string id;

    // the name of this item
    std::string name;

    // the description of this item
    std::string description;

    // the price of this item
    Price price;

    // how many of this item do we have on hand?
    unsigned on_hand;
};

/**
 * the node that holds the data about an item stored in memory
 **/
class Node
{
public:
    Node();
    Node(Stock temp);
    ~Node();
    // pointer to the data held for the node
    std::shared_ptr<Stock> data;
    // pointer to the next node in the list
    std::shared_ptr<Node> next; 
    // pointer to the previous node in the list
    std::shared_ptr<Node> prev; 
};

#endif // NODE_H