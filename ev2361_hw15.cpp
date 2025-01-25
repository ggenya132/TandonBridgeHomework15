#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// Node to represent each person
struct Node {
    string name;
    double amountPaid;
    double balance;
    Node* next;
};

// Linked list to manage the participants
class LinkedList {
   private:
    Node* head;

   public:
    LinkedList() : head(NULL) {}

    void addPerson(const string& name, double amount) {
        Node* newNode = new Node;
        newNode->name = name;
        newNode->amountPaid = amount;
        newNode->balance = 0.0;
        newNode->next = head;
        head = newNode;
    }

    void calculateBalances() {
        double total = 0.0;
        int count = 0;
        Node* temp = head;

        while (temp) {
            total += temp->amountPaid;
            count++;
            temp = temp->next;
        }

        double targetAmount = total / count;

        temp = head;
        while (temp) {
            temp->balance = temp->amountPaid - targetAmount;
            temp = temp->next;
        }
    }

    void printReimbursements() {
        Node* giver = head;
        Node* receiver = head;

        while (giver) {
            if (giver->balance < 0) {
                while (receiver) {
                    if (receiver->balance > 0) {
                        double amount = min(-giver->balance, receiver->balance);
                        giver->balance += amount;
                        receiver->balance -= amount;
                        cout << giver->name << ", you give " << receiver->name
                             << " $" << fixed << setprecision(6) << amount
                             << endl;
                        if (giver->balance == 0) break;
                    }
                    receiver = receiver->next;
                }
                receiver = head;
            }
            giver = giver->next;
        }
    }

    void displayFinalBalance() {
        Node* temp = head;
        if (temp) {
            cout << "In the end, you should all have spent around $" << fixed
                 << setprecision(2) << (temp->amountPaid - temp->balance)
                 << endl;
        }
    }

    void freeList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    // This doesn't seem to be working?
    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    LinkedList list;
    string filename;

    cout << "Enter the filename: ";
    cin >> filename;

    ifstream file(filename.c_str());
    if (!file) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    double amount;
    string firstName, lastName, extraName;
    while (file >> amount >> firstName >> lastName) {
        string fullName = firstName + " " + lastName;
        if (file.peek() != '\n' && file >> extraName) {
            fullName += " " + extraName;
        }
        list.addPerson(fullName, amount);
    }

    file.close();

    list.calculateBalances();
    list.printReimbursements();
    list.displayFinalBalance();

    list.freeList();
    return 0;
}
