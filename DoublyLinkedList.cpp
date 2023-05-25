#include "DoublyLinkedList.h"
#include <algorithm>

DoublyLinkedList::DoublyLinkedList() : LinkedList()
{
    tail = nullptr;
}

DoublyLinkedList::~DoublyLinkedList()
{
    deleteList();
}

void DoublyLinkedList::add(Stock info)
{
    std::shared_ptr<Node> newNode = std::make_shared<Node>(info);
    idList.push_back(info.id);

    if (head == nullptr)
    {
        // if the list is empty, set both head and tail to the new node
        head = newNode;
        tail = newNode;
    }
    else if (head->data->name > newNode->data->name)
    {
        // if the new node should be inserted at the beginning
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    else
    {
        std::shared_ptr<Node> currNode = head;
        bool inserted = false;

        while (currNode->next != nullptr && !inserted)
        {
            if (currNode->next->data->name > newNode->data->name)
            {
                // insert the new node in the middle
                newNode->next = currNode->next;
                newNode->prev = currNode;
                currNode->next->prev = newNode;
                currNode->next = newNode;
                inserted = true;
            }
            else
            {
                currNode = currNode->next;
            }
        }

        if (!inserted)
        {
            // insert the new node at the end
            currNode->next = newNode;
            newNode->prev = currNode;
            tail = newNode;
        }
    }

    listLength++;
}


void DoublyLinkedList::remove(std::string id)
{
    /* remove id from idList attribute */
    idList.erase(std::remove(idList.begin(), idList.end(), id), idList.end());

    std::shared_ptr<Node> temp = head;
    while (temp != nullptr)
    {
        if (temp->data->id == id)
        {
            if (temp == head)
            {
                // if the node to be removed is the head node
                head = temp->next;
                if (head != nullptr)
                    head->prev = nullptr;
            }
            else if (temp == tail)
            {
                // if the node to be removed is the tail node
                tail = temp->prev;
                if (tail != nullptr)
                    tail->next = nullptr;
            }
            else
            {
                // if the node to be removed is in the middle
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
            }

            listLength--;
            return;
        }
        temp = temp->next;
    }
}

