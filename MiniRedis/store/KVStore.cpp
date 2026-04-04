#include "KVStore.h"


void KVStore::set(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(mtx);   // LOCK
    store[key] = value;
}


std::string KVStore::get(const std::string &key) {
    std::lock_guard<std::mutex> lock(mtx);  // LOCK

    if (store.find(key) != store.end())
        return store[key];

    return "NULL";
}



void KVStore::del(const std::string &key) {
    std::lock_guard<std::mutex> lock(mtx);  // LOCK
    store.erase(key);
}




