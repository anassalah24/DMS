#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char* server_ip = "127.0.0.1"; // Server IP address
    int port = 12345; // Port on which the server is listening

    // Create the socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 or IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    std::cout << "Connected to the server at " << server_ip << ":" << port << std::endl;

    // Continuous frame reception and display
    while (true) {
        uint32_t imgSize;
        // First, receive the size of the image
        if(read(sock, &imgSize, sizeof(imgSize)) <= 0) {
            std::cerr << "Failed to read image size. Ending stream." << std::endl;
            break;
        }

        imgSize = ntohl(imgSize); // Ensure network byte order is converted to host byte order

        std::vector<uchar> imgData(imgSize);

        // Now, receive the image data
        ssize_t bytesRead = 0;
        size_t totalBytesRead = 0;
        uchar* bufferPtr = imgData.data();

        while (totalBytesRead < imgSize) {
            bytesRead = read(sock, bufferPtr + totalBytesRead, imgSize - totalBytesRead);
            if (bytesRead <= 0) {
                std::cerr << "Failed to read image data or connection closed by server. Ending stream." << std::endl;
                break;
            }
            totalBytesRead += bytesRead;
        }

        if (bytesRead <= 0) break; // Exit the loop if the connection is closed or an error occurred

        // Decode the received image data
        cv::Mat img = cv::imdecode(imgData, cv::IMREAD_COLOR);

        if (img.empty()) {
            std::cerr << "Received an empty image or decoding failed." << std::endl;
            continue;
        }

        // Display the image
        cv::imshow("Received Image", img);

        // Use waitKey(1) to give some time for the window to refresh and check if the user has requested an exit
        char c = (char)cv::waitKey(1);
        if (c == 27) break; // Exit the loop if the user presses the ESC key
    }

    close(sock); // Close the socket
    return 0;
}

