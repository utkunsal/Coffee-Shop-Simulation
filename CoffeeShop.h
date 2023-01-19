#ifndef CoffeeShop_h
#define CoffeeShop_h
#include <vector>
#include "Cashier.h"
#include "Barista.h"

class CoffeeShop {
public:
    void startFirstModel(std::string inputFileName, std::string outputFileName);
    void startSecondModel(std::string inputFileName, std::string outputFileName);
private:
    int getIndexOfCashierWithTheSmallestTimeToBeFree(std::vector<Cashier> cashiers);
    int getIndexOfBaristaWithTheSmallestTimeToBeFree(std::vector<Barista> baristas);
};


#endif /* CoffeeShop_h */
