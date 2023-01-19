#ifndef Cashier_h
#define Cashier_h
#include "Customer.h"

class Cashier {
private:
    double timeToBeFree;
    double totalBusyTime;
public:
    Cashier() {timeToBeFree = totalBusyTime = 0;}
    double getTimeToBeFree() {return timeToBeFree;}
    double getTotalBusyTime() {return totalBusyTime;}
    bool isFree(double currentTime) {return currentTime - timeToBeFree >= 0;}
    void takeOrder(Customer* customer, double currentTime);
};

#endif /* Cashier_h */
