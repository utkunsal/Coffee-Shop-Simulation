#include "Barista.h"

// barista brews the given customers order, and sets customer's turnaround time.
void Barista::startBrewing(Customer* customer, double currentTime)
{
    timeToBeFree = currentTime + customer->getBrewTime();
    totalBusyTime += customer->getBrewTime();
    customer->setTurnaroudTime(timeToBeFree - customer->getArrivalTime());
    
}
