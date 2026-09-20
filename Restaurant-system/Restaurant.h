#ifndef RESTAURANT_H
#define RESTAURANT_H

#include "Order.h"
#include <string>

// Manages the orders using a Doubly Linked List
class Restaurant {
private:
    Order* head;
    Order* tail;
    int nextOrderId;

public:
    /**
     * @brief Constructor
     * Purpose: Initializes the empty doubly linked list and sets initial Order ID.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(1)
     */
    Restaurant();

    /**
     * @brief Destructor
     * Purpose: Frees all allocated memory in the doubly linked list.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    ~Restaurant();

    /**
     * @brief addOrder
     * Purpose: Takes validated user input and adds a new order to the system. Auto-saves afterwards.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(1)
     */
    void addOrder();

    /**
     * @brief displayOrders
     * Purpose: Prints all current orders in a formatted table.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void displayOrders() const;

    /**
     * @brief searchOrder
     * Purpose: Finds and prints an order based on ID.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void searchOrder() const;

    /**
     * @brief updateOrder
     * Purpose: Prompts for new details to update an existing order by ID. Auto-saves afterwards.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void updateOrder();

    /**
     * @brief deleteOrder
     * Purpose: Removes an order by ID from the list. Auto-saves afterwards.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void deleteOrder();

    /**
     * @brief updateOrderStatus
     * Purpose: Modifies the status of a specific order. Auto-saves afterwards.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void updateOrderStatus();

    /**
     * @brief displayOrdersByStatus
     * Purpose: Displays all orders matching a specific status.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void displayOrdersByStatus() const;

    /**
     * @brief displayDashboard
     * Purpose: Computes metrics across all orders and displays a summary dashboard.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void displayDashboard() const;

    /**
     * @brief calculateTotalRevenue
     * Purpose: Sums up the total bill across all active orders and displays it.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void calculateTotalRevenue() const;

    // Phase 4 functionalities

    /**
     * @brief saveOrdersToFile
     * Purpose: Serializes the linked list into orders.txt.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void saveOrdersToFile() const;

    /**
     * @brief loadOrdersFromFile
     * Purpose: Reads orders.txt and reconstructs the linked list.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void loadOrdersFromFile();

    /**
     * @brief clearScreen
     * Purpose: Clears the console.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(1)
     */
    void clearScreen() const;

    /**
     * @brief pauseScreen
     * Purpose: Waits for user input before continuing.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(1)
     */
    void pauseScreen() const;

    /**
     * @brief printHeader
     * Purpose: Prints a consistent, decorated header for screens.
     * Parameters: const std::string& title - The text to display in the header
     * Return Value: None
     * Time Complexity: O(1)
     */
    void printHeader(const std::string& title) const;

    /**
     * @brief printFooter
     * Purpose: Prints a consistent bottom border.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(1)
     */
    void printFooter() const;

    /**
     * @brief aboutProject
     * Purpose: Displays the About Project information screen.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(1)
     */
    void aboutProject() const;

    /**
     * @brief displayExitSummary
     * Purpose: Prints the final end-of-day summary before terminating the app.
     * Parameters: None
     * Return Value: None
     * Time Complexity: O(n)
     */
    void displayExitSummary() const;

    // Web API support functions
    std::string getOrdersAsJson() const;
    Order* addOrderProgrammatic(const std::string& name, int table, const std::string& food, int qty, double price);
    bool updateOrderStatusProgrammatic(int id, const std::string& status);
    bool deleteOrderProgrammatic(int id);
};

#endif // RESTAURANT_H
