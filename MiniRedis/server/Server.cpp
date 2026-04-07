#include "Server.h"
#include "../parser/CommandParser.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/socket.h>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include "../utils/Logger.h"

using namespace std;




void loadData(KVStore &kv) {
    std::ifstream file("data.log");
    std::string line;

    while (getline(file, line)) {
        std::vector<std::string> tokens = CommandParser::parse(line);

        if (tokens.empty()) continue;

        if (tokens[0] == "SET") {
            if (tokens.size() == 5 && tokens[3] == "EX") {
                int ttl = stoi(tokens[4]);
                kv.set(tokens[1], tokens[2], ttl);
                Logger::log("SET " + tokens[1] + " " + tokens[2] + " EX " + tokens[4]);
            }
            else {
                kv.set(tokens[1], tokens[2]);
                Logger::log("SET " + tokens[1] + " " + tokens[2]);
            }
        }
        else if (tokens[0] == "DEL" && tokens.size() == 2) {
            kv.del(tokens[1]);
        }
    }
}







void handleClient(int client_socket, KVStore &kv , vector<int> &slaves) {
    bool is_slave = false;
    char buffer[1024];
    string pending = "";

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        int valread = recv(client_socket, buffer, 1024, 0);

        if (valread == 0) {
            cout << "Client disconnected\n";
            close(client_socket);
            break;
        }
        if (valread < 0) {
            perror("recv failed");
            close(client_socket);
            break;
        }

        pending += string(buffer, valread);

        size_t pos;
        while ((pos = pending.find('\n')) != string::npos) {
            string line = pending.substr(0, pos);
            pending.erase(0, pos + 1);

            if (line == "SLAVE") {
                cout << "Slave registered\n";
                slaves.push_back(client_socket);
                is_slave = true;
                continue;
            }

            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            vector<string> tokens = CommandParser::parse(line);

            if (tokens.empty()) continue;

            string response;


            if (tokens[0] == "SET") {

                if (tokens.size() == 5 && tokens[3] == "EX") {
                    int ttl = stoi(tokens[4]);
                    kv.set(tokens[1], tokens[2], ttl);

                    Logger::log("SET " + tokens[1] + " " + tokens[2] + " EX " + tokens[4]);
                    response = "OK\n";
                }
                else if (tokens.size() == 3) {
                    kv.set(tokens[1], tokens[2]);

                    Logger::log("SET " + tokens[1] + " " + tokens[2]);
                    response = "OK\n";
                }
                else {
                    response = "Invalid Command\n";
                }

                // FORWARD TO SLAVES
                if (!is_slave) {
                    for (int s : slaves) {
                        send(s, (line + "\n").c_str(), line.size() + 1, 0);
                    }
                }
            }
            else if (tokens[0] == "GET" && tokens.size() == 2) {
                response = kv.get(tokens[1]) + "\n";
            }
            else if (tokens[0] == "DEL" && tokens.size() == 2) {
                kv.del(tokens[1]);

                Logger::log("DEL " + tokens[1]);
                response = "Deleted\n";

                // FORWARD
                if (!is_slave) {
                    for (int s : slaves) {
                        send(s, (line + "\n").c_str(), line.size() + 1, 0);
                    }
                }
            }
            else {
                response = "Invalid Command\n";
            }

            send(client_socket, response.c_str(), response.size(), 0);
        }
    }
}







void Server::start(int port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (server_fd < 0) {
        perror("Socket failed");
        return;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (::bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) 
    {
        perror("Bind failed");
        return;
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        return;
    }

    loadData(kv);

    cout << "Server started on port " << port << endl;

    // int addrlen = sizeof(address);

    while (true) {
        sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);

        int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        cout << "Client connected\n";

        // create thread
        thread t([this, client_socket]() {
            handleClient(client_socket, this->kv, this->slave_sockets);
        });
        t.detach();
    }
}











void Server::startAsSlave(const std::string &ip, int port) {

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return;
    }

    cout << "Connected to master\n";

    // REGISTER
    send(sock, "SLAVE\n", 6, 0);

    char buffer[1024];
    string pending = "";

    while (true) {
        int valread = recv(sock, buffer, 1024, 0);

        if (valread <= 0) {
            cout << "Master disconnected\n";
            break;
        }

        pending += string(buffer, valread);

        size_t pos;
        while ((pos = pending.find('\n')) != string::npos) {

            string line = pending.substr(0, pos);
            pending.erase(0, pos + 1);

            vector<string> tokens = CommandParser::parse(line);

            if (tokens.empty()) continue;

            if (tokens[0] == "SET") {
                if (tokens.size() == 5 && tokens[3] == "EX") {
                    kv.set(tokens[1], tokens[2], stoi(tokens[4]));
                }
                else if (tokens.size() == 3) {
                    kv.set(tokens[1], tokens[2]);
                }
                cout << "[SLAVE] SET " << tokens[1] << " = " << tokens[2] << endl; // Just to check working 
            }
            else if (tokens[0] == "DEL") {
                kv.del(tokens[1]);
                cout << "[SLAVE] DEL " << tokens[1] << endl; // Just to check working
            }
        }
    }
}












