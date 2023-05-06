#include "Node.h"

Node::Node(Stock info)
{
    data = new Stock;
    *data = info;
    next = nullptr;
};
Node::~Node(){
    // TODO
};
void Price::print()
{
    std::cout << this->dollars;
    std::cout << "." << std::setfill('0') << std::setw(2) << this->cents << std::endl;
}