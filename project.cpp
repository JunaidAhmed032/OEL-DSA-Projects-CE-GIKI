#include <iostream>
#include <string>
#include <stack>
using namespace std;

// Node structure for Linked List
struct IC {
    string name;
    string type;
    int stock;
    IC* next;
    IC(string n, string t, int s) : name(n), type(t), stock(s), next(nullptr) {}
};

// Define a struct to represent an action
struct Action {
    string type;  // "add" or "delete"
    IC* ic;       // Pointer to the IC involved

    Action(string t, IC* i) : type(t), ic(i) {}
};

class LinkedList {
private:
    IC* head;
    stack<Action> undoStack; // Tracks actions for undo
    stack<Action> redoStack; // Tracks actions for redo

public:
    LinkedList() : head(nullptr) {} // linked list to store ic s and other basic operations

    void addIC(string name, string type, int stock, bool updateUndo = true) {
        IC* newIC = new IC(name, type, stock);
        newIC->next = head;
        head = newIC;

        if (updateUndo) {
            undoStack.push(Action("add", new IC(name, type, stock))); // Push a copy to undo stack
            while (!redoStack.empty()) redoStack.pop();
        }

        cout << "IC added: " << name << "\n";
    }

    void deleteIC(string name, bool updateUndo = true) {  // delete function under ll 
        IC* temp = head;
        IC* prev = nullptr;

        while (temp && temp->name != name) {
            prev = temp;
            temp = temp->next;
        }

        if (!temp) {
            cout << "IC not found: " << name << "\n";
            return;
        }

        if (updateUndo) {
            undoStack.push(Action("delete", new IC(temp->name, temp->type, temp->stock))); // Push a copy to undo stack
            while (!redoStack.empty()) redoStack.pop();
        }

        if (prev) {
            prev->next = temp->next;
        } else {
            head = temp->next;
        }

        delete temp;
        cout << "IC deleted: " << name << "\n";
    }

    void undo() {               // undo redo using stack 
        if (undoStack.empty()) {
            cout << "Nothing to undo.\n";
            return;
        }

        Action lastAction = undoStack.top();
        undoStack.pop();

        if (lastAction.type == "add") {
            deleteIC(lastAction.ic->name, false); // Undo adding (delete without updating undo stack)
        } else if (lastAction.type == "delete") {
            addIC(lastAction.ic->name, lastAction.ic->type, lastAction.ic->stock, false); // Undo deleting (add without updating undo stack)
        }

        redoStack.push(lastAction);
        cout << "Undo successful.\n";
    }

    void redo() {
        if (redoStack.empty()) {
            cout << "Nothing to redo.\n";
            return;
        }

        Action lastRedo = redoStack.top();
        redoStack.pop();

        if (lastRedo.type == "add") {
            addIC(lastRedo.ic->name, lastRedo.ic->type, lastRedo.ic->stock, false); // Redo adding
        } else if (lastRedo.type == "delete") {
            deleteIC(lastRedo.ic->name, false); // Redo deleting
        }

        undoStack.push(lastRedo);
        cout << "Redo successful.\n";
    }

    void display() {
        IC* temp = head;
        if (!temp) {
            cout << "Catalog is empty.\n";
            return;
        }

        while (temp) {
            cout << "Name: " << temp->name << ", Type: " << temp->type << ", Stock: " << temp->stock << "\n";
            temp = temp->next;
        }
    }

    void borrowIC(string name) {   // borrow using stack 
        IC* temp = head;

        while (temp) {
            if (temp->name == name) {
                if (temp->stock > 0) {
                    temp->stock--;
                    cout << "IC borrowed successfully: " << name << "\n";
                    return;
                } else {
                    cout << "Stock unavailable for IC: " << name << "\n";
                    return;
                }
            }
            temp = temp->next;
        }

        cout << "IC not found in the catalog: " << name << "\n";
    }
// Sort ICs by name using selection sort
    void sortCatalog() {
        if (!head || !head->next) return; // If the list is empty or has only one element

        for (IC* i = head; i && i->next; i = i->next) {
            IC* minNode = i;
            for (IC* j = i->next; j; j = j->next) {
                if (j->name < minNode->name) {
                    minNode = j;
                }
            }
            // Swap data of the current node and the minNode
            swap(i->name, minNode->name);
            swap(i->type, minNode->type);
            swap(i->stock, minNode->stock);
        }
        cout << "Catalog sorted by name.\n";
    }

};

// Node structure for Binary Search Tree
struct BSTNode {
    string name;
    string type;
    int stock;
    BSTNode* left;
    BSTNode* right;

    BSTNode(string n, string t, int s) : name(n), type(t), stock(s), left(nullptr), right(nullptr) {}
};

// Binary Search Tree for IC Search  ( searching through already sorted data)
class BST {
private:
    BSTNode* root;

    BSTNode* insert(BSTNode* node, string name, string type, int stock) {
        if (!node) return new BSTNode(name, type, stock);

        if (name < node->name)
            node->left = insert(node->left, name, type, stock);
        else if (name > node->name)
            node->right = insert(node->right, name, type, stock);

        return node;
    }

    void inOrderTraversal(BSTNode* node) {
        if (!node) return;

        inOrderTraversal(node->left);
        cout << "Name: " << node->name << ", Type: " << node->type << ", Stock: " << node->stock << "\n";
        inOrderTraversal(node->right);
    }

    BSTNode* search(BSTNode* node, const string& name) {
        if (!node || node->name == name) return node;

        if (name < node->name)
            return search(node->left, name);
        return search(node->right, name);
    }

public:
    BST() : root(nullptr) {}

    void addIC(string name, string type, int stock) {
        root = insert(root, name, type, stock);
        cout << "IC added to BST: " << name << "\n";
    }

    void display() {
        cout << "\n--- IC Catalog (BST In-Order) ---\n";
        inOrderTraversal(root);
    }

    void searchIC(const string& name) {
        BSTNode* result = search(root, name);
        if (result)
            cout << "Found IC - Name: " << result->name << ", Type: " << result->type << ", Stock: " << result->stock << "\n";
        else
            cout << "IC not found: " << name << "\n";
    }
};

// Main program and BST logic are unchanged
void displayMenu() {
    cout << "\n--- IC Management System ---\n";
    cout << "1. Add New IC\n";
    cout << "2. Delete an IC\n";
    cout << "3. Display IC Catalog\n";
    cout << "4. Borrow IC\n";
    cout << "5. Sort IC Catalog\n";
    cout << "6. Search IC\n";
    cout << "7. Undo Last Action\n";
    cout << "8. Redo Last Action\n";
    cout << "9. Exit\n";
}

int main() {
    LinkedList catalog;
	BST bstCatalog; 
    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
            case 1: {
                string name, type;
                int stock;
                cout << "Enter IC name: ";
                getline(cin, name);
                cout << "Enter IC type: ";
                getline(cin, type);
                cout << "Enter stock quantity: ";
                cin >> stock;
                cin.ignore();

                catalog.addIC(name, type, stock);
                break;
            }
            case 2: {
                string name;
                cout << "Enter IC name to delete: ";
                getline(cin, name);
                catalog.deleteIC(name);
                break;
            }
            case 3:
                catalog.display();
                break;
            case 4: {
                string name;
                cout << "Enter IC name to borrow: ";
                getline(cin, name);
                catalog.borrowIC(name);
                break;
            }
            case 5:
                catalog.sortCatalog();
                cout<<"Catalog Sorted";
                break;
            case 6: {
                string name;
                cout << "Enter IC name to search: ";
                getline(cin, name);
                bstCatalog.searchIC(name); // Call on the BST instance
                break;
            }
            case 7:
                catalog.undo();
                break;
            case 8:
                catalog.redo();
                break;
            case 9:
                cout << "Exiting the system.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 9);

    return 0;
}
