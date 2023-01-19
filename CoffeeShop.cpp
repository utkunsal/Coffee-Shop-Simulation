#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Cashier.h"
#include "Barista.h"
#include "Queue.h"
#include "Customer.h"
#include "CoffeeShop.h"
using namespace std;



// runs the first model of the coffee shop and writes its information to the output file.
void CoffeeShop::startFirstModel(string inputFileName, string outputFileName)
{
    ifstream input;
    input.open(inputFileName.c_str(), ios::in);
    Queue cashierQueue;
    Queue baristaQueue;
    int maxLengthOfCashierQueue = 0;
    int maxLengthOfBaristaQueue = 0;

    
    // initializing cashiers, baristas and numberOfOrders
    vector<Cashier> cashiers;
    vector<Barista> baristas;
    vector<Customer*> customers;
    int numberOfCashiers;
    int numberOfOrders;
    input >> numberOfCashiers;
    input >> numberOfOrders;
    for(int i = 0; i < numberOfCashiers; i++){
        Cashier cashier;
        cashiers.push_back(cashier);
    }
    for(int i = 0; i < numberOfCashiers/3; i++){
        Barista barista;
        baristas.push_back(barista);
    }
    

    // reading orders
    string line;
    while (getline(input, line)) {
        if (line[line.length()-1] == '\n' || line[line.length()-1] == '\r')
            line.erase(line.length()-1);
        if (line == "")
            continue;
        stringstream ss(line);
        string temp;
        double orderInfo[4];
        int i = 0;
        while (getline(ss, temp, ' ')) {
            orderInfo[i++] = stod(temp);
        }
        double currentTime = orderInfo[0];
        double orderTime = orderInfo[1];
        double brewTime = orderInfo[2];
        double priceOfOrder = orderInfo[3];
        bool orderTaken = false;
        
        Customer* newCustomer = new Customer(currentTime, orderTime, brewTime, priceOfOrder);
        customers.push_back(newCustomer);
        

        // if there are people in the queue, their order will be taken first.
        if (cashierQueue.getSize() > 0) {
            
            // to take orders from the queue until all cashiers are busy at the current time or the queue is empty.
            int indexOfWantedCashier = getIndexOfCashierWithTheSmallestTimeToBeFree(cashiers);
            while (cashiers[indexOfWantedCashier].getTimeToBeFree() <= currentTime && cashierQueue.getSize() > 0){
                Customer* customerFromQueue = cashierQueue.getFront();
                cashiers[indexOfWantedCashier].takeOrder(customerFromQueue, cashiers[indexOfWantedCashier].getTimeToBeFree());
                cashierQueue.dequeue();
                indexOfWantedCashier = getIndexOfCashierWithTheSmallestTimeToBeFree(cashiers);

            }
        }
        
        // if cashiers are not busy, the new order will be taken.
        if (cashierQueue.getSize() == 0) {
            for(int j = 0; j < numberOfCashiers; j++){
                if (cashiers[j].isFree(currentTime)){
                    cashiers[j].takeOrder(newCustomer, currentTime);
                    orderTaken = true;
                    break;
                }
            }
        }

        // if cashiers are busy, customer will go to queue.
        if (!orderTaken) {
            cashierQueue.enqueue(newCustomer);
        }
        
        // updating max length of the cashier queue
        if (cashierQueue.getSize() > maxLengthOfCashierQueue) {
            maxLengthOfCashierQueue = cashierQueue.getSize();
        }
        
    }
    input.close();
    
    
    // to take all orders from the queue when there are not any new customer coming.
    while (cashierQueue.getSize() > 0) {
        int indexOfWantedCashier = getIndexOfCashierWithTheSmallestTimeToBeFree(cashiers);
        Customer* customerFromQueue = cashierQueue.getFront();
        cashiers[indexOfWantedCashier].takeOrder(customerFromQueue, cashiers[indexOfWantedCashier].getTimeToBeFree());
        cashierQueue.dequeue();
    }
    
    
    // sorting customers according to the time after cashier took their order
    Queue afterCashierQueue;
    for(int i = 0; i<numberOfOrders; i++){
        afterCashierQueue.enqueueByTimeAfterCashierTookOrderPriority(customers[i]);
    }

    
    // moving orders to baristas or barista queue
    // using the afterCashierQueue which is sorted according to the times after cashier took their order
    while (afterCashierQueue.getSize() > 0) {
        
        Customer* customer = afterCashierQueue.getFront();
        
        if (baristaQueue.getSize() > 0){
            // to take orders from the queue until all baristas are busy at the current time or the queue is empty.
            int indexOfWantedBarista = getIndexOfBaristaWithTheSmallestTimeToBeFree(baristas);
            while (baristas[indexOfWantedBarista].getTimeToBeFree() <= customer->getTimeAfterCashierTookOrder() && baristaQueue.getSize() > 0){
                Customer* customerFromQueue = baristaQueue.getFront();
                baristas[indexOfWantedBarista].startBrewing(customerFromQueue, baristas[indexOfWantedBarista].getTimeToBeFree());
                baristaQueue.dequeue();
                indexOfWantedBarista = getIndexOfBaristaWithTheSmallestTimeToBeFree(baristas);
            }
        }
        
        // if there are free baristas when the cashier completed taking the order, they will start brewing the new order.
        bool orderTaken = false;
        if (baristaQueue.getSize() == 0) {
            for(int i = 0; i < baristas.size(); i++){
                if (baristas[i].isFree(customer->getTimeAfterCashierTookOrder())){
                    baristas[i].startBrewing(customer, customer->getTimeAfterCashierTookOrder());
                    orderTaken = true;
                    break;
                }
            }
        }
        
        // if baristas are busy, enqueues the new order for barista queue according to order price.
        if (!orderTaken){
            baristaQueue.enqueueByPricePriority(customer);
        }
        
        // updating max length of the barista queue
        if (baristaQueue.getSize() > maxLengthOfBaristaQueue) {
            maxLengthOfBaristaQueue = baristaQueue.getSize();
        }

        afterCashierQueue.dequeue();
        
    }
    
    
    
    // to brew all coffees from the barista queue when there are not any new customers.
    while (baristaQueue.getSize() > 0) {
        int indexOfWantedBarista = getIndexOfBaristaWithTheSmallestTimeToBeFree(baristas);
        Customer* customerFromQueue = baristaQueue.getFront();
        baristas[indexOfWantedBarista].startBrewing(customerFromQueue, baristas[indexOfWantedBarista].getTimeToBeFree());
        baristaQueue.dequeue();
    }
    
    
    // to find the total running time by finding the barista who completes their work last.
    double totalRunningTime = 0;
    for(int i = 0; i<baristas.size(); i++){
        if (baristas[i].getTimeToBeFree() > totalRunningTime) {
            totalRunningTime = baristas[i].getTimeToBeFree();
        }
    }
    
    
    // to write the output for the first model
    ofstream output;
    output.open(outputFileName.c_str(), ios::out);
    output << setprecision(2);
    output << fixed;
    output << totalRunningTime << "\n";
    output << maxLengthOfCashierQueue << "\n";
    output << maxLengthOfBaristaQueue << "\n";
    for(int i = 0; i<numberOfCashiers; i++){
        output << cashiers[i].getTotalBusyTime()/totalRunningTime << "\n";
    }
    for(int i = 0; i<baristas.size(); i++){
        output << baristas[i].getTotalBusyTime()/totalRunningTime << "\n";
    }
    for(int i = 0; i<numberOfOrders; i++){
        output << customers[i]->getTurnaroundTime() << "\n";
    }
    output << "\n";
    output.close();
    
    
    // to delete all customer pointers
    for (int i = 0; i<numberOfOrders; i++){
        delete customers[i];
    }
    
}



// runs the second model of the coffee shop and appends its information to the output file.
void CoffeeShop::startSecondModel(string inputFileName, string outputFileName)
{
    ifstream input;
    input.open(inputFileName.c_str(), ios::in);
    Queue cashierQueue;
    int maxLengthOfCashierQueue = 0;
    
    // initializing cashiers, baristas and numberOfOrders
    vector<Cashier> cashiers;
    vector<Barista> baristas;
    vector<Customer*> customers;
    vector<Queue> allBaristaQueues;
    vector<Queue> afterCashierQueues;
    int numberOfCashiers;
    int numberOfOrders;
    input >> numberOfCashiers;
    input >> numberOfOrders;
    for(int i = 0; i < numberOfCashiers; i++){
        Cashier cashier;
        cashiers.push_back(cashier);
    }
    for(int i = 0; i < numberOfCashiers/3; i++){
        Barista barista;
        baristas.push_back(barista);
        Queue baristaQueue;
        allBaristaQueues.push_back(baristaQueue);
        Queue afterCashierQueue;
        afterCashierQueues.push_back(afterCashierQueue);
    }
    vector<int> maxLengthsOfBaristaQueues(numberOfCashiers/3, 0);
    
    
    // reading new orders
    string line;
    while (getline(input, line)) {
        if (line[line.length()-1] == '\n' || line[line.length()-1] == '\r')
            line.erase(line.length()-1);
        if (line == "")
            continue;
        stringstream ss(line);
        string temp;
        double orderInfo[4];
        int i = 0;
        while (getline(ss, temp, ' ')) {
            orderInfo[i++] = stod(temp);
        }
        double currentTime = orderInfo[0];
        double orderTime = orderInfo[1];
        double brewTime = orderInfo[2];
        double priceOfOrder = orderInfo[3];
        bool orderTaken = false;
        
        Customer* newCustomer = new Customer(currentTime, orderTime, brewTime, priceOfOrder);
        customers.push_back(newCustomer);
        

        // if there are people in the queue, their order will be taken first.
        if (cashierQueue.getSize() > 0) {
            
            // to take orders from the queue until all cashiers are busy at the current time or the queue is empty.
            int indexOfWantedCashier = getIndexOfCashierWithTheSmallestTimeToBeFree(cashiers);
            while (cashiers[indexOfWantedCashier].getTimeToBeFree() <= currentTime && cashierQueue.getSize() > 0){
                Customer* customerFromQueue = cashierQueue.getFront();
                cashiers[indexOfWantedCashier].takeOrder(customerFromQueue, cashiers[indexOfWantedCashier].getTimeToBeFree());
                cashierQueue.dequeue();
                
                afterCashierQueues[indexOfWantedCashier/3].enqueueByTimeAfterCashierTookOrderPriority(customerFromQueue);
                
                indexOfWantedCashier = getIndexOfCashierWithTheSmallestTimeToBeFree(cashiers);

            }
        }
        
        // if cashiers are not busy, the new order will be taken.
        if (cashierQueue.getSize() == 0) {
            for(int j = 0; j < numberOfCashiers; j++){
                if (cashiers[j].isFree(currentTime)){
                    cashiers[j].takeOrder(newCustomer, currentTime);
                    orderTaken = true;
                    
                    afterCashierQueues[j/3].enqueueByTimeAfterCashierTookOrderPriority(newCustomer);
                    
                    break;
                }
            }
        }

        // if cashiers are busy, customer will go to queue.
        if (!orderTaken) {
            cashierQueue.enqueue(newCustomer);
        }
        
        // updating max length of the cashier queue
        if (cashierQueue.getSize() > maxLengthOfCashierQueue) {
            maxLengthOfCashierQueue = cashierQueue.getSize();
        }
        
    }
    input.close();
    

    
    
    // to take all orders from the queue when there are not any new customer coming.
    while (cashierQueue.getSize() > 0) {
        int indexOfWantedCashier = getIndexOfCashierWithTheSmallestTimeToBeFree(cashiers);
        Customer* customerFromQueue = cashierQueue.getFront();
        cashiers[indexOfWantedCashier].takeOrder(customerFromQueue, cashiers[indexOfWantedCashier].getTimeToBeFree());
        cashierQueue.dequeue();
        
        afterCashierQueues[indexOfWantedCashier/3].enqueueByTimeAfterCashierTookOrderPriority(customerFromQueue);
    }
    
    
    
    // moving orders to baristas or barista queue
    // using the afterCashierQueues which is sorted according to the times after cashier took their order
    for (int i = 0; i<afterCashierQueues.size(); i++){
                
        while (afterCashierQueues[i].getSize() > 0) {
            
            Customer* customer = afterCashierQueues[i].getFront();
            
            if (allBaristaQueues[i].getSize() > 0){
                // to take orders from the queue until the barista is busy at the current time or the queue is empty.
                while (baristas[i].getTimeToBeFree() <= customer->getTimeAfterCashierTookOrder() && allBaristaQueues[i].getSize() > 0){
                    Customer* customerFromQueue = allBaristaQueues[i].getFront();
                    baristas[i].startBrewing(customerFromQueue, baristas[i].getTimeToBeFree());
                    allBaristaQueues[i].dequeue();
                }
            }
            
            // if the barista is free when the cashier completed taking the order, they will start brewing the new order.
            bool orderTaken = false;
            if (allBaristaQueues[i].getSize() == 0) {
                if (baristas[i].isFree(customer->getTimeAfterCashierTookOrder())){
                    baristas[i].startBrewing(customer, customer->getTimeAfterCashierTookOrder());
                    orderTaken = true;
                }
            }
            
            // if the barista is busy, enqueues the new order for barista queue according to order price.
            if (!orderTaken){
                allBaristaQueues[i].enqueueByPricePriority(customer);
            }
            
            // updating max length of each barista queue
            if (allBaristaQueues[i].getSize() > maxLengthsOfBaristaQueues[i]) {
                maxLengthsOfBaristaQueues[i] = allBaristaQueues[i].getSize();
            }

            afterCashierQueues[i].dequeue();
            
        }
        
    }
    
    
    // to brew all coffees from the barista queues when there are not any new customers.
    for(int i = 0; i<allBaristaQueues.size(); i++){
        while (allBaristaQueues[i].getSize() > 0) {
            Customer* customerFromQueue = allBaristaQueues[i].getFront();
            baristas[i].startBrewing(customerFromQueue, baristas[i].getTimeToBeFree());
            allBaristaQueues[i].dequeue();
        }
    }
    
    
    // to find the total running time by finding the barista who completes their work last.
    double totalRunningTime = 0;
    for(int i = 0; i<baristas.size(); i++){
        if (baristas[i].getTimeToBeFree() > totalRunningTime) {
            totalRunningTime = baristas[i].getTimeToBeFree();
        }
    }
    
    
    // to write the output for the second model
    ofstream output;
    output.open(outputFileName.c_str(), ios::app);
    output << setprecision(2);
    output << fixed;
    output << totalRunningTime << "\n";
    output << maxLengthOfCashierQueue << "\n";
    for (int i = 0; i<allBaristaQueues.size(); i++){
        output << maxLengthsOfBaristaQueues[i] << "\n";
    }
    for(int i = 0; i<numberOfCashiers; i++){
        output << cashiers[i].getTotalBusyTime()/totalRunningTime << "\n";
    }
    for(int i = 0; i<baristas.size(); i++){
        output << baristas[i].getTotalBusyTime()/totalRunningTime << "\n";
    }
    for(int i = 0; i<numberOfOrders; i++){
        output << customers[i]->getTurnaroundTime() << "\n";
    }
    output << "\n";
    output.close();
    
    
    // to delete all customer pointers
    for (int i = 0; i<numberOfOrders; i++){
        delete customers[i];
    }

}



// returns the index of cashier with the smallest time to be free.
int CoffeeShop::getIndexOfCashierWithTheSmallestTimeToBeFree(vector<Cashier> cashiers)
{
    int i = 0;
    for(int j = 1; j<cashiers.size(); j++){
        if (cashiers[j].getTimeToBeFree() < cashiers[i].getTimeToBeFree())
            i = j;
    }
    return i;
}

// returns the index of barista with the smallest time to be free.
int CoffeeShop::getIndexOfBaristaWithTheSmallestTimeToBeFree(vector<Barista> baristas)
{
    int j = 0;
    for(int i = 1; i<baristas.size(); i++){
        if (baristas[i].getTimeToBeFree() < baristas[j].getTimeToBeFree())
            j = i;
    }
    return j;
}

