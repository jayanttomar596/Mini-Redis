#pragma once
#include <unordered_map>
#include <mutex>
#include <list>
#include <string>




class KVStore {
private:
    int capacity = 5;  //small for testing

    // key → (value, iterator in list)
    std::unordered_map<std::string, std::pair<std::string, std::list<std::string>::iterator>> store;

    // doubly linked list (most recent at front)
    std::list<std::string> lru;

    std::mutex mtx;

public:
    void set(const std::string &key, const std::string &value);
    std::string get(const std::string &key);
    void del(const std::string &key);
};




