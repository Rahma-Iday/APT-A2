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

}

std::string LinkedList::getName(std::string id){

}

std::string LinkedList::getDescription(std::string id){

}

Price LinkedList::getPrice(std::string id){

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
