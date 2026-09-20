#ifndef ORDER_H
#define ORDER_H

#include <string>

// Represents a single order in the restaurant
class Order {
public:
    int orderId;
    std::string customerName;
    int tableNumber;
    std::string foodName;
    int quantity;
    double price;
    double totalBill;
    std::string status;

    // Pointers for Doubly Linked List
    Order* prev;
    Order* next;

    // Constructor to initialize an order
    Order(int id, const std::string& cName, int tNum, const std::string& fName, int qty, double prc);
};

#endif // ORDER_H
