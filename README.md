# 🍽️ Smart Restaurant Management System

[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows)](https://microsoft.com)
[![Web Interface](https://img.shields.io/badge/Web_UI-HTML5%20%7C%20CSS3%20%7C%20JS-E34F26?style=for-the-badge&logo=html5)](presentation.html)
[![Networking](https://img.shields.io/badge/Networking-WinSock2-blue?style=for-the-badge)](https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)

An end-to-end, high-performance **Restaurant Management System** combining a robust **C++17 Doubly Linked List engine** and an embedded **WinSock HTTP REST Server** with a sleek, responsive **Web Dashboard & Visualizer**.

---

## 📌 Table of Contents
- [🌟 Key Highlights](#-key-highlights)
- [🏗️ System Architecture](#️-system-architecture)
- [🧩 Data Structures & Algorithms](#-data-structures--algorithms)
- [🚀 Features](#-features)
- [📂 Repository Structure](#-repository-structure)
- [🛠️ Build & Installation](#️-build--installation)
- [🌐 REST API Reference](#-rest-api-reference)
- [📊 Complexity Analysis](#-complexity-analysis)
- [📸 User Interface & Visualizer](#-user-interface--visualizer)
- [📄 License & Authors](#-license--authors)

---

## 🌟 Key Highlights

- **Custom Doubly Linked List Engine:** Low-overhead in-memory order pipeline engineered with bidirectional pointers for $O(1)$ appends and efficient node deletions.
- **Embedded WinSock HTTP Server:** Native C++ non-blocking socket server running on port `8080`, providing RESTful JSON endpoints without heavy third-party web frameworks.
- **Interactive Web UI & Visualizer (`presentation.html`):** Real-time visual representation of Doubly Linked List nodes in memory, dynamic order placement, automated invoice generation with tax calculations, and step-by-step bidirectional traversal animations.
- **Persistent Storage:** Synchronous file I/O layer (`orders.txt`) for automatic session persistence, order recovery, and state reconciliation upon boot.

---

## 🏗️ System Architecture

```
+-------------------------------------------------------------+
|                      Client Layer                           |
|   Interactive Web Dashboard / Visualizer (presentation.html) |
+-------------------------------------------------------------+
                              |
                     HTTP / JSON (REST API)
                              v
+-------------------------------------------------------------+
|               C++ WinSock HTTP Server Layer                 |
|               (HttpServer.h - Port 8080)                    |
|   - Request Router (/api/orders, /api/orders/status)        |
|   - JSON Parser & Response Formatter                        |
+-------------------------------------------------------------+
                              |
                      Method Dispatching
                              v
+-------------------------------------------------------------+
|             Business Logic & List Controller                |
|                    (Restaurant.cpp / .h)                    |
|   - Head & Tail Pointers Management                         |
|   - Order Lifecycle & Status Engine                         |
|   - Revenue Aggregation & Validation                        |
+-------------------------------------------------------------+
             |                                  |
   Pointer Manipulations               Synchronous File I/O
             v                                  v
+------------------------+             +----------------------+
|   Doubly Linked List   |             |   orders.txt         |
|      (Order Nodes)     |             |  (Persistent Disk)   |
+------------------------+             +----------------------+
```

---

## 🧩 Data Structures & Algorithms

### Doubly Linked List (DLL) Design
Each customer order is represented by a dynamically allocated node:

```cpp
class Order {
public:
    int orderId;
    std::string customerName;
    int tableNumber;
    std::string foodName;
    int quantity;
    double price;
    double totalBill;
    std::string status; // Pending, Preparing, Ready, Served, Completed, Cancelled
    
    Order* prev;
    Order* next;
};
```

```
HEAD                                                                    TAIL
 +---------+         +---------+         +---------+         +---------+
 | Order 1 |  next   | Order 2 |  next   | Order 3 |  next   | Order 4 |
 | ID:1001 | ------> | ID:1002 | ------> | ID:1003 | ------> | ID:1004 | ---> NULL
 |         | <------ |         | <------ |         | <------ |         |
 +---------+   prev  +---------+   prev  +---------+   prev  +---------+
```

### Why Doubly Linked List?
1. **Dynamic Memory Allocation:** Seamlessly grows and shrinks with demand without the need for contiguous memory re-allocation or capacity copying.
2. **$O(1)$ Tail Insertion:** By retaining a `tail` pointer, incoming orders are appended instantaneously.
3. **$O(1)$ Pointer-level Deletion:** Once a target order is located, removing it requires only updating adjacent references (`prev->next` and `next->prev`) and releasing the memory.
4. **Bidirectional Traversal:** Permits forward chronological audits as well as reverse traversal for recent order tracking.

---

## 🚀 Features

- **Order Management (CRUD):** Add, view, edit, search, and delete orders in real time.
- **Order Lifecycle Tracking:** Follow orders through 6 states: `Pending` ➔ `Preparing` ➔ `Ready` ➔ `Served` ➔ `Completed` (or `Cancelled`).
- **Billing & Tax Computation:** Automatically computes item subtotals, Central GST (2.5%), State GST (2.5%), and grand total payable.
- **Live Memory Node Strip:** Visualizes nodes directly in the browser with memory connection lines and highlight animations during traversal.
- **Dark / Light Theme:** Modern minimalist interface with persistent theme toggle.

---

## 📂 Repository Structure

```
Restaurant-Management-System/
│
├── README.md                      # Main project documentation
├── presentation.html              # Interactive Web Dashboard, Visualizer & Presentation
├── .gitignore                     # Build artifacts and binary exclusions
│
└── Restaurant-system/             # C++ Core Engine
    ├── CMakeLists.txt             # CMake build configuration
    ├── main.cpp                   # Application entry point & server orchestrator
    ├── HttpServer.h               # WinSock2 embedded HTTP REST server
    ├── Restaurant.h               # Core Restaurant controller interface
    ├── Restaurant.cpp             # Implementation of DLL business logic & I/O
    ├── Order.h                    # Order node class declaration
    ├── Order.cpp                  # Order constructor & methods
    ├── orders.txt                 # Persistent CSV storage file
    └── README.md                  # Detailed C++ engine specification
```

---

## 🛠️ Build & Installation

### Prerequisites
- **Operating System:** Windows 10 / 11 (64-bit)
- **Compiler:** GCC / MinGW (C++17 support) or Microsoft Visual Studio (MSVC)
- **Linker Flag:** `-lws2_32` (Windows Sockets 2 library)

### Compilation

#### Option 1: Direct g++ / MinGW Build
Open PowerShell or Command Prompt in the `Restaurant-system/` directory:

```powershell
cd Restaurant-system
g++ -std=c++17 main.cpp Restaurant.cpp Order.cpp -lws2_32 -o SmartRestaurant.exe
```

#### Option 2: CMake Build
```powershell
cd Restaurant-system
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Running the Application

1. **Launch the C++ REST Server:**
   ```powershell
   .\SmartRestaurant.exe
   ```
   *Output:*
   ```text
   Loading existing orders from orders.txt...
   Loaded 6 orders successfully.
   [Server] Listening on http://localhost:8080
   ```

2. **Open the Web Interface:**
   Double click `presentation.html` or open it in any modern browser (Chrome, Edge, Firefox, Brave):
   ```powershell
   Start-Process ..\presentation.html
   ```

---

## 🌐 REST API Reference

The embedded WinSock server exposes the following REST endpoints on `http://localhost:8080`:

| Method | Endpoint | Description | Sample Request / Query |
| :--- | :--- | :--- | :--- |
| `GET` | `/api/orders` | Retrieve all active orders | None |
| `POST` | `/api/orders` | Create a new order | `{"name":"Alice","table":3,"food":"Pizza","qty":2,"price":250}` |
| `PUT` | `/api/orders/status` | Update an existing order status | `{"id":1005,"status":"Served"}` |
| `DELETE` | `/api/orders?id=1005` | Remove an order from DLL and disk | `id=1005` (URL query parameter) |

---

## 📊 Complexity Analysis

| Operation | Time Complexity | Space Complexity | Notes |
| :--- | :---: | :---: | :--- |
| **Append New Order** | $O(1)$ | $O(1)$ | Direct insertion at `tail` |
| **Search by ID** | $O(n)$ | $O(1)$ | Linear scan through the linked list |
| **Delete Order** | $O(n)$ search + $O(1)$ unlink | $O(1)$ | Traversal to target node, followed by pointer updates |
| **Update Status** | $O(n)$ | $O(1)$ | Direct field mutation in memory |
| **Calculate Revenue** | $O(n)$ | $O(1)$ | Single pass accumulator |
| **File Persistence** | $O(n)$ | $O(1)$ | Sequential write to disk |

---

## 📸 User Interface & Visualizer

The bundled `presentation.html` contains:
- **Live Memory Strip:** Interactive boxes representing each node with pointer badges and instant node selection.
- **Animated Node Traversal:** Visually highlights orders sequentially from `HEAD` to `TAIL` or in reverse order (`TAIL` to `HEAD`).
- **Interactive POS Form:** Enter customer credentials, table number, item, and rates to test API dispatching.
- **Invoice Modal:** Computes and renders a printable bill with CGST, SGST, and Grand Total.

---

## 📄 License & Authors

Developed by **Omji Mishra**  
- **GitHub:** [@omji-mishraa](https://github.com/omji-mishraa)  
- **Repository:** [Restaurant-Management-System](https://github.com/omji-mishraa/Restaurant-Management-System)

Distributed under the **MIT License**.
