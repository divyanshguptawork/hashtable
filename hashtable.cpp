#include <iostream>
#include <iomanip>
#include <cstring>

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

int main() {
    // initialize table with NULLs
    hashTable = new Node*[currentTableSize];
    for(int i = 0; i < currentTableSize; i++) hashTable[i] = NULL;

    return 0;
}