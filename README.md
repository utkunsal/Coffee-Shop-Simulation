# Coffee Shop Simulation

The Coffee Shop is available in two different models, each model with N cashiers and N/3 baristas. In the first model, there is one queue for both cashiers and baristas. For the second model, there is one queue for cashiers and N/3 queues for baristas. First 3 cashiers send the orders to the first barista and next 3 cashiers send the orders to the second barista and goes on like that. In the cashier queue it is first come first serve, while in the barista queue the most expensive coffee is served first. This design is valid for both models.

This program simulates these two models and get statistics given below:
1. Total running time (for all system)
2. Utilization (for both cashier and barista)(busy time of the unit / total running time of the coffee shop)
3. Turnaround time (for each order)(time when the coffee is done - arrival time of the customer)
4. Maximum length of each queue

# Input and Output

The input file has the following format:
1. First line is number of cashiers (N) which is always divisible by 3.
2. Second line is the number of orders (X).
3. Following X lines contain information about orders. There are 4 variables separated with space. They are:
• Arrival Time: The time when customer enters to the coffee shop (in seconds) 
• Order Time: The time required to give coffee order (in seconds)
• Brew Time: The time required to make coffee (in seconds)
• Price of Order: Price of the coffee.

The output file has the following format for both models:
1. First line is the total running time of the coffee shop (in seconds).
2. Next line is the maximum length of the cashier queue.
3. If the coffee shop has one barista queue, next line is maximum length of this queue. If the coffee shop has N/3 barista queues, next N/3 lines are maximum lengths of each barista queue.
4. Next N lines are the utilization of the cashiers.
5. Next N/3 lines are the utilization of the baristas.
6. Next X lines are turnaround times of orders.  
There is an empty line between the first model and the second model.

# Execution
    make
    ./CoffeeShopSimulation input.txt output.txt
