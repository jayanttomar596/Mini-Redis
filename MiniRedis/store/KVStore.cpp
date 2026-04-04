#include "KVStore.h"


void KVStore::set(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(mtx);

    // If key exists → update + move to front
    if (store.find(key) != store.end()) {
        lru.erase(store[key].second);
    }
    else if (store.size() >= capacity) {
        // remove least recently used (back)
        std::string lru_key = lru.back();
        lru.pop_back();
        store.erase(lru_key);
    }

    // insert at front
    lru.push_front(key);
    store[key] = {value, lru.begin()};
}





std::string KVStore::get(const std::string &key) {
    std::lock_guard<std::mutex> lock(mtx);

    if (store.find(key) == store.end()) {
        return "NULL";
    }

    // move to front (recently used)
    lru.erase(store[key].second);
    lru.push_front(key);
    store[key].second = lru.begin();

    return store[key].first;
}




void KVStore::del(const std::string &key) {
    std::lock_guard<std::mutex> lock(mtx);

    if (store.find(key) != store.end()) {
        lru.erase(store[key].second);
        store.erase(key);
    }
}




