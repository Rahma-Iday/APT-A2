#include "DoubleLinkedList.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <algorithm>

DoubleLinkedList::DoubleLinkedList()
{
    head = nullptr;
    tail = nullptr;
    listLength = 0;
}

DoubleLinkedList::~DoubleLinkedList()
{
    listLength = 0;
}

void DoubleLinkedList::add(Stock info) 
{
    std::shared_ptr<Node> newNode = std::make_shared<Node>(info);
    idList.push_back(info.id);

    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        if (head->data->name > newNode->data->name)
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        else
        {
            std::shared_ptr<Node> currNode = head;
            while (currNode->next != nullptr && currNode->next->data->name < newNode->data->name)
            {
                currNode = currNode->next;
            }
            newNode->next = currNode->next;
            newNode->prev = currNode;
            if (currNode->next != nullptr)
            {
                currNode->next->prev = newNode;
            }
            currNode->next = newNode;
            if (newNode->next == nullptr)
            {
                tail = newNode;
            }
        }
    }
    listLength++;
}

void DoubleLinkedList::remove(std::string id) 
{
    /* remove id from idList attribute */
    idList.erase(std::remove(idList.begin(), idList.end(), id), idList.end());

    std::shared_ptr<Node> currNode = head;
    while (currNode != nullptr)
    {
        if (currNode->data->id == id)
        {
            if (currNode->prev != nullptr)
            {
                currNode->prev->next = currNode->next;
            }
            else
            {
                // currNode is the head
                head = currNode->next;
            }
            if (currNode->next != nullptr)
            {
                currNode->next->prev = currNode->prev;
            }
            else
            {
                // currNode is the tail
                tail = currNode->prev;
            }
            listLength--;
            return;
        }
        currNode = currNode->next;
    }
}