#ifndef Queue_h
#define Queue_h
#include "Customer.h"

struct Node{
    Customer* customer;
    struct Node* next;
};

class Queue {
private:
    struct Node* front = NULL;
    struct Node* rear = NULL;
    int size = 0;
public:
    Customer* getFront() {return front->customer;}
    int getSize() {return size;}
    void enqueue(Customer*& customer);
    void enqueueByPricePriority(Customer*& customer);
    void enqueueByTimeAfterCashierTookOrderPriority(Customer*& customer);
    void dequeue();
};

#endif /* Queue_h */
