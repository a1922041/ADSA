#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* 
- NEVER_USED: slot has never held a key
 - TOMBSTONE: slot used to have a key but was deleted
 - OCCUPIED: slot currently holds a key
  */


struct Slot {
    string key;
    enum Status { NEVER_USED, TOMBSTONE, OCCUPIED } status;
    Slot() : key(""), status(NEVER_USED) {} // default constructor
};

int main() {
    // hash table always has 26 slots (a to z)
    vector<Slot> table(26);

    //Read entire input 
    string line;
    getline(cin, line);

    // Split the input line into individual commands by separated spaces
    vector<string> moves;
    string temp;
    for (char c : line) {
        if (c == ' ') {
            if (!temp.empty()) {
                moves.push_back(temp);
                temp.clear();
            }
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) moves.push_back(temp);

    //go through  each command 
    for (const string &move : moves) {
        if (move.size() < 2) continue; // skip invalid
        char op = move[0];             // a for add, d for delete
        string key = move.substr(1);   // extract the word
        int index = key.back() - 'a';  // hash value = last character




        // search
        // Check if the key already exists in the table
        bool exists = false;
        int foundIndex = -1;
        for (int i = 0; i < 26; i++) {
            int pos = (index + i) % 26; 
            if (table[pos].status == Slot::NEVER_USED) {
                // Never used slot means the key  isn’t in the table
                break;
            }
            if (table[pos].status == Slot::OCCUPIED && table[pos].key == key) {
                // Found key!
                exists = true;
                foundIndex = pos;
                break;
            }
            // If it's a tombstone or different key =  keep searching
        }

        //insert 
        if (op == 'A') {
            if (exists) continue; // nothing if the key already exists

            // Trying to find the first available slot (NEVER_USED or TOMBSTONE)
            for (int i = 0; i < 26; i++) {
                int pos = (index + i) % 26;
                if (table[pos].status != Slot::OCCUPIED) {
                    table[pos].key = key;
                    table[pos].status = Slot::OCCUPIED;
                    break;
                }
            }
        }

        //delete
        else if (op == 'D') {
            if (!exists) continue; // Do nothing if the key is not found

            //mark slot as a tombstone (deleted)
            table[foundIndex].status = Slot::TOMBSTONE;
            table[foundIndex].key.clear(); 
        }
    }

    //output
    // Print  in order from a to z
    bool first = true;
    for (int i = 0; i < 26; i++) {
        if (table[i].status == Slot::OCCUPIED) {
            if (!first) cout << " ";
            cout << table[i].key;
            first = false;
        }
    }
    cout << endl;

    return 0;
}
