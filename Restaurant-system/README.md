# Smart Restaurant Management System

## Table of Contents
1. [Project Overview](#project-overview)
2. [Problem Statement](#problem-statement)
3. [Project Objectives](#project-objectives)
4. [Technologies Used](#technologies-used)
5. [Data Structure Used](#data-structure-used)
6. [Why Doubly Linked List was Chosen](#why-doubly-linked-list-was-chosen)
7. [OOP Concepts Used](#oop-concepts-used)
8. [Features Implemented](#features-implemented)
9. [Project Architecture](#project-architecture)
10. [Folder Structure](#folder-structure)
11. [Module Explanations](#module-explanations)
    - [Source Files](#source-files)
    - [Classes](#classes)
    - [Important Functions](#important-functions)
    - [Inter-module Communication](#inter-module-communication)
12. [Program Workflow](#program-workflow)
13. [Menu Flow](#menu-flow)
14. [Data Structures Deep Dive](#data-structures-deep-dive)
    - [Node Structure Explanation](#node-structure-explanation)
    - [Doubly Linked List Working](#doubly-linked-list-working)
    - [Memory Management Explanation](#memory-management-explanation)
15. [System Logics](#system-logics)
    - [CRUD Operation Flow](#crud-operation-flow)
    - [Revenue Calculation Logic](#revenue-calculation-logic)
    - [Order Status Management Logic](#order-status-management-logic)
    - [File Handling Logic](#file-handling-logic)
    - [Input Validation Logic](#input-validation-logic)
    - [Error Handling Strategy](#error-handling-strategy)
16. [Complexity Analysis](#complexity-analysis)
    - [Time Complexity](#time-complexity)
    - [Space Complexity](#space-complexity)
17. [Evaluation](#evaluation)
    - [Advantages](#advantages-of-the-current-implementation)
    - [Limitations](#limitations-of-the-current-implementation)
18. [Future Vision](#future-vision)
    - [Future Scope](#future-scope)
    - [Scalability Plan](#scalability-plan)
19. [Sample Console Output](#sample-console-output)
20. [Getting Started](#getting-started)
    - [Installation Guide](#installation-guide)
    - [Compilation Instructions](#compilation-instructions)
    - [Run Instructions](#run-instructions)
    - [Example Usage](#example-usage)
21. [Screenshots](#screenshots)
22. [Project Specifics](#project-specifics)
    - [Dependencies](#project-dependencies)
    - [Coding Standards Followed](#coding-standards-followed)
    - [Design Decisions](#design-decisions)
    - [Assumptions](#assumptions)
    - [Known Issues](#known-issues)
23. [Conclusion](#conclusion)

---

## Project Overview
The **Smart Restaurant Management System** is a robust, console-based application designed to digitalize and streamline restaurant operations. Developed in C++17, the system handles order management, status tracking, revenue calculation, and daily summaries. Instead of relying on a database in its current iteration, it utilizes a custom-built Doubly Linked List for in-memory data management combined with automatic file persistence, ensuring that no data is lost between sessions.

## Problem Statement
Traditional restaurants often rely on paper-based ticketing systems or cumbersome spreadsheet software. This leads to issues like misplaced orders, difficulty in calculating end-of-day revenue, tracking order statuses (e.g., Pending, Preparing, Ready), and poor historical data management. A lightweight, efficient, and reliable digital system is required to handle these CRUD operations seamlessly while running on minimal hardware resources.

## Project Objectives
- **Digitalize Order Management:** Allow staff to add, update, delete, and search orders rapidly.
- **Track Order Lifecycle:** Move orders through various statuses (Pending -> Preparing -> Ready -> Served -> Completed).
- **Automate Calculations:** Calculate total bills per order and aggregate daily revenue instantly.
- **Ensure Data Persistence:** Automatically save and load orders to/from a local text file to survive application restarts.
- **Provide a User-Friendly Interface:** Offer an intuitive, clean console UI with clear headers, dashboards, and input validations.

## Technologies Used
- **Programming Language:** C++17
- **Environment:** Console/Terminal
- **Build System:** CMake (Optional, as indicated by `CMakeLists.txt`)

## Data Structure Used
The primary data structure driving this application is the **Doubly Linked List (DLL)**. Every order added to the system is represented as a node within this list.

## Why Doubly Linked List was Chosen
1. **Dynamic Size:** Unlike arrays, a linked list can grow and shrink dynamically as orders are placed and canceled. There is no hard limit on the number of daily orders (bounded only by heap memory).
2. **O(1) Tail Insertions:** By maintaining a `tail` pointer, appending a new order is extremely fast.
3. **Efficient Deletions:** Deleting an order from the middle of the list is an O(1) pointer operation once the node is found, which is faster and cleaner than shifting elements in an array or vector.
4. **Bidirectional Traversal:** Although the current implementation primarily traverses forward, the `prev` pointer allows for easy expansion in the future (e.g., displaying orders in reverse chronological order, or undoing operations).

## OOP Concepts Used
1. **Encapsulation:** The system uses classes (`Order` and `Restaurant`) to bundle data (variables) and methods (functions) together. The `Restaurant` class hides its `head`, `tail`, and `nextOrderId` from the outside world (private access modifiers).
2. **Abstraction:** Complex operations like `saveOrdersToFile()` or `addOrder()` are abstracted away. The `main.cpp` merely calls these public functions without needing to know *how* the file serialization or pointer manipulation works.

---

## Features Implemented
- **Order Creation (Add Order):** Records customer name, food, quantity, and price. Automatically generates a unique Order ID and calculates the total bill.
- **Order Searching:** Find any order instantly using its unique Order ID.
- **Order Modification (Update Order):** Correct mistakes by updating names, items, quantities, or prices.
- **Order Cancellation (Delete Order):** Remove an order entirely from the system.
- **Live Display:** View all active orders formatted in a clean, tabulated structure.
- **Status Tracking:** Update an order's status through 6 stages (Pending, Preparing, Ready, Served, Completed, Cancelled).
- **Status Filtering:** View only orders that match a specific status (e.g., show all "Pending" orders).
- **Dashboard:** A real-time overview showing counts of orders in each status and the total daily revenue.
- **Automatic File Persistence:** Data is automatically written to `orders.txt` upon every change and loaded back upon application startup.
- **Input Validation:** Prevents crashes and bad data by enforcing strict type checking and range limits on user inputs.

---

## Project Architecture

```text
+-------------------+       +-----------------------+       +-------------------+
|                   |       |                       |       |                   |
|     main.cpp      | ----> |     Restaurant        | ----> |       Order       |
|  (User Interface &|       |   (Business Logic &   |       |   (Data Node)     |
|    Menu Loop)     |       |    List Manager)      |       |                   |
|                   |       |                       |       |                   |
+-------------------+       +-----------+-----------+       +-------------------+
                                        |
                                        v
                            +-----------------------+
                            |                       |
                            |      orders.txt       |
                            |   (Persistent Data)   |
                            |                       |
                            +-----------------------+
```

## Folder Structure

```text
Restaurant-system/
│
├── CMakeLists.txt        # Build configuration for CMake
├── main.cpp              # Entry point of the application
├── Order.h               # Header file defining the Order node class
├── Order.cpp             # Implementation of the Order constructor
├── Restaurant.h          # Header file defining the list manager and logic
├── Restaurant.cpp        # Implementation of the core business logic
└── build/                # Directory containing compiled binaries
```

---

## Module Explanations

### Source Files
1. **`Order.h` & `Order.cpp`:** Define the properties of a single order. They represent the "Node" in our Doubly Linked List.
2. **`Restaurant.h` & `Restaurant.cpp`:** Contain the `Restaurant` class which acts as the system controller. It manages the Doubly Linked List, File I/O, UI rendering, and business calculations.
3. **`main.cpp`:** The driver code. It instantiates the `Restaurant` class, loads data, and runs the infinite menu loop.

### Classes
1. **`Order`:** 
   - Fields: `orderId`, `customerName`, `foodName`, `quantity`, `price`, `totalBill`, `status`.
   - Pointers: `prev`, `next`.
   - Role: Purely a data-holding class/struct.
2. **`Restaurant`:**
   - Fields: `head`, `tail`, `nextOrderId`.
   - Role: Manages memory, list pointers, file streams, and UI console outputs.

### Important Functions
- `addOrder()`: Prompts for input, creates a new `Order` node, appends it to the list, and triggers a file save.
- `deleteOrder()`: Finds an order by ID, adjusts adjacent `prev` and `next` pointers to bypass it, deletes the memory, and saves.
- `displayDashboard()`: Traverses the list, counting statuses and aggregating `totalBill`.
- `loadOrdersFromFile()`: Parses CSV formatted lines from `orders.txt`, instantiates nodes, and builds the linked list on startup.
- `saveOrdersToFile()`: Iterates through the list, writing out fields separated by commas to `orders.txt`.

### Inter-module Communication
1. **User to `main()`:** User inputs a choice via `std::cin`.
2. **`main()` to `Restaurant`:** Based on the choice, a specific public method (e.g., `restaurant.addOrder()`) is invoked.
3. **`Restaurant` to `Order`:** The `Restaurant` class instantiates `Order` objects via the `new` keyword and manipulates their `prev`/`next` pointers.
4. **`Restaurant` to Disk:** Whenever a modification occurs, `Restaurant` opens an `std::ofstream` to write the current state of all `Order` objects to `orders.txt`.

---

## Program Workflow

```text
1. Application Starts (main.cpp).
2. Restaurant object is instantiated.
3. loadOrdersFromFile() is called. 
   - Reads orders.txt.
   - Reconstructs the Doubly Linked List.
   - Sets nextOrderId based on the highest found ID.
4. displayMenu() is called.
5. User selects an option (e.g., 1 to Add Order).
6. Associated Restaurant method (addOrder()) executes.
   - Input is taken and validated.
   - New node added to list.
   - saveOrdersToFile() is called to persist change.
7. Return to step 4 until User selects '11' (Exit).
8. displayExitSummary() runs.
9. Application terminates. Destructor frees memory.
```

## Menu Flow

```text
[ MAIN MENU ]
    |
    +-- 1. Add Order ----> Input Details -> Calculate Bill -> Append DLL -> Save -> [Menu]
    +-- 2. Search Order -> Input ID -> Traverse DLL -> Display -> [Menu]
    +-- 3. Update Order -> Input ID -> Find -> Input New Details -> Update Node -> Save -> [Menu]
    +-- 4. Delete Order -> Input ID -> Find -> Bypass Pointers -> Free Memory -> Save -> [Menu]
    +-- 5. Display All --> Traverse DLL -> Print Table -> [Menu]
    +-- 6. Update Status-> Input ID -> Find -> Select Status(1-6) -> Update -> Save -> [Menu]
    +-- 7. Filter Status-> Select Status -> Traverse DLL -> Print Matches -> [Menu]
    +-- 8. Dashboard ----> Traverse DLL -> Tally Stats -> Print Summary -> [Menu]
    +-- 9. Revenue ------> Traverse DLL -> Sum Bills -> Print -> [Menu]
    +-- 10. About -------> Print Info -> [Menu]
    +-- 11. Exit --------> Print Summary -> Free Memory -> Terminate Program
```

---

## Data Structures Deep Dive

### Node Structure Explanation
Each `Order` object is a Node containing data fields and two directional pointers:
```cpp
class Order {
    int orderId;
    std::string customerName;
    // ... other data fields
    Order* prev;
    Order* next;
};
```

### Doubly Linked List Working

```text
HEAD                                                                    TAIL
 +---------+         +---------+         +---------+         +---------+
 | Order 1 |  next   | Order 2 |  next   | Order 3 |  next   | Order 4 |
 | ID:1001 | ------> | ID:1002 | ------> | ID:1003 | ------> | ID:1004 | ---> NULL
 |         | <------ |         | <------ |         | <------ |         |
 +---------+   prev  +---------+   prev  +---------+   prev  +---------+
```
- The `Restaurant` class holds pointers to the `HEAD` and `TAIL`.
- Insertion happens at the `TAIL`. 
- Deletion involves updating the `next` pointer of `prev` and the `prev` pointer of `next`.

### Memory Management Explanation
- **Allocation:** Nodes are created dynamically on the heap using the `new` keyword during `addOrder()` and `loadOrdersFromFile()`.
- **Deallocation:** When an order is deleted, the `delete current;` command frees the specific node's memory.
- **Destructor:** When the program exits, `~Restaurant()` traverses the entire list from `HEAD`, deleting every node to prevent memory leaks.

---

## System Logics

### CRUD Operation Flow
- **Create:** Validates inputs. Creates a new `Order`. Updates the old `tail->next` to point to it. Updates `tail` to the new node.
- **Read (Search/Display):** Starts at `head`, uses a `while(current != nullptr)` loop to traverse to `current->next`.
- **Update:** Traverses to find the ID. Directly modifies the attributes of the `current` object.
- **Delete:** Traverses to find the ID. 
  - If `HEAD`: `head = head->next; head->prev = nullptr;`
  - If `TAIL`: `tail = tail->prev; tail->next = nullptr;`
  - If Middle: `current->prev->next = current->next; current->next->prev = current->prev;`

### Revenue Calculation Logic
The application does not store total revenue as a static variable. Instead, every time the Dashboard or Revenue screen is requested, the system iterates through the active linked list and sums up the `totalBill` attribute of every node in O(N) time. This ensures absolute accuracy and prevents sync issues.

### Order Status Management Logic
Each order defaults to `"Pending"`. Users can transition the order through a predefined set of string statuses: `Pending, Preparing, Ready, Served, Completed, Cancelled`. The dashboard logic specifically counts these string occurrences.

### File Handling Logic
- **Storage Format:** CSV (Comma Separated Values) inside `orders.txt`.
- **Saving:** Overwrites the entire `orders.txt` file on *every* modification by traversing the list from head to tail.
- **Loading:** Reads `orders.txt` line by line. Uses `std::stringstream` to split by commas (`getline(ss, token, ',')`). Casts strings back to integers and doubles, and rebuilds the linked list.

### Input Validation Logic
`std::cin` stream states are actively monitored. If a user inputs a letter when an integer is expected, the stream fails. The system catches this:
```cpp
if (!(std::cin >> choice)) {
    std::cin.clear(); // Clear the error flag
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad input
    // Show error message
}
```
This guarantees the program will never enter an infinite crash loop due to bad inputs.

### Error Handling Strategy
- Missing file on startup? Soft fail (assume it's the first run, load an empty list).
- Order not found during Search/Update/Delete? Output "Order Not Found." and return safely to the menu.

---

## Complexity Analysis

### Time Complexity
- **Add Order:** `O(1)` (Insertion at tail).
- **Search Order:** `O(N)` (Linear traversal).
- **Update/Delete Order:** `O(N)` (Finding the node is O(N), updating/pointer manipulation is O(1)).
- **Display All/Dashboard:** `O(N)`.
- **File Save/Load:** `O(N)`.
Where N is the number of active orders.

### Space Complexity
- **Overall Application:** `O(N)` where N is the number of active orders. Each order takes a fixed amount of memory (pointers, strings, ints, doubles). 

---

## Evaluation

### Advantages of the current implementation
1. **Blazing Fast In-Memory Operations:** Because the database is a linked list in RAM, CRUD operations feel instantaneous.
2. **Zero Dependencies:** No external database servers (like MySQL) or heavy libraries are required. It compiles anywhere.
3. **Robust Input Validation:** Highly resistant to crash-inducing bad user inputs.
4. **Data Safety:** Auto-saving on every action ensures a power outage only loses the currently typing keystrokes, nothing more.

### Limitations of the current implementation
1. **File Overwrite Bottleneck:** `saveOrdersToFile()` rewrites the *entire* list to the file on every single change. For 10,000+ orders, this O(N) operation will cause noticeable UI lag.
2. **String-based Statuses:** Statuses are strings, prone to case-sensitivity issues or spelling mistakes if modified directly.
3. **No Historical Archive:** Deleted orders are permanently removed. There is no "Archive" file for historical auditing.

---

## Future Vision

### Future Scope
1. **Authentication:** Implement a Login system (Admin vs Staff roles).
2. **Database Integration:** Replace `orders.txt` with SQLite or MySQL for massive scale.
3. **Menu Management:** Allow dynamic addition/removal of food items with predefined prices rather than typing them manually every time.

### Scalability Plan
To scale this application, the linked-list file-writing logic should be transitioned to an Append-Only log (O(1) file writing), or a background thread should handle file writing asynchronously so the main UI thread is not blocked during large saves.

---

## Sample Console Output
```text
====================================================
 SMART RESTAURANT MANAGEMENT SYSTEM - DISPLAY ORDERS
====================================================

Order ID  Customer            Food                Qty       Price          Bill           Status
----------------------------------------------------------------------------------------------------
1001      John Doe            Pizza               2         ₹250.00        ₹500.00        Preparing
1002      Alice Smith         Burger              1         ₹150.00        ₹150.00        Pending

====================================================
```

---

## Getting Started

### Installation Guide
1. Ensure you have a C++ compiler installed (e.g., GCC, Clang, or MSVC).
2. Clone or download the project directory.

### Compilation Instructions
**Using g++ (Terminal):**
```bash
g++ main.cpp Restaurant.cpp Order.cpp -o SmartRestaurant.exe -std=c++17
```

**Using CMake (if preferred):**
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Run Instructions
**Windows:**
```bash
SmartRestaurant.exe
```
**Linux / macOS:**
```bash
./SmartRestaurant
```

### Example Usage
1. Run the application.
2. Press `1` and hit Enter to add an order.
3. Type "Mark", "Pasta", "2", "200".
4. Press `5` to view the order you just placed.
5. Press `8` to see the dashboard summarize your revenue.
6. Press `11` to safely exit the system.

---

## Screenshots
*(Add screenshots here to visualize the terminal UI)*

![Main Menu Placeholder](link_to_image)
![Dashboard Placeholder](link_to_image)

---

## Project Specifics

### Project Dependencies
- **C++ Standard Library:** `<iostream>`, `<iomanip>`, `<fstream>`, `<sstream>`, `<limits>`, `<string>`.
- **System Commands:** Uses `std::system("cls")` for clearing the screen (Windows specific. May need changing to `"clear"` for Unix systems).

### Coding Standards Followed
- **Naming Conventions:** camelCase for variables and functions (`addOrder`, `nextOrderId`). PascalCase for Classes (`Restaurant`, `Order`).
- **Documentation:** Header files (`Restaurant.h`) feature detailed block comments including Time Complexity analysis.
- **Safety:** Use of `const` correctness on read-only methods (`displayOrders() const`).

### Design Decisions
- **Total Bill calculation on Creation:** Decided to calculate and store `totalBill` upon `Order` construction to save CPU cycles during dashboard rendering.
- **Auto-Save:** Decided to sacrifice some CPU efficiency (saving the whole file on every edit) to guarantee data persistence without requiring the user to hit a "Save" button.

### Assumptions
- Customer names and Food names do not contain commas (`,`). If they do, the CSV file parsing in `loadOrdersFromFile()` will break.
- The terminal environment supports `system("cls")`.
- UTF-8 encoding is supported in the terminal to display the Rupee symbol (`₹`).

### Known Issues
- Inputting a comma inside a string (e.g., "Burger, Extra cheese") will corrupt the `orders.txt` file format and cause bugs upon reloading.
- Screen clearing relies on OS-specific commands (`cls`).

---

## Conclusion
The Smart Restaurant Management System demonstrates a highly effective application of core Data Structures (Doubly Linked List) and Object-Oriented Programming in C++. It bridges the gap between theoretical computer science concepts and practical, real-world business utility, providing a solid foundation that can be expanded into a commercial-grade application.
