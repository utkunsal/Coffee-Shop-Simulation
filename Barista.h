#ifndef Barista_h
#define Barista_h
#include "Customer.h"

class Barista {
private:
    double timeToBeFree;
    double totalBusyTime;
public:
    Barista() {timeToBeFree = totalBusyTime = 0;}
    double getTimeToBeFree() {return timeToBeFree;}
    double getTotalBusyTime() {return totalBusyTime;}
    bool isFree(double currentTime) {return currentTime - timeToBeFree >= 0;}
    void startBrewing(Customer* customer, double currentTime);
};

#endif /* Barista_h */
