#include "LinkedList.h"

LinkedList::LinkedList() {
    head = nullptr;
    tail = nullptr;
    listLength = 0;
}

void LinkedList::add(Stock info){
    
}

void LinkedList::remove(std::string id){

}

void LinkedList::print(){

}

unsigned int LinkedList::getListLength(){
    return listLength;
}

unsigned int LinkedList::getStockLevels(std::string id){
    return 1;
}

std::string LinkedList::getName(std::string id){
    return "name";
}

std::string LinkedList::getDescription(std::string id){
    return "description";
}

Price LinkedList::getPrice(std::string id){
    Price price;
    price.dollars = 0;
    price.cents = 0;
    return price;
}

void LinkedList::restock(){

}

void LinkedList::buy(std::string id){

}

void LinkedList::deleteList(){

}


LinkedList::~LinkedList() {
    deleteList();
}
