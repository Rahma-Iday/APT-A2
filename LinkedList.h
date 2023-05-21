#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"
#include <vector>


class LinkedList
{
public:
    LinkedList();
    ~LinkedList();
    std::vector<std::string> idList;

    // more functions to be added perhaps...
    virtual void add(Stock info);
    virtual void remove(std::string id);
    void print();
    unsigned int getListLength();
    unsigned int  getStockLevels(std::string id);
    std::string getName(std::string id);
    std::string getDescription(std::string id);
    Price getPrice(std::string id);
    void resetStock();
    void buy(std::string id);
    void deleteList();
    void printIdList();



protected:
    // the beginning of the list
    std::shared_ptr<Node> head;

    // how many nodes are there in the list?
    unsigned listLength;
    
};

#endif  // LINKEDLIST_H


