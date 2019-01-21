#ifndef INVENTORY_H
#define INVENTORY_H
#include "..\items\item.h"

class Inventory
{
private:
    int capacity;
    static const int DEFAULT_CAPACITY = 10;
    Item storage;

public:
    Inventory()
    {};
    ~Inventory()
    {};
};
#endif