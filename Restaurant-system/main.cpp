#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include "Restaurant.h"
#include "HttpServer.h"
#include <iostream>

int main() {
    Restaurant restaurant;
    
    // Load previously saved data on startup
    restaurant.loadOrdersFromFile();
    
    // Initialize and start the Web Server on port 8080
    HttpServer server(restaurant, 8080);
    if (server.init()) {
        server.start();
    } else {
        std::cerr << "Failed to start the web server.\n";
        return 1;
    }

    return 0;
}
