#include "Order.h"

Order::Order(int id, const std::string& cName, int tNum, const std::string& fName, int qty, double prc)
    : orderId(id), customerName(cName), tableNumber(tNum), foodName(fName), quantity(qty), price(prc),
      prev(nullptr), next(nullptr) {
    // Automatically calculate totalBill
    totalBill = quantity * price;
    // Initialize status as "Pending"
    status = "Pending";
}
