#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include <mutex>
#include <chrono>







class KVStore {
private:
    int capacity = 5;

    struct Node {
        std::string value;
        long long expiry;   // 🔥 epoch time (ms), -1 = no expiry
        std::list<std::string>::iterator it;
    };

    std::unordered_map<std::string, Node> store;
    std::list<std::string> lru;

    std::mutex mtx;

    long long getCurrentTime();  // helper

public:
    void set(const std::string &key, const std::string &value, int ttl = -1); // 🔥 updated
    std::string get(const std::string &key);
    void del(const std::string &key);
};





