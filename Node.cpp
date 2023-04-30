#include "Node.h"

Node::Node(Stock info){

    data = &info;
    next = nullptr;
    back = nullptr;

};
Node::~Node(){
    // TODO
    delete data;
};