#include "commtcpcomponent.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <opencv2/opencv.hpp>

CommTCPComponent::CommTCPComponent(int port, ThreadSafeQueue<cv::Mat>& outputQueue)
: port(port), outputQueue(outputQueue), running(false) {}

CommTCPComponent::~CommTCPComponent() {
    stopServer();
}

void CommTCPComponent::startServer() {
    if (running) return;
    running = true;
    serverThread = std::thread(&CommTCPComponent::serverLoop, this);
    std::cout << "Server starting..." << std::endl;
}

void CommTCPComponent::stopServer() {
    running = false;
    if (serverThread.joinable()) {
        serverThread.join();
    }
    std::cout << "Server stopped." << std::endl;
}

void CommTCPComponent::serverLoop() {
    int serverFd, newSocket;
    struct sockaddr_in address;
    int opt = 1;

    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(serverFd, F_SETFL, O_NONBLOCK);  // Set socket to non-blocking

    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(serverFd, (struct sockaddr*)&address, sizeof(address));
    listen(serverFd, 3);

    std::cout << "Server is ready and waiting for connections on port " << port << std::endl;

    while (running) {
        newSocket = accept(serverFd, NULL, NULL);
        if (newSocket > 0) {
            std::cout << "Client connected: socket FD " << newSocket << std::endl;
            std::thread(&CommTCPComponent::handleClient, this, newSocket).detach();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    close(serverFd);
}

//----------------------------------connection check----------------------------------------------

//void CommTCPComponent::handleClient(int clientSocket) {
    // Simulate some processing time
//    std::this_thread::sleep_for(std::chrono::seconds(2));

//    std::cout << "Closing client connection: socket FD " << clientSocket << std::endl;
//    close(clientSocket);
//}

//----------------------------------echo check----------------------------------------------

//void CommTCPComponent::handleClient(int clientSocket) {
//    char buffer[1024] = {0};
//    ssize_t bytesRead;

//    while ((bytesRead = recv(clientSocket, buffer, 1024, 0)) > 0) {
//        send(clientSocket, buffer, bytesRead, 0);  // Echo back the received data
//   }

//    close(clientSocket);
//}

//----------------------------------commands check----------------------------------------------

//void CommTCPComponent::handleClient(int clientSocket) {
//    char buffer[1024] = {0};
//    ssize_t bytesRead;

//    while ((bytesRead = recv(clientSocket, buffer, 1024, 0)) > 0) {
//         //Create a string from the buffer and trim potential newline characters
//        std::string command(buffer, bytesRead);
//        command.erase(std::remove(command.begin(), command.end(), '\n'), command.end());
//        command.erase(std::remove(command.begin(), command.end(), '\r'), command.end());

        //Debug print the trimmed command
//        std::cout << "Received command: [" << command << "]" << std::endl;

//        if (command == "ping") {
//            std::string response = "pong\n";
//            send(clientSocket, response.c_str(), response.length(), 0);
//        } else if (command == "quit") {
//            break;
//        } else {
//            std::string response = "Unknown command\n";
//            send(clientSocket, response.c_str(), response.length(), 0);
//        }
//    }

//    close(clientSocket);
//}

//----------------------------------image check----------------------------------------------

//void CommTCPComponent::handleClient(int clientSocket) {
//    cv::Mat testImage = cv::imread("/home/anas/DMS-main/Images/PHOTO-2024-03-03-20-10-22.jpg");
//    std::vector<uchar> buffer;
//    cv::imencode(".jpeg", testImage, buffer);
//    //auto bufferSize = buffer.size();
//    auto bufferSize = htonl(buffer.size()); // Convert to network byte order
//    send(clientSocket, &bufferSize, sizeof(bufferSize), 0);
//    send(clientSocket, buffer.data(), buffer.size(), 0);

//    close(clientSocket);
//}

//----------------------------------frames check----------------------------------------------

void CommTCPComponent::handleClient(int clientSocket) {
    cv::Mat frame;
    while (running) {
        if (outputQueue.tryPop(frame) && !frame.empty()) {
           std::vector<uchar> buffer;
            cv::imencode(".jpg", frame, buffer);
            //auto bufferSize = buffer.size();
            auto bufferSize = htonl(buffer.size()); // Convert to network byte order
            send(clientSocket, &bufferSize, sizeof(bufferSize), 0);
            send(clientSocket, buffer.data(), buffer.size(), 0);
        }
    }
    close(clientSocket);
}


