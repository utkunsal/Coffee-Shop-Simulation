#ifndef Customer_h
#define Customer_h

class Customer {
private:
    double arrivalTime;
    double orderTime;
    double brewTime;
    double priceOfOrder;
    double turnaroundTime;
    double timeAfterCashierTookOrder;
public:
    Customer() {}
    Customer(double arrivalTime, double orderTime, double brewTime, double priceOfOrder): arrivalTime(arrivalTime), orderTime(orderTime), brewTime(brewTime), priceOfOrder(priceOfOrder) {}
    double getArrivalTime() {return arrivalTime;}
    double getOrderTime() {return orderTime;}
    double getBrewTime() {return brewTime;}
    double getPriceOfOrder() {return priceOfOrder;}
    double getTurnaroundTime() {return turnaroundTime;}
    double getTimeAfterCashierTookOrder() {return timeAfterCashierTookOrder;}
    void setTurnaroudTime(double time) {turnaroundTime = time;}
    void setTimeAfterCashierTookOrder(double time) {timeAfterCashierTookOrder = time;}
};

#endif /* Customer_h */
