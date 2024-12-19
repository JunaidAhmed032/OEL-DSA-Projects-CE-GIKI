//2023231
//2023536
//2023590

#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
using namespace std;

struct Packet
{
    string source;
    string destination;
    int priority;
    int size;
    Packet *next;
    string encryptedData;

    Packet(string src, string dest, int prio, int sz, char encryptionKey = 'K')
        : source(src), destination(dest), priority(prio), size(sz), next(nullptr)
    {
        string data = source + destination + to_string(priority) + to_string(size);
        encryptedData = xorEncryptDecrypt(data, encryptionKey);
    }

    string xorEncryptDecrypt(const string& data, char encryptionKey) {
        string encryptedData = data;
        for (char &c : encryptedData) {
            c ^= encryptionKey;
        }
        return encryptedData;
    }
};

class PacketList
{
private:
    Packet *head;

public:
    PacketList() : head(nullptr) {}

    void addPacket(string source, string destination, int priority, int size)
    {
        Packet *newPacket = new Packet(source, destination, priority, size);
        newPacket->next = head;
        head = newPacket;
        cout << "Packet added successfully.\n";
    }

    void deletePacket(stack<Packet *> &undoStack)
    {
        if (!head)
        {
            cout << "No packets to delete.\n";
            return;
        }
        Packet *temp = head;
        head = head->next;

        undoStack.push(temp);
        cout << "Packet deleted and added to undo stack.\n";
    }

    void restorePacket(Packet *packet)
    {
        if (!packet)
            return;
        packet->next = head;
        head = packet;
    }

    void displayPackets()
    {
        if (!head)
        {
            cout << "No packets to display.\n";
            return;
        }
        cout << "Packets in the list:\n";
        Packet *temp = head;
        while (temp)
        {
            cout << "Source: " << temp->source
                 << ", Destination: " << temp->destination
                 << ", Priority: " << temp->priority
                 << ", Size: " << temp->size << " KB\n";
            cout << "Encrypted Data: " << temp->encryptedData << endl;
            temp = temp->next;
        }
    }

    Packet *getHead() { return head; }
};

struct BSTNode
{
    int priority;
    string source;
    BSTNode *left;
    BSTNode *right;

    BSTNode(int prio, string src) : priority(prio), source(src), left(nullptr), right(nullptr) {}
};

class PacketBST
{
private:
    BSTNode *root;

    void inOrderTraversal(BSTNode *node)
    {
        if (!node)
            return;
        inOrderTraversal(node->left);
        cout << "Priority: " << node->priority << ", Source: " << node->source << endl;
        inOrderTraversal(node->right);
    }

    bool searchByPriority(BSTNode *node, int priority)
    {
        if (!node)
            return false;
        if (node->priority == priority)
            return true;
        return (priority < node->priority) ? searchByPriority(node->left, priority) : searchByPriority(node->right, priority);
    }

    bool searchBySource(BSTNode *node, const string &source)
    {
        if (!node)
            return false;
        if (node->source == source)
            return true;
        return (source < node->source) ? searchBySource(node->left, source) : searchBySource(node->right, source);
    }

public:
    PacketBST() : root(nullptr) {}

    void insert(int priority, string source)
    {
        BSTNode *newNode = new BSTNode(priority, source);
        if (!root)
        {
            root = newNode;
            return;
        }
        BSTNode *current = root;
        BSTNode *parent = nullptr;

        while (current)
        {
            parent = current;
            if (priority < current->priority)
                current = current->left;
            else
                current = current->right;
        }

        if (priority < parent->priority)
            parent->left = newNode;
        else
            parent->right = newNode;
    }

    void display()
    {
        if (!root)
        {
            cout << "No packets in BST.\n";
            return;
        }
        cout << "Packets in BST:\n";
        inOrderTraversal(root);
    }

    bool search(int priority)
    {
        return searchByPriority(root, priority);
    }

    bool search(const string &source)
    {
        return searchBySource(root, source);
    }
};

int countHighPriorityPackets(Packet *head, int threshold)
{
    if (!head)
        return 0;
    return (head->priority > threshold ? 1 : 0) + countHighPriorityPackets(head->next, threshold);
}

void sortPackets(Packet *head)
{
    if (!head)
        return;

    bool swapped;
    do
    {
        swapped = false;
        Packet *current = head;
        while (current->next)
        {
            if (current->priority < current->next->priority)
            {
                swap(current->priority, current->next->priority);
                swap(current->source, current->next->source);
                swap(current->destination, current->next->destination);
                swap(current->size, current->next->size);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
    cout << "Packets sorted by priority.\n";
}

int main()
{
    PacketList packetList;
    PacketBST packetBST;
    stack<Packet *> undoStack, redoStack;

    int choice;
    do
    {
        cout << "\n--- Network Packet Analyzer ---\n";
        cout << "1. Add Packet\n2. Delete Packet\n3. Display Packets\n4. Undo Delete\n5. Redo Delete\n";
        cout << "6. Sort Packets\n7. Search Packets in BST\n8. Count High-Priority Packets\n9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string source, destination;
            int priority, size;
            cout << "Enter source: ";
            cin >> source;
            cout << "Enter destination: ";
            cin >> destination;
            cout << "Enter priority (higher is more important): ";
            cin >> priority;
            cout << "Enter size (in KB): ";
            cin >> size;

            packetList.addPacket(source, destination, priority, size);
            packetBST.insert(priority, source);
            while (!redoStack.empty())
                redoStack.pop();
            break;
        }
        case 2:
            packetList.deletePacket(undoStack);
            break;

        case 3:
            packetList.displayPackets();
            break;

        case 4:
        {
            if (!undoStack.empty())
            {
                Packet *restored = undoStack.top();
                undoStack.pop();
                packetList.restorePacket(restored);
                redoStack.push(restored);
                cout << "Undo successful: Packet restored.\n";
            }
            else
            {
                cout << "No deleted packets to undo.\n";
            }
            break;
        }
        case 5:
        {
            if (!redoStack.empty())
            {
                Packet *redoPacket = redoStack.top();
                redoStack.pop();
                packetList.deletePacket(undoStack);
                cout << "Redo successful: Packet deleted again.\n";
            }
            else
            {
                cout << "No undone actions to redo.\n";
            }
            break;
        }
        case 6:
            sortPackets(packetList.getHead());
            break;

        case 7:
        {
            int searchChoice;
            cout << "Search by:\n1. Priority\n2. Source\nEnter choice: ";
            cin >> searchChoice;
            if (searchChoice == 1)
            {
                int priority;
                cout << "Enter priority to search for: ";
                cin >> priority;
                if (packetBST.search(priority))
                {
                    cout << "Packet with priority " << priority << " found in BST.\n";
                }
                else
                {
                    cout << "Packet with priority " << priority << " not found.\n";
                }
            }
            else if (searchChoice == 2)
            {
                string source;
                cout << "Enter source to search for: ";
                cin >> source;
                if (packetBST.search(source))
                {
                    cout << "Packet with source " << source << " found in BST.\n";
                }
                else
                {
                    cout << "Packet with source " << source << " not found.\n";
                }
            }
            else
            {
                cout << "Invalid choice.\n";
            }
            break;
        }
        case 8:
        {
            int threshold;
            cout << "Enter priority threshold: ";
            cin >> threshold;
            cout << "Number of high-priority packets: "
                 << countHighPriorityPackets(packetList.getHead(), threshold) << endl;
            break;
        }
        case 9:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 9);

    return 0;
}