#include <iostream>
#include "store/KVStore.h"

int main() {
    KVStore kv;

    kv.set("name", "Jayant");

    std::cout << kv.get("name") << std::endl;

    kv.del("name");

    std::cout << kv.get("name") << std::endl;

    return 0;
}