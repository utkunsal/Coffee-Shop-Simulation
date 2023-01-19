#include "Cashier.h"

// cashier takes the order of the given customer,
// and records the time when customer left from the cashier to customer's timeAfterCashierTookOrder attribute.
void Cashier::takeOrder(Customer* customer, double currentTime)
{
    timeToBeFree = currentTime + customer->getOrderTime();
    totalBusyTime += customer->getOrderTime();
    customer->setTimeAfterCashierTookOrder(timeToBeFree);
    
}

