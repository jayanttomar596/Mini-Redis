#pragma once
#include "../store/KVStore.h"

class Server {
private:
    int server_fd;
    KVStore kv;

public:
    void start(int port);
};