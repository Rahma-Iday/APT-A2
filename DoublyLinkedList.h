#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include "LinkedList.h"

class DoublyLinkedList : public LinkedList
{
public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void add(Stock info) override;
    void remove(std::string id) override;

private:
    // additional tail pointer for doubly linked list
    std::shared_ptr<Node> tail;
};

#endif // DOUBLYLINKEDLIST_H
