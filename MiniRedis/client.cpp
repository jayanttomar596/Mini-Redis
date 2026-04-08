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
    string hello = "CLIENT\n";
    send(sock, hello.c_str(), hello.size(), 0);

    char buffer[1024];

    while (true) {
        cout << ">> ";
        string cmd;
        getline(cin, cmd);

        // ignore empty / whitespace input
        bool onlySpaces = true;
        for (char c : cmd) {
            if (!isspace(c)) {
                onlySpaces = false;
                break;
            }
        }
        if (onlySpaces) continue;

        if (cmd == "EXIT") {
            close(sock);
            break;
        }

        string msg = cmd + "\n";

        if (send(sock, msg.c_str(), msg.size(), 0) < 0) {
            perror("Send failed");
            break;
        }

        static string pending = "";

        while (true) {
            int valread = recv(sock, buffer, 1024, 0);

            if (valread <= 0) {
                cout << "Server closed connection\n";
                break;
            }

            pending += string(buffer, valread);

            size_t pos;
            while ((pos = pending.find('\n')) != string::npos) {
                string line = pending.substr(0, pos);
                pending.erase(0, pos + 1);
                cout << line << endl;
            }

            // break after at least one full response
            if (pending.empty()) break;
        }
    }

    return 0;
}



