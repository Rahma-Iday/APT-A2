#include "LinkedList.h"
#include <iostream>
#include <iomanip>

LinkedList::LinkedList()
{
    head = nullptr;
    listLength = 0;
}

void LinkedList::add(Stock info)
{
    Node *newNode = new Node(info);
    idList.push_back(info.id);

    if (head == nullptr)
    {
        head = newNode;
    }
    else if (head->data->name > newNode->data->name)
    {
        newNode->next = head;
        head = newNode;
    }
    else
    {
        Node *currNode = head;
        Node *nextNode = head->next;
        bool inserted = false;
        while (nextNode != nullptr && !inserted)
        {
            if (nextNode->data->name > newNode->data->name)
            {
                currNode->next = newNode;
                newNode->next = nextNode;
                inserted = true;
            }
            else
            {
                currNode = currNode->next;
                nextNode = nextNode->next;
            }
        }
        if (!inserted)
        {
            currNode->next = newNode;
        }
    }
    listLength++;
}

void LinkedList::remove(std::string id)
{
    Node *temp = head;
    Node *prev = nullptr;
    while (temp != nullptr)
    {
        if (temp->data->id == id)
        {
            if (prev == nullptr)
            {
                head = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            delete temp;
            listLength--;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void LinkedList::print()
{
    std::cout << "Items Menu" << std::endl;
    std::cout << "----------" << std::endl;
    std::cout << "ID   |Name                                    | Available | Price" << std::endl;
    std::cout << "-------------------------------------------------------------------" << std::endl;

    Node *temp = head;
    while (temp != nullptr)
    {
        std::cout << std::left << std::setfill(' ') << std::setw(5) << temp->data->id << "|";
        std::cout << std::left << std::setfill(' ') << std::setw(40) << temp->data->name << "| ";
        std::cout << std::left << std::setfill(' ') << std::setw(9) << temp->data->on_hand << "| ";
        std::cout << "$" << temp->data->price.dollars << PRICE_DELIM;
        std::cout << std::setfill('0') << std::setw(2) << temp->data->price.cents << std::endl;
        std::cout << std::setfill(' ');
        temp = temp->next;
    }
}

unsigned int LinkedList::getListLength()
{
    return listLength;
}

unsigned int LinkedList::getStockLevels(std::string id)
{
    int stockLevel = 0;
    Node *temp = head;

    while (temp != nullptr)
    {
        if (temp->data->id == id)
        {
            stockLevel = temp->data->on_hand;
            break;
        }
        temp = temp->next;
    }

    return stockLevel;
}

std::string LinkedList::getName(std::string id)
{
    std::string name = "Not Found";
    Node *temp = head;

    while (temp != nullptr)
    {
        if (temp->data->id == id)
        {
            name = temp->data->name;
            break;
        }
        temp = temp->next;
    }

    return name;
}

std::string LinkedList::getDescription(std::string id)
{
    std::string description = "Not Found";
    Node *temp = head;

    while (temp != nullptr)
    {
        if (temp->data->id == id)
        {
            description = temp->data->description;
            break;
        }
        temp = temp->next;
    }

    return description;
}

Price LinkedList::getPrice(std::string id)
{
    Price price;
    price.dollars = 0;
    price.cents = 0;

    Node *temp = head;
    while (temp != nullptr)
    {
        if (temp->data->id == id)
        {
            price.dollars = temp->data->price.dollars;
            price.cents = temp->data->price.cents;
        }
        temp = temp->next;
    }

    return price;
}

void LinkedList::resetStock()
{
    Node *temp = head;
    while (temp != nullptr)
    {
        temp->data->on_hand = DEFAULT_STOCK_LEVEL;
        temp = temp->next;
    }
}

void LinkedList::buy(std::string id)
{
    Node *temp = head;
    while (temp != nullptr)
    {
        if (temp->data->id == id)
        {
            if (temp->data->on_hand > 0)
            {
                temp->data->on_hand--;
                std::cout << "You have purchased " << temp->data->name << std::endl;
            }
            else
            {
                std::cout << "Sorry, " << temp->data->name << " is out of stock" << std::endl;
            }
        }
        temp = temp->next;
    }
}

void LinkedList::deleteList()
{
    Node *temp = head;
    while (temp != nullptr)
    {
        Node *next = temp->next;
        delete temp;
        temp = next;
    }
    head = nullptr;
    listLength = 0;
}

LinkedList::~LinkedList()
{
    deleteList();
}

void LinkedList::printIdList()
{
    std::cout << "ID List: ";
    for (const std::string &id : idList)
    {
        std::cout << id << " ";
    }
    std::cout << std::endl;
}