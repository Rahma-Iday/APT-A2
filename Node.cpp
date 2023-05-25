#include "Node.h"



Node::Node(Stock info)
{
    data = std::make_shared<Stock>(info);
    next = nullptr;
    prev = nullptr;
};
Node::~Node(){
    // TODO
};
void Price::print()
{
    std::cout << this->dollars;
    std::cout << "." << std::setfill('0') << std::setw(2) << this->cents << std::endl;
}
double Price::getTotal()
{
    double total = this->cents;
    total = total / 100;
    total += this->dollars;
    return total;
}