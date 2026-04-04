#include "KVStore.h"


long long KVStore::getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}




void KVStore::set(const std::string &key, const std::string &value, int ttl) {
    std::lock_guard<std::mutex> lock(mtx);

    long long expiry_time = -1;

    if (ttl != -1) {
        expiry_time = getCurrentTime() + ttl * 1000;
    }

    if (store.find(key) != store.end()) {
        lru.erase(store[key].it);
    }
    else if (store.size() >= capacity) {
        std::string lru_key = lru.back();
        lru.pop_back();
        store.erase(lru_key);
    }

    lru.push_front(key);
    store[key] = {value, expiry_time, lru.begin()};
}





std::string KVStore::get(const std::string &key) {
    std::lock_guard<std::mutex> lock(mtx);

    if (store.find(key) == store.end()) {
        return "NULL";
    }

    // 🔥 CHECK EXPIRY
    long long expiry = store[key].expiry;

    if (expiry != -1 && getCurrentTime() > expiry) {
        // expired → delete
        lru.erase(store[key].it);
        store.erase(key);
        return "NULL";
    }

    // move to front (LRU)
    lru.erase(store[key].it);
    lru.push_front(key);
    store[key].it = lru.begin();

    return store[key].value;
}




void KVStore::del(const std::string &key) {
    std::lock_guard<std::mutex> lock(mtx);

    if (store.find(key) != store.end()) {
        lru.erase(store[key].it);
        store.erase(key);
    }
}




