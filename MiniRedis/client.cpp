#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    char buffer[1024];

    while (true) {
        cout << ">> ";
        string cmd;
        getline(cin, cmd);

        if (cmd == "EXIT") break;

        string msg = cmd + "\n";
        send(sock, msg.c_str(), msg.size(), 0);

        memset(buffer, 0, sizeof(buffer));
        int valread = recv(sock, buffer, 1024, 0);

        cout.write(buffer, valread);
        cout << endl;
    }

    close(sock);
}

