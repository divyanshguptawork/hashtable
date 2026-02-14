#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

//struct defs
struct Student {
    char firstName[50];
    char lastName[50];
    int id;
    float gpa;
};

struct Node {
    Student* student;
    Node* next;
};

//globl variables
Node** hashTable;
int currentTableSize = 100;
int nextID = 1000; 

//function prototypes
int hashFunction(int id, int size);
void addStudentToTable(Node** table, Student* s, int size);
void rehash();
void checkAndRehash(int index);
void manualAdd();
void generateStudents();
void printStudents();
void deleteStudent();
void quit();

int main() {
    srand(time(NULL)); 
    
    hashTable = new Node*[currentTableSize];
    for (int i = 0; i < currentTableSize; i++) hashTable[i] = NULL;

    char command[15];
    cout << "Hash Table Student System (No-File Version)" << endl;
    cout << "Commands: ADD, GENERATE, PRINT, DELETE, QUIT" << endl;

    while (true) {
        cout << "\n>";
        cin >> command;

        if (strcmp(command, "ADD") == 0) {
            manualAdd();
        } else if (strcmp(command, "GENERATE") == 0) {
            generateStudents();
        } else if (strcmp(command, "PRINT") == 0) {
            printStudents();
        } else if (strcmp(command, "DELETE") == 0) {
            deleteStudent();
        } else if (strcmp(command, "QUIT") == 0) {
            quit();
            break;
        } else {
            cout << "Invalid command." << endl;
        }
    }
    return 0;
}

//logic

int hashFunction(int id, int size) {
    return id % size;
}

void addStudentToTable(Node** table, Student* s, int size) {
    int index = hashFunction(s->id, size);
    Node* newNode = new Node;
    newNode->student = s;
    newNode->next = table[index]; 
    table[index] = newNode;
}

void checkAndRehash(int index) {
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


void rehash() {
    int oldSize = currentTableSize;
    currentTableSize *= 2; 
    Node** newTable = new Node*[currentTableSize];
    for (int i = 0; i < currentTableSize; i++) newTable[i] = NULL;

    for (int i = 0; i < oldSize; i++) {
        Node* curr = hashTable[i];
        while (curr != NULL) {
            addStudentToTable(newTable, curr->student, currentTableSize);
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
    delete[] hashTable;
    hashTable = newTable;
    cout << "!! REHASHING COMPLETE !! New Table Size: " << currentTableSize << endl;
}

void manualAdd() {
    Student* s = new Student;
    cout << "First Name: "; cin >> s->firstName;
    cout << "Last Name: "; cin >> s->lastName;
    cout << "ID: "; cin >> s->id;
    cout << "GPA: "; cin >> s->gpa;

    addStudentToTable(hashTable, s, currentTableSize);
    checkAndRehash(hashFunction(s->id, currentTableSize));
    cout << "Student added." << endl;
}

void generateStudents() {
    int numToGenerate;
    int numNames;
    vector<string> firsts, lasts;
    string tempName;

    cout << "How many first names would you like to provide for the pool? ";
    cin >> numNames;
    for(int i = 0; i < numNames; i++) {
        cout << "Enter first name " << i+1 << ": ";
        cin >> tempName;
        firsts.push_back(tempName);
    }

    cout << "How many last names would you like to provide for the pool? ";
    cin >> numNames;
    for(int i = 0; i < numNames; i++) {
        cout << "Enter last name " << i+1 << ": ";
        cin >> tempName;
        lasts.push_back(tempName);
    }

    cout << "How many total random students should I generate from these pools? ";
    cin >> numToGenerate;

    for (int i = 0; i < numToGenerate; i++) {
        Student* s = new Student;
        // Pick random names from the vectors you just filled
        strcpy(s->firstName, firsts[rand() % firsts.size()].c_str());
        strcpy(s->lastName, lasts[rand() % lasts.size()].c_str());
        s->id = nextID++;
        s->gpa = (float)(rand() % 401) / 100.0; 

        addStudentToTable(hashTable, s, currentTableSize);
        checkAndRehash(hashFunction(s->id, currentTableSize));
    }
    cout << "Successfully generated " << numToGenerate << " students." << endl;
}

void printStudents() {
    for (int i = 0; i < currentTableSize; i++) {
        Node* curr = hashTable[i];
        while (curr != NULL) {
            cout << curr->student->firstName << " " << curr->student->lastName 
                 << " | ID: " << curr->student->id 
                 << " | GPA: " << fixed << setprecision(2) << curr->student->gpa << endl;
            curr = curr->next;
        }
    }
}

void deleteStudent() {
    int id;
    cout << "ID to delete: ";
    cin >> id;

    int index = hashFunction(id, currentTableSize);
    Node* curr = hashTable[index];
    Node* prev = NULL;

    while (curr != NULL) {
        if (curr->student->id == id) {
            if (prev == NULL) hashTable[index] = curr->next;
            else prev->next = curr->next;
            
            delete curr->student;
            delete curr;
            cout << "Student deleted." << endl;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    cout << "ID not found." << endl;
}

void quit() {
    for (int i = 0; i < currentTableSize; i++) {
        Node* curr = hashTable[i];
        while (curr != NULL) {
            Node* temp = curr;
            curr = curr->next;
            delete temp->student;
            delete temp;
        }
    }
    delete[] hashTable;
}