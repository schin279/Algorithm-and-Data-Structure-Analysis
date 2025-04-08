#include <iostream>
#include <string>
using namespace std;

// hash table class
class HashTable {
private:
    // slot structure for hash table
    struct Slot {
        string key;
        string status;

        Slot() : status("never used") {} // initialise status as "never used"
    };

    static const int size = 26;
    Slot table[size];

public:
    /* searching works as follows:
        1. given a key, take its last character as the hash value. 
        2. first, try corresponding table slot, if objective key is there, it is found.
        3. if "never used", terminate.
        4. if "occupied", but not objective or if "tombstone, move to next slot (may need to wrap around table if current slot is the last one).
        5. keep trying until key is found or key does not exist."*/

    // function to search for a key in the hash table
    bool search(const string& key) {
        int hv = key.back() - 'a';// hash value uses the last character

        while(table[hv].status != "never used") {
            if (table[hv].status == "occupied" && table[hv].key == key) {
                return true; // key found
            }
            // move to the next slot
            hv = (hv + 1) % size;
        }
        return false; // key does not exist
    }

    /* insertion works as follows:
        1. first, perform searching to ensure that key does not exist.
        2. if already exists, do nothing.
        3. else, take hash value.
        4. if slot is not occupied ("never used" or "tombstone"), insert key (slot becomes "occupied").
        5. if slot is already occupied, try next slot.
        6. repeat until unoccupied slot is found.*/

    // function to insert a key into the hash table
    void insert(const std::string& key) {
        int hv = key.back() - 'a';// hash value uses the last character

        // check if key already exists
        if (search(key)) {
            return; // key already exists, do nothing
        }

        // find next available slot
        while (table[hv].status != "never used" && table[hv].status != "tombstone") {
            hv = (hv + 1) % size; // linear probing
        }

        // insert key in available slot
        table[hv].key = key;
        table[hv].status = "occupied";
    }

    /* deletion works as follows:
        1. search for the slot with the given key.
        2. if key is not in table, do nothing.
        3. once key is found, change slot status to "tombstone".*/

    // deletion function to remove a key from the table
    void remove(const std::string& key) {
        // if key is not in table,
        if (!search(key)) {
            return; // do noting
        }

        int hv = key.back() - 'a';// hash value uses the last character

        while (table[hv].status != "never used") {
            if (table[hv].status == "occupied" && table[hv].key == key) {
                // key found, change slot status to "tombstone"
                table[hv].status = "tombstone";
                return;
            }
            // move to the next slot
            hv = (hv + 1) % size;
        }
    }

    // print keys in order of 'a' to 'z'
    void printKeys() {
        // go through slots from a to z
        for (char c = 'a'; c <= 'z'; ++c) {
            int hv = c - 'a';
            // output all keys separated by space
            if (table[hv].status == "occupied") {
                cout << table[hv].key << " ";
            }
        }
        cout << endl;
    }
};

int main() {
    HashTable table;

    // takes one line as input containing n "modification moves" separated by spaces
    string input;
    getline(std::cin, input);

    size_t pos = 0;
    while (pos < input.size()) {
        // AWord: insert key into hash table
        if (input[pos] == 'A') {
            pos += 1;
            string key;
            while (pos < input.size() && input[pos] != ' ') {
                key += input[pos];
                pos++;
            }
            table.insert(key);
        // DWord: delete key from the hash table
        } else if (input[pos] == 'D') {
            pos += 1;
            string key;
            while (pos < input.size() && input[pos] != ' ') {
                key += input[pos];
                pos++;
            }
            table.remove(key);
        }
        pos++;
    }

    table.printKeys();

    return 0;
}