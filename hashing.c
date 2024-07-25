#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 11

int hashTable[TABLE_SIZE];

// Function to initialize the hash table
void initializeTable() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        hashTable[i] = -1; // Using -1 to indicate an empty slot
    }
}

// Primary hash function
int h1(int key) {
    int x = (key + 7) * (key + 7);
    x = x / 16;
    x = x + key;
    x = x % TABLE_SIZE;
    return x;
}

// Secondary hash function to reverse the digits of the key
int Reverse(int key) {
    int reversed = 0;
    while (key > 0) {
        reversed = reversed * 10 + key % 10;
        key /= 10;
    }
    return reversed;
}

// Function to insert a key into the hash table using double hashing
void insert(int key) {
    int homeSlot = h1(key);
    int secondHash = Reverse(key) % TABLE_SIZE;

    int i = 0;
    int index;
    do {
        index = (homeSlot + i * secondHash) % TABLE_SIZE;
        i++;
    } while (hashTable[index] != -1 && i < TABLE_SIZE);

    if (hashTable[index] == -1) {
        hashTable[index] = key;
    } else {
        printf("Error: Hash table is full\n");
    }
}

// Function to print the hash table
void printTable() {
    printf("Final Hash Table:\n");
    for (int i = 0; i < TABLE_SIZE; ++i) {
        if (hashTable[i] != -1) {
            printf("Slot %d: %d\n", i, hashTable[i]);
        } else {
            printf("Slot %d: empty\n", i);
        }
    }
}

int main() {
    initializeTable();

    int keys[] = {43, 23, 1, 0, 15, 31, 4, 7, 11, 3};
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; ++i) {
        insert(keys[i]);
    }

    printTable();

    return 0;
}
