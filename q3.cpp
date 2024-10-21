#include <iostream>
#include <vector>
#include <cmath>

class HashTable {
public:
    HashTable(int initialSize = 7) : size(initialSize), threshold(0.7) {
        table.resize(size, -1);
    }

    void insert(int key) {
        if (static_cast<double>(occupied) / size >= threshold) {
            resize();
        }

        if (search(key) != -1) {
            std::cout << "Duplicate key insertion is not allowed" << std::endl;
            return;
        }

        int index = key % size;
        int probes = 0;
        while (table[index] != -1 && table[index] != key) {
            index = (index + probes * probes) % size;
            probes++;
            if (probes > size) {
                std::cout << "Max probing limit reached!" << std::endl;
                return;
            }
        }
        table[index] = key;
        occupied++;
    }

    void remove(int key) {
        int index = search(key);
        if (index == -1) {
            std::cout << "Element not found" << std::endl;
            return;
        }
        table[index] = -1;
        occupied--;
    }

    int search(int key) {
        int index = key % size;
        int probes = 0;
        while (table[index] != -1) {
            if (table[index] == key) {
                return index;
            }
            index = (index + probes * probes) % size;
            probes++;
            if (probes > size) {
                return -1;
            }
        }
        return -1;
    }

    void printTable() {
        for (int i = 0; i < size; i++) {
            if (table[i] == -1) {
                std::cout << "- ";
            } else {
                std::cout << table[i] << " ";
            }
        }
        std::cout << std::endl;
    }

private:
    void resize() {
        int newSize = findNextPrime(size * 2);
        std::vector<int> newTable(newSize, -1);
        for (int i = 0; i < size; i++) {
            if (table[i] != -1) {
                int newIndex = table[i] % newSize;
                int probes = 0;
                while (newTable[newIndex] != -1) {
                    newIndex = (newIndex + probes * probes) % newSize;
                    probes++;
                }
                newTable[newIndex] = table[i];
            }
        }
        size = newSize;
        table = std::move(newTable);
        occupied = 0;
    }

    int findNextPrime(int n) {
        while (!isPrime(n)) {
            n++;
        }
        return n;
    }

    bool isPrime(int n) {
        if (n <= 1) {
            return false;
        }
        for (int i = 2; i <= std::sqrt(n); i++) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }

    std::vector<int> table;
    int size;
    double threshold;
    int occupied = 0;
};

int main() {
    HashTable ht(7); // Create a hash table with an initial size of 7

    // Insert some keys
    ht.insert(10);
    ht.insert(20);
    ht.insert(5);
    ht.insert(15);
    ht.insert(17); // Should cause a collision

    // Print the current state of the hash table
    ht.printTable();

    // Search for a key
    int index = ht.search(5);
    if (index != -1) {
        std::cout << "Key 5 found at index: " << index << std::endl;
    } else {
        std::cout << "Key 5 not found" << std::endl;
    }

    // Remove a key
    ht.remove(20);
    ht.printTable();

    // Insert more keys to trigger a resize
    ht.insert(26);
    ht.insert(35);
    ht.printTable(); // Should resize after crossing the threshold

    return 0;
}

