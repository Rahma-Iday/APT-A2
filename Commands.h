#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include "DoubleLinkedList.h"
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "Coin.h"
#include <dirent.h>
#include <memory>
#include <functional>
using std::string;
using std::vector;

string readInput();
void printInvalidInput();


class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

class DisplayItemsCommand : public Command {
private:
    LinkedList& list;
public:
    DisplayItemsCommand(LinkedList& list);
    void execute() override;
};

class PurchaseItemCommand : public Command {
private:
    vector<Coin>& coins;
    LinkedList& list;
    bool& colour;
public:
    PurchaseItemCommand(vector<Coin>& coins, LinkedList& list, bool& colour);
    void execute() override;
};

class SaveAndExitCommand : public Command {
private:
    LinkedList& list;
    std::vector<Coin>& coins;
    std::string stockFilePath;
    std::string coinFilePath;
public:
    SaveAndExitCommand(LinkedList& list, std::vector<Coin>& coins, std::string stockFilePath, std::string coinFilePath);
    void execute() override;
};

class AddItemCommand : public Command {
private:
    LinkedList& list;
public:
    AddItemCommand(LinkedList& list);
    void execute() override;
};

class RemoveItemCommand : public Command {
private:
    LinkedList& list;
public:
    RemoveItemCommand(LinkedList& list);
    void execute() override;
};

class DisplayCoinsCommand : public Command {
private:
    std::vector<Coin>& coins;
public:
    DisplayCoinsCommand(std::vector<Coin>& coins);
    void execute() override;
};

class ResetStockCommand : public Command {
private:
    LinkedList& list;
public:
    ResetStockCommand(LinkedList& list);
    void execute() override;
};

class ResetCoinsCommand : public Command {
private:
    std::vector<Coin>& coins;
public:
    ResetCoinsCommand(std::vector<Coin>& coins);
    void execute() override;
};

class AbortCommand : public Command {
public:
    AbortCommand();
    void execute() override;
};

#endif //COMMANDS_H
