#include "Server.h"
#include "../parser/CommandParser.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

void Server::start(int port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0) {
        perror("Socket failed");
        return;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return;
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        return;
    }

    cout << "Server started on port " << port << endl;

    int addrlen = sizeof(address);

    while (true) {
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        cout << "Client connected\n";

        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);

        string input(buffer);

        vector<string> tokens = CommandParser::parse(input);

        string response;

        if (tokens[0] == "SET" && tokens.size() == 3) {
            kv.set(tokens[1], tokens[2]);
            response = "OK\n";
        }
        else if (tokens[0] == "GET" && tokens.size() == 2) {
            response = kv.get(tokens[1]) + "\n";
        }
        else if (tokens[0] == "DEL" && tokens.size() == 2) {
            kv.del(tokens[1]);
            response = "Deleted\n";
        }
        else {
            response = "Invalid Command\n";
        }

        send(client_socket, response.c_str(), response.size(), 0);
        close(client_socket);
    }
}