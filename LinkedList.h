#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"
#include <vector>


class LinkedList
{
public:
    LinkedList();
    ~LinkedList();

    // more functions to be added perhaps...
    void add(Stock info);
    void remove(std::string id);
    void print();
    unsigned int getListLength();
    unsigned int  getStockLevels(std::string id);
    std::string getName(std::string id);
    std::string getDescription(std::string id);
    Price getPrice(std::string id);
    void restock();
    void buy(std::string id);
    void deleteList();
    std::vector<std::string> idList;
    void printIdList();



private:
    // the beginning of the list
    Node* head;
  
    // how many nodes are there in the list?
    unsigned listLength;
    
};

#endif  // LINKEDLIST_H


