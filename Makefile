CoffeeShopSimulation: main CoffeeShop Cashier Barista Queue
	g++ -std=c++11 main.o CoffeeShop.o Cashier.o Barista.o Queue.o -o CoffeeShopSimulation
main: main.cpp
	g++ -std=c++11 -c main.cpp -o main.o
CoffeeShop: 
	g++ -std=c++11 -c CoffeeShop.cpp -o CoffeeShop.o
Cashier: 
	g++ -std=c++11 -c Cashier.cpp -o Cashier.o
Barista: 
	g++ -std=c++11 -c Barista.cpp -o Barista.o
Queue: 
	g++ -std=c++11 -c Queue.cpp -o Queue.o
