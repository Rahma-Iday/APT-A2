#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H
#include "LinkedList.h"


class DoubleLinkedList : public LinkedList
{
public:

    DoubleLinkedList();
    ~DoubleLinkedList();
    void add(Stock info) override;
    void remove(std::string id) override;

protected:
    // the ending of the list
    std::shared_ptr<Node> tail;
    
};

#endif  // DOUBLELINKEDLIST_H


