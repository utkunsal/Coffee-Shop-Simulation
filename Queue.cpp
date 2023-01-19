#include <stdio.h>
#include "Queue.h"
#include "Customer.h"

// enqueues a new customer to the queue.
void Queue::enqueue(Customer*& customer)
{
    struct Node* newNode = new struct Node;
    newNode->customer = customer;
    newNode->next = NULL;
    
    if (front == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    size++;
}

// dequeues the first customer from the queue.
void Queue::dequeue()
{
    if (front == NULL)
        return;
    
    struct Node* temp = front;
    front = front->next;
    delete temp;
    size--;
}

// enqueues a new customer according to their order price.
// customer with the highest order price will be first in the queue.
void Queue::enqueueByPricePriority(Customer*& customer)
{
    struct Node* newNode = new struct Node;
    newNode->customer = customer;
    newNode->next = NULL;


    if (front == NULL) {
        front = rear = newNode;
    } else {
        struct Node* node = front;
        struct Node* prevNode = NULL;
        while (node != NULL) {
            if (node->customer->getPriceOfOrder() < customer->getPriceOfOrder()) {
                newNode->next = node;
                if (prevNode == NULL) {
                    // if the new order is the most expensive
                    newNode->next = front;
                    front = newNode;
                } else {
                    // if the new order's price is somewhere in between
                    prevNode->next = newNode;
                }
                break;
            }
            prevNode = node;
            node = node->next;
        }
        if (node == NULL) {
            // if the new order is the cheapest
            prevNode->next = newNode;
            rear = newNode;
        }
    }
    size++;
}

// enqueues a new customer according to the time when cashier completed taking their order.
// customer with the highest time will be last in the queue.
void Queue::enqueueByTimeAfterCashierTookOrderPriority(Customer*& customer)
{
    struct Node* newNode = new struct Node;
    newNode->customer = customer;
    newNode->next = NULL;


    if (front == NULL) {
        front = rear = newNode;
    } else {
        struct Node* node = front;
        struct Node* prevNode = NULL;
        while (node != NULL) {
            if (node->customer->getTimeAfterCashierTookOrder() > customer->getTimeAfterCashierTookOrder()) {
                newNode->next = node;
                if (prevNode == NULL) {
                    // if the customer is the fastest
                    newNode->next = front;
                    front = newNode;
                } else {
                    // if the customer's time is somewhere in between
                    prevNode->next = newNode;
                }
                break;
            }
            prevNode = node;
            node = node->next;
        }
        if (node == NULL) {
            // if the customer is the slowest
            prevNode->next = newNode;
            rear = newNode;
        }
    }
    size++;
}


