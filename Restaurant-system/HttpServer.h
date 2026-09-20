#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include "Restaurant.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

// Simple inline JSON parser helper
inline std::string getJsonValue(const std::string& json, const std::string& key) {
    size_t pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(":", pos);
    if (pos == std::string::npos) return "";
    pos++; // skip ':'
    // skip whitespace
    while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\r' || json[pos] == '\n')) {
        pos++;
    }
    if (pos >= json.length()) return "";
    if (json[pos] == '\"') {
        // String value
        pos++;
        size_t endPos = json.find("\"", pos);
        if (endPos == std::string::npos) return "";
        return json.substr(pos, endPos - pos);
    } else {
        // Number or boolean value
        size_t endPos = pos;
        while (endPos < json.length() && json[endPos] != ',' && json[endPos] != '}' && json[endPos] != ']') {
            endPos++;
        }
        std::string val = json.substr(pos, endPos - pos);
        // trim trailing whitespace/delimiters
        while (!val.empty() && (val.back() == ' ' || val.back() == '\t' || val.back() == '\r' || val.back() == '\n' || val.back() == '}')) {
            val.pop_back();
        }
        return val;
    }
}

class HttpServer {
private:
    SOCKET listenSocket;
    Restaurant& restaurant;
    int port;

public:
    HttpServer(Restaurant& rest, int p = 8080) : restaurant(rest), port(p), listenSocket(INVALID_SOCKET) {}

    ~HttpServer() {
        if (listenSocket != INVALID_SOCKET) {
            closesocket(listenSocket);
        }
        WSACleanup();
    }

    bool init() {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            std::cerr << "WSAStartup failed with error: " << iResult << "\n";
            return false;
        }

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        std::string portStr = std::to_string(port);
        iResult = getaddrinfo(NULL, portStr.c_str(), &hints, &result);
        if (iResult != 0) {
            std::cerr << "getaddrinfo failed with error: " << iResult << "\n";
            WSACleanup();
            return false;
        }

        listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (listenSocket == INVALID_SOCKET) {
            std::cerr << "socket failed with error: " << WSAGetLastError() << "\n";
            freeaddrinfo(result);
            WSACleanup();
            return false;
        }

        // Allow address reuse
        char optval = 1;
        setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

        iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "bind failed with error: " << WSAGetLastError() << "\n";
            freeaddrinfo(result);
            closesocket(listenSocket);
            WSACleanup();
            return false;
        }

        freeaddrinfo(result);

        iResult = listen(listenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "listen failed with error: " << WSAGetLastError() << "\n";
            closesocket(listenSocket);
            WSACleanup();
            return false;
        }

        std::cout << "\n==================================================\n";
        std::cout << "  SMART RESTAURANT WEB SERVER STARTED\n";
        std::cout << "  Open this link in your browser:\n";
        std::cout << "  --> http://localhost:" << port << "/ <--\n";
        std::cout << "  Press Ctrl+C in this terminal to stop the server.\n";
        std::cout << "==================================================\n\n";

        return true;
    }

    void start() {
        while (true) {
            SOCKET clientSocket = accept(listenSocket, NULL, NULL);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "accept failed with error: " << WSAGetLastError() << "\n";
                break;
            }

            handleClient(clientSocket);
        }
    }

private:
    void handleClient(SOCKET clientSocket) {
        std::vector<char> buffer(8192);
        int bytesReceived = recv(clientSocket, buffer.data(), buffer.size() - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string request(buffer.data());

            // Parse HTTP Request Line
            std::stringstream ss(request);
            std::string method, path, httpVersion;
            ss >> method >> path >> httpVersion;

            // Extract body if present
            size_t bodyPos = request.find("\r\n\r\n");
            std::string body = "";
            if (bodyPos != std::string::npos) {
                body = request.substr(bodyPos + 4);
            }

            // Handle preflight CORS OPTIONS request
            if (method == "OPTIONS") {
                sendCorsOk(clientSocket);
            } else if (method == "GET" && (path == "/" || path == "/index.html")) {
                serveFile(clientSocket, "../presentation.html", "text/html");
            } else if (method == "GET" && path == "/api/orders") {
                std::string json = restaurant.getOrdersAsJson();
                sendJsonResponse(clientSocket, 200, "OK", json);
            } else if (method == "POST" && path == "/api/orders") {
                std::string name = getJsonValue(body, "name");
                std::string tableStr = getJsonValue(body, "table");
                std::string food = getJsonValue(body, "food");
                std::string qtyStr = getJsonValue(body, "qty");
                std::string priceStr = getJsonValue(body, "price");

                int table = tableStr.empty() ? 0 : std::stoi(tableStr);
                int qty = qtyStr.empty() ? 1 : std::stoi(qtyStr);
                double price = priceStr.empty() ? 0.0 : std::stod(priceStr);

                Order* newOrder = restaurant.addOrderProgrammatic(name, table, food, qty, price);
                
                std::stringstream respJson;
                respJson << "{\"success\":true,\"id\":" << newOrder->orderId << "}";
                sendJsonResponse(clientSocket, 200, "OK", respJson.str());
            } else if (method == "PUT" && path == "/api/orders/status") {
                std::string idStr = getJsonValue(body, "id");
                std::string status = getJsonValue(body, "status");
                
                int id = idStr.empty() ? 0 : std::stoi(idStr);
                bool success = restaurant.updateOrderStatusProgrammatic(id, status);
                
                std::stringstream respJson;
                respJson << "{\"success\":" << (success ? "true" : "false") << "}";
                sendJsonResponse(clientSocket, success ? 200 : 400, success ? "OK" : "Bad Request", respJson.str());
            } else if (method == "DELETE" && path.rfind("/api/orders", 0) == 0) {
                // Parse ID from query params (e.g. /api/orders?id=1001)
                int id = 0;
                size_t idPos = path.find("id=");
                if (idPos != std::string::npos) {
                    size_t endPos = path.find_first_not_of("0123456789", idPos + 3);
                    std::string idStr = (endPos == std::string::npos) ? path.substr(idPos + 3) : path.substr(idPos + 3, endPos - (idPos + 3));
                    if (!idStr.empty()) {
                        id = std::stoi(idStr);
                    }
                }
                
                bool success = restaurant.deleteOrderProgrammatic(id);
                std::stringstream respJson;
                respJson << "{\"success\":" << (success ? "true" : "false") << "}";
                sendJsonResponse(clientSocket, success ? 200 : 400, success ? "OK" : "Bad Request", respJson.str());
            } else {
                sendJsonResponse(clientSocket, 404, "Not Found", "{\"error\":\"Not Found\"}");
            }
        }
        closesocket(clientSocket);
    }

    void serveFile(SOCKET clientSocket, const std::string& filePath, const std::string& contentType) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            // Try fallback to local directory in case pathing is different
            std::ifstream fileFallback("presentation.html", std::ios::binary);
            if (!fileFallback) {
                std::string notFound = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 30\r\n\r\npresentation.html was not found";
                send(clientSocket, notFound.c_str(), notFound.length(), 0);
                return;
            }
            file.swap(fileFallback);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        std::stringstream response;
        response << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: " << contentType << "\r\n"
                 << "Content-Length: " << content.length() << "\r\n"
                 << "Access-Control-Allow-Origin: *\r\n"
                 << "Connection: close\r\n\r\n"
                 << content;

        send(clientSocket, response.str().c_str(), response.str().length(), 0);
    }

    void sendJsonResponse(SOCKET clientSocket, int statusCode, const std::string& statusStr, const std::string& body) {
        std::stringstream response;
        response << "HTTP/1.1 " << statusCode << " " << statusStr << "\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << body.length() << "\r\n"
                 << "Access-Control-Allow-Origin: *\r\n"
                 << "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                 << "Access-Control-Allow-Headers: Content-Type\r\n"
                 << "Connection: close\r\n\r\n"
                 << body;

        send(clientSocket, response.str().c_str(), response.str().length(), 0);
    }

    void sendCorsOk(SOCKET clientSocket) {
        std::stringstream response;
        response << "HTTP/1.1 204 No Content\r\n"
                 << "Access-Control-Allow-Origin: *\r\n"
                 << "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                 << "Access-Control-Allow-Headers: Content-Type\r\n"
                 << "Connection: close\r\n\r\n";
        send(clientSocket, response.str().c_str(), response.str().length(), 0);
    }
};

#endif // HTTP_SERVER_H
