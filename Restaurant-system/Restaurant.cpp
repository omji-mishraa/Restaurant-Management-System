#include "Restaurant.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>

Restaurant::Restaurant() : head(nullptr), tail(nullptr), nextOrderId(1001) {}

Restaurant::~Restaurant() {
    Order* current = head;
    while (current != nullptr) {
        Order* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

// Phase 4 UI Functions

void Restaurant::clearScreen() const {
    std::system("cls"); // For Windows
}

void Restaurant::pauseScreen() const {
    std::system("pause");
}

void Restaurant::printHeader(const std::string& title) const {
    std::cout << "\n====================================================\n";
    std::cout << " " << title << "\n";
    std::cout << "====================================================\n\n";
}

void Restaurant::printFooter() const {
    std::cout << "\n====================================================\n";
}

// Phase 4 File Persistence

void Restaurant::saveOrdersToFile() const {
    std::ofstream outFile("orders.txt");
    if (!outFile) {
        std::cerr << "Error: Could not open orders.txt for saving.\n";
        return;
    }
    Order* current = head;
    while (current != nullptr) {
        outFile << current->orderId << ","
                << current->customerName << ","
                << current->tableNumber << ","
                << current->foodName << ","
                << current->quantity << ","
                << current->price << ","
                << current->totalBill << ","
                << current->status << "\n";
        current = current->next;
    }
    outFile.close();
}

void Restaurant::loadOrdersFromFile() {
    std::ifstream inFile("orders.txt");
    if (!inFile) {
        // File doesn't exist yet, just start fresh.
        return;
    }
    
    std::string line;
    int maxId = 1000;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string token;
        
        int id;
        std::string cName, fName, status;
        int qty = 0;
        int tableNumber = 0;
        double price = 0.0, bill = 0.0;
        
        if (!std::getline(ss, token, ',')) continue;
        id = std::stoi(token);
        if (!std::getline(ss, cName, ',')) continue;
        
        std::string thirdToken;
        if (!std::getline(ss, thirdToken, ',')) continue;
        
        // Detect if the third token is tableNumber (numeric) or foodName (alphabetic/text)
        bool isTableNumeric = !thirdToken.empty();
        for (char c : thirdToken) {
            if (!std::isdigit(c)) {
                isTableNumeric = false;
                break;
            }
        }
        
        if (isTableNumeric) {
            tableNumber = std::stoi(thirdToken);
            if (!std::getline(ss, fName, ',')) continue;
        } else {
            tableNumber = 0; // Default if not present
            fName = thirdToken;
        }
        
        if (!std::getline(ss, token, ',')) continue;
        qty = std::stoi(token);
        if (!std::getline(ss, token, ',')) continue;
        price = std::stod(token);
        if (!std::getline(ss, token, ',')) continue;
        bill = std::stod(token);
        if (!std::getline(ss, status, ',')) continue;
        
        // Trim any potential trailing carriage return from status (common on Windows line endings)
        if (!status.empty() && status.back() == '\r') {
            status.pop_back();
        }
        
        Order* newOrder = new Order(id, cName, tableNumber, fName, qty, price);
        newOrder->totalBill = bill;
        newOrder->status = status;
        
        if (head == nullptr) {
            head = newOrder;
            tail = newOrder;
        } else {
            tail->next = newOrder;
            newOrder->prev = tail;
            tail = newOrder;
        }
        if (id > maxId) {
            maxId = id;
        }
    }
    inFile.close();
    nextOrderId = maxId + 1;
}

// Business Logic with UI and Validation

void Restaurant::addOrder() {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - ADD ORDER");
    
    std::string customerName, foodName;
    int quantity;
    double price;

    int tableNumber;

    while (true) {
        std::cout << "Enter Customer Name: ";
        std::getline(std::cin >> std::ws, customerName);
        if (!customerName.empty()) break;
        std::cout << "Name cannot be empty. Please try again.\n";
    }

    while (true) {
        std::cout << "Enter Table Number: ";
        if (std::cin >> tableNumber && tableNumber > 0) break;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Table number must be a positive integer. Please try again.\n";
    }

    while (true) {
        std::cout << "Enter Food Name: ";
        std::getline(std::cin >> std::ws, foodName);
        if (!foodName.empty()) break;
        std::cout << "Food name cannot be empty. Please try again.\n";
    }

    while (true) {
        std::cout << "Enter Quantity: ";
        if (std::cin >> quantity && quantity > 0) break;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Quantity must be a positive integer. Please try again.\n";
    }

    while (true) {
        std::cout << "Enter Price per item: ";
        if (std::cin >> price && price > 0) break;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Price must be a positive number. Please try again.\n";
    }

    Order* newOrder = new Order(nextOrderId++, customerName, tableNumber, foodName, quantity, price);

    if (head == nullptr) {
        head = newOrder;
        tail = newOrder;
    } else {
        tail->next = newOrder;
        newOrder->prev = tail;
        tail = newOrder;
    }
    
    saveOrdersToFile(); // Auto save
    
    std::cout << "\nOrder added successfully! Order ID: " << newOrder->orderId << "\n";
    printFooter();
    pauseScreen();
}

void Restaurant::displayOrders() const {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - DISPLAY ORDERS");
    
    if (head == nullptr) {
        std::cout << "No orders exist in the system.\n";
        printFooter();
        pauseScreen();
        return;
    }

    std::cout << std::left 
              << std::setw(10) << "Order ID" 
              << std::setw(20) << "Customer" 
              << std::setw(20) << "Food" 
              << std::setw(10) << "Qty" 
              << std::setw(15) << "Price" 
              << std::setw(15) << "Bill" 
              << "Status\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";

    Order* current = head;
    while (current != nullptr) {
        std::cout << std::left 
                  << std::setw(10) << current->orderId 
                  << std::setw(20) << current->customerName 
                  << std::setw(20) << current->foodName 
                  << std::setw(10) << current->quantity 
                  << "Rs. " << std::setw(12) << std::fixed << std::setprecision(2) << current->price 
                  << "Rs. " << std::setw(12) << std::fixed << std::setprecision(2) << current->totalBill 
                  << current->status << "\n";
        current = current->next;
    }
    printFooter();
    pauseScreen();
}

void Restaurant::searchOrder() const {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - SEARCH ORDER");
    
    int searchId;
    std::cout << "Enter Order ID to search: ";
    if (!(std::cin >> searchId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Returning to menu.\n";
        pauseScreen();
        return;
    }

    Order* current = head;
    while (current != nullptr) {
        if (current->orderId == searchId) {
            std::cout << "\nOrder Found:\n";
            std::cout << "Order ID     : " << current->orderId << "\n";
            std::cout << "Customer Name: " << current->customerName << "\n";
            std::cout << "Food Name    : " << current->foodName << "\n";
            std::cout << "Quantity     : " << current->quantity << "\n";
            std::cout << "Price        : Rs. " << std::fixed << std::setprecision(2) << current->price << "\n";
            std::cout << "Total Bill   : Rs. " << std::fixed << std::setprecision(2) << current->totalBill << "\n";
            std::cout << "Status       : " << current->status << "\n";
            printFooter();
            pauseScreen();
            return;
        }
        current = current->next;
    }
    std::cout << "\nOrder Not Found.\n";
    printFooter();
    pauseScreen();
}

void Restaurant::updateOrder() {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - UPDATE ORDER");
    
    int updateId;
    std::cout << "Enter Order ID to update: ";
    if (!(std::cin >> updateId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        pauseScreen();
        return;
    }

    Order* current = head;
    while (current != nullptr) {
        if (current->orderId == updateId) {
            std::cout << "\nOrder Found. Enter new details:\n";
            
            while (true) {
                std::cout << "Enter Customer Name: ";
                std::getline(std::cin >> std::ws, current->customerName);
                if (!current->customerName.empty()) break;
                std::cout << "Name cannot be empty.\n";
            }

            while (true) {
                std::cout << "Enter Food Name: ";
                std::getline(std::cin >> std::ws, current->foodName);
                if (!current->foodName.empty()) break;
                std::cout << "Food name cannot be empty.\n";
            }

            while (true) {
                std::cout << "Enter Quantity: ";
                if (std::cin >> current->quantity && current->quantity > 0) break;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Quantity must be > 0.\n";
            }

            while (true) {
                std::cout << "Enter Price per item: ";
                if (std::cin >> current->price && current->price > 0) break;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Price must be > 0.\n";
            }

            current->totalBill = current->quantity * current->price;

            saveOrdersToFile();

            std::cout << "\nOrder updated successfully!\n";
            printFooter();
            pauseScreen();
            return;
        }
        current = current->next;
    }
    std::cout << "\nOrder Not Found.\n";
    printFooter();
    pauseScreen();
}

void Restaurant::deleteOrder() {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - DELETE ORDER");
    
    int deleteId;
    std::cout << "Enter Order ID to delete: ";
    if (!(std::cin >> deleteId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        pauseScreen();
        return;
    }

    Order* current = head;
    while (current != nullptr) {
        if (current->orderId == deleteId) {
            if (current == head && current == tail) {
                head = nullptr;
                tail = nullptr;
            } else if (current == head) {
                head = head->next;
                head->prev = nullptr;
            } else if (current == tail) {
                tail = tail->prev;
                tail->next = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }

            delete current;
            saveOrdersToFile();
            
            std::cout << "\nOrder deleted successfully!\n";
            printFooter();
            pauseScreen();
            return;
        }
        current = current->next;
    }
    std::cout << "\nOrder Not Found.\n";
    printFooter();
    pauseScreen();
}

void Restaurant::calculateTotalRevenue() const {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - REVENUE");
    
    if (head == nullptr) {
        std::cout << "No Orders Available.\n";
        printFooter();
        pauseScreen();
        return;
    }

    int totalOrders = 0;
    double totalRevenue = 0.0;
    Order* current = head;
    
    while (current != nullptr) {
        totalOrders++;
        totalRevenue += current->totalBill;
        current = current->next;
    }

    std::cout << "Today's Total Revenue\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Total Orders  : " << totalOrders << "\n";
    std::cout << "Total Revenue : Rs. " << std::fixed << std::setprecision(2) << totalRevenue << "\n";
    printFooter();
    pauseScreen();
}

void Restaurant::updateOrderStatus() {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - UPDATE STATUS");
    
    int updateId;
    std::cout << "Enter Order ID to update status: ";
    if (!(std::cin >> updateId)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        pauseScreen();
        return;
    }

    Order* current = head;
    while (current != nullptr) {
        if (current->orderId == updateId) {
            std::cout << "\nSelect Status\n";
            std::cout << "1 Pending\n";
            std::cout << "2 Preparing\n";
            std::cout << "3 Ready\n";
            std::cout << "4 Served\n";
            std::cout << "5 Completed\n";
            std::cout << "6 Cancelled\n";
            
            int statusChoice;
            while (true) {
                std::cout << "Enter choice (1-6): ";
                if (std::cin >> statusChoice && statusChoice >= 1 && statusChoice <= 6) break;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice. Please enter a number between 1 and 6.\n";
            }

            switch (statusChoice) {
                case 1: current->status = "Pending"; break;
                case 2: current->status = "Preparing"; break;
                case 3: current->status = "Ready"; break;
                case 4: current->status = "Served"; break;
                case 5: current->status = "Completed"; break;
                case 6: current->status = "Cancelled"; break;
            }
            
            saveOrdersToFile();
            
            std::cout << "\nOrder status updated to " << current->status << " successfully!\n";
            printFooter();
            pauseScreen();
            return;
        }
        current = current->next;
    }
    std::cout << "\nOrder Not Found.\n";
    printFooter();
    pauseScreen();
}

void Restaurant::displayDashboard() const {
    clearScreen();
    printHeader("SMART RESTAURANT DASHBOARD");
    
    int totalOrders = 0;
    int pending = 0, preparing = 0, ready = 0, served = 0, completed = 0, cancelled = 0;
    double todaysRevenue = 0.0;

    Order* current = head;
    while (current != nullptr) {
        totalOrders++;
        todaysRevenue += current->totalBill;
        
        if (current->status == "Pending") pending++;
        else if (current->status == "Preparing") preparing++;
        else if (current->status == "Ready") ready++;
        else if (current->status == "Served") served++;
        else if (current->status == "Completed") completed++;
        else if (current->status == "Cancelled") cancelled++;
        
        current = current->next;
    }

    std::cout << std::left << std::setw(20) << "Total Orders" << ": " << totalOrders << "\n";
    std::cout << std::left << std::setw(20) << "Pending" << ": " << pending << "\n";
    std::cout << std::left << std::setw(20) << "Preparing" << ": " << preparing << "\n";
    std::cout << std::left << std::setw(20) << "Ready" << ": " << ready << "\n";
    std::cout << std::left << std::setw(20) << "Served" << ": " << served << "\n";
    std::cout << std::left << std::setw(20) << "Completed" << ": " << completed << "\n";
    std::cout << std::left << std::setw(20) << "Cancelled" << ": " << cancelled << "\n\n";
    std::cout << std::left << std::setw(20) << "Today's Revenue" << ": Rs. " << std::fixed << std::setprecision(2) << todaysRevenue << "\n";
    printFooter();
    pauseScreen();
}

void Restaurant::displayOrdersByStatus() const {
    clearScreen();
    printHeader("SMART RESTAURANT MANAGEMENT SYSTEM - STATUS FILTER");
    
    std::cout << "Select Status to Filter\n";
    std::cout << "1 Pending\n";
    std::cout << "2 Preparing\n";
    std::cout << "3 Ready\n";
    std::cout << "4 Served\n";
    std::cout << "5 Completed\n";
    std::cout << "6 Cancelled\n";
    
    int choice;
    while (true) {
        std::cout << "Enter choice (1-6): ";
        if (std::cin >> choice && choice >= 1 && choice <= 6) break;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Please enter a number between 1 and 6.\n";
    }

    std::string targetStatus = "";
    switch (choice) {
        case 1: targetStatus = "Pending"; break;
        case 2: targetStatus = "Preparing"; break;
        case 3: targetStatus = "Ready"; break;
        case 4: targetStatus = "Served"; break;
        case 5: targetStatus = "Completed"; break;
        case 6: targetStatus = "Cancelled"; break;
    }

    bool found = false;
    Order* current = head;
    
    std::cout << "\n" << std::left 
              << std::setw(10) << "Order ID" 
              << std::setw(20) << "Customer" 
              << std::setw(20) << "Food" 
              << std::setw(10) << "Qty" 
              << std::setw(15) << "Price" 
              << std::setw(15) << "Bill" 
              << "Status\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";

    while (current != nullptr) {
        if (current->status == targetStatus) {
            found = true;
            std::cout << std::left 
                      << std::setw(10) << current->orderId 
                      << std::setw(20) << current->customerName 
                      << std::setw(20) << current->foodName 
                      << std::setw(10) << current->quantity 
                      << "Rs. " << std::setw(12) << std::fixed << std::setprecision(2) << current->price 
                      << "Rs. " << std::setw(12) << std::fixed << std::setprecision(2) << current->totalBill 
                      << current->status << "\n";
        }
        current = current->next;
    }

    if (!found) {
        std::cout << "No Orders Found for status: " << targetStatus << "\n";
    }
    printFooter();
    pauseScreen();
}

void Restaurant::aboutProject() const {
    clearScreen();
    printHeader("ABOUT PROJECT");
    std::cout << "Project Name  : Smart Restaurant Management System\n";
    std::cout << "Developer     : OMJI MISHRA\n";
    std::cout << "Language      : C++17\n";
    std::cout << "Data Structure: Doubly Linked List\n";
    std::cout << "Version       : 4.0\n";
    std::cout << "Description   : A complete, robust, scalable application managing restaurant orders,\n";
    std::cout << "                sales, and status flows in memory with automatic file persistence.\n";
    std::cout << "Future Scope  : Login System, MySQL Database Integration, Analytics Dashboard,\n";
    std::cout << "                Payment Gateway Integration, GUI.\n";
    printFooter();
    pauseScreen();
}

void Restaurant::displayExitSummary() const {
    clearScreen();
    
    int totalOrders = 0;
    int completedOrders = 0;
    int cancelledOrders = 0;
    double todaysRevenue = 0.0;

    Order* current = head;
    while (current != nullptr) {
        totalOrders++;
        todaysRevenue += current->totalBill;
        if (current->status == "Completed") completedOrders++;
        if (current->status == "Cancelled") cancelledOrders++;
        current = current->next;
    }

    std::cout << "\n========================================\n";
    std::cout << "               Thank You\n\n";
    std::cout << "           Today's Summary\n";
    std::cout << "----------------------------------------\n";
    std::cout << std::left << std::setw(20) << "Total Orders" << ": " << totalOrders << "\n";
    std::cout << std::left << std::setw(20) << "Completed Orders" << ": " << completedOrders << "\n";
    std::cout << std::left << std::setw(20) << "Cancelled Orders" << ": " << cancelledOrders << "\n";
    std::cout << std::left << std::setw(20) << "Revenue" << ": Rs. " << std::fixed << std::setprecision(2) << todaysRevenue << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << "      Data Saved Successfully\n";
    std::cout << "========================================\n\n";
}

std::string Restaurant::getOrdersAsJson() const {
    std::stringstream ss;
    ss << "[";
    Order* current = head;
    bool first = true;
    while (current != nullptr) {
        if (!first) ss << ",";
        
        // Escape quotes in strings just in case
        std::string safeName = "";
        for (char c : current->customerName) {
            if (c == '"') safeName += "\\\"";
            else if (c == '\\') safeName += "\\\\";
            else safeName += c;
        }
        std::string safeFood = "";
        for (char c : current->foodName) {
            if (c == '"') safeFood += "\\\"";
            else if (c == '\\') safeFood += "\\\\";
            else safeFood += c;
        }

        ss << "{"
           << "\"id\":" << current->orderId << ","
           << "\"name\":\"" << safeName << "\","
           << "\"table\":" << current->tableNumber << ","
           << "\"food\":\"" << safeFood << "\","
           << "\"qty\":" << current->quantity << ","
           << "\"price\":" << current->price << ","
           << "\"bill\":" << current->totalBill << ","
           << "\"status\":\"" << current->status << "\""
           << "}";
        first = false;
        current = current->next;
    }
    ss << "]";
    return ss.str();
}

Order* Restaurant::addOrderProgrammatic(const std::string& name, int table, const std::string& food, int qty, double price) {
    Order* newOrder = new Order(nextOrderId++, name, table, food, qty, price);
    if (head == nullptr) {
        head = newOrder;
        tail = newOrder;
    } else {
        tail->next = newOrder;
        newOrder->prev = tail;
        tail = newOrder;
    }
    saveOrdersToFile();
    return newOrder;
}

bool Restaurant::updateOrderStatusProgrammatic(int id, const std::string& status) {
    Order* current = head;
    while (current != nullptr) {
        if (current->orderId == id) {
            current->status = status;
            saveOrdersToFile();
            return true;
        }
        current = current->next;
    }
    return false;
}

bool Restaurant::deleteOrderProgrammatic(int id) {
    Order* current = head;
    while (current != nullptr) {
        if (current->orderId == id) {
            if (current == head && current == tail) {
                head = tail = nullptr;
            } else if (current == head) {
                head = head->next;
                head->prev = nullptr;
            } else if (current == tail) {
                tail = tail->prev;
                tail->next = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
            saveOrdersToFile();
            return true;
        }
        current = current->next;
    }
    return false;
}
