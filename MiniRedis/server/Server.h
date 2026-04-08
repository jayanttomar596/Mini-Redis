#pragma once
#include "../store/KVStore.h"
#include <vector>

class Server {
private:
    int server_fd;
    KVStore kv;
    std::vector<int> slave_sockets; // store slaves
    std::mutex slave_mtx;

public:
    void start(int port);
    void startAsSlave(const std::string &ip, int port);
};