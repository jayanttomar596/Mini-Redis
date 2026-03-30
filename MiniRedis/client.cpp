#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    cout << "Connected to server\n";

    char buffer[1024];

    while (true) {
        cout << ">> ";
        string cmd;
        getline(cin, cmd);

        if (cmd == "EXIT") {
            close(sock);
            break;
        }

        string msg = cmd + "\n";

        if (send(sock, msg.c_str(), msg.size(), 0) < 0) {
            perror("Send failed");
            break;
        }

        memset(buffer, 0, sizeof(buffer));

        int valread = recv(sock, buffer, 1024, 0);

        if (valread <= 0) {
            cout << "Server closed connection\n";
            break;
        }

        cout << string(buffer, valread);
    }

    return 0;
}



