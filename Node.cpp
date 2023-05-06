#include "Node.h"

Node::Node(Stock info){
    data = new Stock;
    *data = info;
    next = nullptr;

};
Node::~Node(){
    // TODO
};