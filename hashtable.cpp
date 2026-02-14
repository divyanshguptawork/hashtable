#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

struct Student {
    char firstName[50];
    char lastName[50];
    int id;
    float gpa;
};

// node for the linked list (Chaining)
struct Node {
    Student* student;
    Node* next;
};

// global or class variables for the table
Node** hashTable; 
int currentTableSize = 100;

// simple hash function: uses the ID and table size
int hashFunction(int id, int size) {
    return id % size;
}

void addStudentToTable(Node** table, Student* s, int size) {
    int index = hashFunction(s->id, size);
    Node* newNode = new Node;
    newNode->student = s;
    newNode->next = table[index]; // insert at head of chain
    table[index] = newNode;
}

// manual ADD command logic
void manualAdd() {
    Student* s = new Student;
    cout << "Enter first name: "; cin >> s->firstName;
    cout << "Enter last name: "; cin >> s->lastName;
    cout << "Enter ID: "; cin >> s->id;
    cout << "Enter GPA: "; cin >> s->gpa;
    addStudentToTable(hashTable, s, currentTableSize);
}

//function to rehash the entire table
void rehash() {
    int oldSize = currentTableSize;
    currentTableSize *= 2; //double the size
    Node** newTable = new Node*[currentTableSize];
    for (int i=0; i < currentTableSize; i++) newTable[i] = NULL;

    //iterate through old table
    for (int i = 0; i < oldSize; i++) {
        Node* current = hashTable[i];
        while (current != NULL) {
            // Re-insert into new table using the NEW size
            addStudentToTable(newTable, current->student, currentTableSize);
            Node* temp = current;
            current = current->next;
            delete temp; // Delete the old node (but keep the Student!)
        }
    }
    delete[] hashTable;
    hashTable = newTable;
    cout << "Table resized to " << currentTableSize << " slots." << endl;
}

//update the Add function to check for chain length
void addWithCheck(Student* s) {
    addStudentToTable(hashTable, s, currentTableSize);
    
    //check chain length at the index we just added to
    int index = hashFunction(s->id, currentTableSize);
    int count = 0;
    Node* curr = hashTable[index];
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }

    if (count >= 4) {
        rehash();
    }
}

int main() {
    // initialize table with NULLs
    hashTable = new Node*[currentTableSize];
    for(int i = 0; i < currentTableSize; i++) hashTable[i] = NULL;

    return 0;
}