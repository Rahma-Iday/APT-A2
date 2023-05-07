#include "Coin.h"

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
        value = 0.1;
    }
    else if (this->denom == 2)
    {
        value = 0.2;
    }
    else if (this->denom == 3)
    {
        value = 0.5;
    }
    else if (this->denom == 4)
    {
        value = 1.0;
    }
    else if (this->denom == 5)
    {
        value = 2.0;
    }
    else if (this->denom == 6)
    {
        value = 5.0;
    }
    else if (this->denom == 7)
    {
        value = 10.0;
    }
    return value;
}
