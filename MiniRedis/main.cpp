#include <iostream>
#include "store/KVStore.h"
#include "parser/CommandParser.h"

using namespace std;

int main() {
    KVStore kv;
    string input;

    cout << "Mini Redis CLI started (type EXIT to quit)\n";

    while (true) {
        cout << ">> ";
        getline(cin, input);

        if (input == "EXIT") break;

        vector<string> tokens = CommandParser::parse(input);

        if (tokens.empty()) continue;

        string command = tokens[0];

        if (command == "SET" && tokens.size() == 3) {
            kv.set(tokens[1], tokens[2]);
            cout << "OK\n";
        }
        else if (command == "GET" && tokens.size() == 2) {
            cout << kv.get(tokens[1]) << endl;
        }
        else if (command == "DEL" && tokens.size() == 2) {
            kv.del(tokens[1]);
            cout << "Deleted\n";
        }
        else {
            cout << "Invalid Command\n";
        }
    }

    return 0;
}