#include <iostream>
#include "CoffeeShop.h"

int main(int argc, const char * argv[]) {
    CoffeeShop coffeeShop;
    coffeeShop.startFirstModel(argv[1], argv[2]);
    coffeeShop.startSecondModel(argv[1], argv[2]);
    return 0;
}
