#include "Coin.h"
#include <iostream>

// implement functions for managing coins; this may depend on your design.
double Coin::getDollarValue()
{
    double value = 0;
    if (this->denom == 0)
    {
        value = 0.05;
    }
    else if (this->denom == 1)
    {
        value = 0.10;
    }
    else if (this->denom == 2)
    {
        value = 0.20;
    }
    else if (this->denom == 3)
    {
        value = 0.50;
    }
    else if (this->denom == 4)
    {
        value = 1.00;
    }
    else if (this->denom == 5)
    {
        value = 2.00;
    }
    else if (this->denom == 6)
    {
        value = 5.00;
    }
    else if (this->denom == 7)
    {
        value = 10.00;
    }
    return value;
}

void Coin::print()
{
    if (this->denom == 0)
    {
        std::cout << "5c";
    }
    else if (this->denom == 1)
    {
        std::cout << "10c";
    }
    else if (this->denom == 2)
    {
        std::cout << "20c";
    }
    else if (this->denom == 3)
    {
        std::cout << "50c";
    }
    else if (this->denom == 4)
    {
        std::cout << "$1";
    }
    else if (this->denom == 5)
    {
        std::cout << "$2";
    }
    else if (this->denom == 6)
    {
        std::cout << "$5";
    }
    else if (this->denom == 7)
    {
        std::cout << "$10";
    }
}
