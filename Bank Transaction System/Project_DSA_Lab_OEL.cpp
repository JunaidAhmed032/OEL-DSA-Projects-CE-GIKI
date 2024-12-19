//2023670
//2023475
//2023624

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Transactions {
public:
    string type;
    int amount;

    Transactions() : type(""), amount(0) {}

    Transactions(string type, int amount) {
        this->type = type;
        this->amount = amount;
    }
};

class Loan {
public:
    long loanAmount;
    float interestRate;
    int tenure; 
    bool isActive;

    Loan() : loanAmount(0), interestRate(0.0), tenure(0), isActive(false) {}

    Loan(long loanAmount, float interestRate, int tenure) {
        this->loanAmount = loanAmount;
        this->interestRate = interestRate;
        this->tenure = tenure;
        this->isActive = true;
    }

    void displayLoanDetails() {
        if (isActive) {
            cout << "Loan Amount: $" << loanAmount << endl;
            cout << "Interest Rate: " << interestRate << "%" << endl;
            cout << "Tenure: " << tenure << " months" << endl;
        }
        else {
            cout << "No active loan." << endl;
        }
    }
};

// Queue for transaction history
class Queue {
public:
    struct QueueNode {
        Transactions transaction;
        QueueNode* next;
    };

    QueueNode* front;
    QueueNode* back;

    Queue() : front(nullptr), back(nullptr) {}

    void push(Transactions trans) {
        QueueNode* newNode = new QueueNode{ trans, nullptr };
        if (back == nullptr) {
            front = back = newNode;
        }
        else {
            back->next = newNode;
            back = newNode;
        }
    }

    Transactions pop() {
        if (front == nullptr) {
            throw runtime_error("Queue is empty");
        }
        Transactions trans = front->transaction;
        QueueNode* temp = front;
        front = front->next;
        delete temp;
        return trans;
    }

    bool empty() {
        return front == nullptr;
    }

    void clear() {
        while (!empty()) {
            pop();
        }
    }
};

class ListNode {
public:
    long acc_num;
    string name;
    string address;
    long long ph_num;
    long amount;
    int pin;
    bool isFrozen;
    bool fraudDetected;
    int failedPinAttempts;
    Queue transactionHistory;
    Loan loan;
    ListNode* next;

    ListNode(long acc_num, string name, string address, long long ph_num, long amount, int pin) {
        this->acc_num = acc_num;
        this->name = name;
        this->address = address;
        this->ph_num = ph_num;
        this->amount = amount;
        this->pin = pin;
        this->isFrozen = false;
        this->fraudDetected = false;
        this->failedPinAttempts = 0;
        this->next = nullptr;
    }

    void resetFailedPinAttempts() {
        failedPinAttempts = 0;
    }

    void incrementFailedPinAttempts() {
        failedPinAttempts++;
    }

    long getBalance() const {
        return amount + loan.loanAmount;
    }

    void displayLoanDetails() {
        loan.displayLoanDetails();
    }
};

class LinkedList {
public:
    ListNode* head;

    LinkedList() : head(nullptr) {}

    void pushFront(ListNode* newNode) {
        newNode->next = head;
        head = newNode;
    }

    ListNode* find(long acc_num) {
        ListNode* current = head;
        while (current != nullptr) {
            if (current->acc_num == acc_num)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    void display() {
        ListNode* current = head;
        int i = 1;
        while (current != nullptr) {
            cout << "Client " << i << " Details" << endl;
            cout << "Account Number -> " << current->acc_num << endl;
            cout << "Account Holder's Name -> " << current->name << endl;
            cout << "Account Holder's Address -> " << current->address << endl;
            cout << "Account Holder's Contact Information -> " << current->ph_num << endl;
            cout << "Account Status -> " << (current->isFrozen ? "Frozen" : "Active") << endl;
            current = current->next;
            i++;
        }
    }
};

// Hashmap for storing data under the account for efficiency having O(1)
class HashMap {
private:
    struct HashNode {
        long key;
        ListNode* value;
        HashNode* next;
    };

    vector<HashNode*> table;
    int capacity;

    int hashFunction(long key) {
        return key % capacity;
    }

public:
    HashMap(int size) : capacity(size) {
        table.resize(capacity, nullptr);
    }

    void insert(long key, ListNode* value) {
        int index = hashFunction(key);
        HashNode* newNode = new HashNode{ key, value, nullptr };

        if (!table[index]) {
            table[index] = newNode;
        }
        else {
            HashNode* current = table[index];
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    ListNode* find(long key) {
        int index = hashFunction(key);
        HashNode* current = table[index];

        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        return nullptr;
    }

    bool exists(long key) {
        return find(key) != nullptr;
    }

    ~HashMap() {
        for (auto& head : table) {
            while (head) {
                HashNode* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }
};

//  For finding accounts for having sorted sortage which depends on the account number
class BST {
private:
    struct Node {
        long key;
        Node* left;
        Node* right;
        Node(long k) : key(k), left(nullptr), right(nullptr) {}
    };
    Node* root;

    void insert(Node*& node, long key) {
        if (!node) {
            node = new Node(key);
        }
        else if (key < node->key) {
            insert(node->left, key);
        }
        else {
            insert(node->right, key);
        }
    }

    bool exists(Node* node, long key) {
        if (!node) return false;
        if (node->key == key) return true;
        return key < node->key ? exists(node->left, key) : exists(node->right, key);
    }

public:
    BST() : root(nullptr) {}

    void insert(long key) {
        insert(root, key);
    }

    bool exists(long key) {
        return exists(root, key);
    }

    ~BST() {
        
    }
};

class BankingSystem {
private:
    LinkedList accountList;
    HashMap accountHash; 
    BST bst; 

public:
    BankingSystem() : accountHash(100) {} // Initialize HashMap with a size of 100

    void addCustomer(long acc_num, string name, string address, long long ph_num, long amount, int pin) {
        ListNode* newNode = new ListNode(acc_num, name, address, ph_num, amount, pin);
        accountList.pushFront(newNode);
        accountHash.insert(acc_num, newNode);
        bst.insert(acc_num); // Insert account number into BST
        cout << "\nCongratulations!!!\nYour Account has been created! \n" << endl;
    }

    void displayCustomers() {
        accountList.display();
    }

    void performTransaction(long acc_num) {
        ListNode* accountNode = accountHash.find(acc_num);
        // Perform fraud detection check before proceeding
        if (accountNode->fraudDetected) {
            cout << "Fraud detected in the account! Transaction not allowed." << endl;
            return;
        }

        if (accountNode->isFrozen) {
            cout << "Account is frozen. No transactions can be performed." << endl;
            char y = '1';
            cout << "If you want to unfreeze account, please enter 1:" << endl;
            cin >> y;
            if (y == '1') {
                accountNode->isFrozen = false;
                cout << "Account has been unfrozen successfully!" << endl;
            }
            else {
                return;
            }
        }

        int maxAttempts = 3;
        int enteredPin;

        for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
            cout << "Enter your PIN (Attempt " << attempt << "/" << maxAttempts << "): ";
            cin >> enteredPin;

            // Check if entered PIN is correct
            if (enteredPin == accountNode->pin) {
                cout << "PIN verification successful. Proceed ing with the transaction." << endl;
                accountNode->resetFailedPinAttempts();
                break;
            }
            else {
                cout << "Incorrect PIN. ";
                accountNode->incrementFailedPinAttempts();

                if (accountNode->failedPinAttempts >= 3) {
                    accountNode->fraudDetected = true;  // Fraud detected  after 3 failed attempts and the accound will be freeze and not be accesible(until visit physical bank)
                    cout << "Multiple failed attempts detected. Account is flagged for fraud!" << endl;
                    return;
                }

                if (attempt < maxAttempts) {
                    cout << "Please try again." << endl;
                }
                else {
                    cout << "Maximum attempts reached. Exiting program." << endl;
                    return;
                }
            }
        }

        int opt2;
        cout << "1. Cash Deposit " << endl;
        cout << "2. Cash Withdrawal " << endl;
        cout << "3. Fund Transfer " << endl;
        cout << "4. Apply for Loan" << endl; 
        cout << "5. Freeze Account" << endl; 
        cout << "6. Exit" << endl;
        cout << "Enter option: ";
        cin >> opt2;

        Transactions temp;

        switch (opt2) {
        case 1:
            // Cash Deposit
            cout << "Enter the amount you want to deposit: ";
            int deposit;
            cin >> deposit;
            accountNode->amount += deposit;
            cout << "Amount $" << deposit << " deposited successfully!\nYour current balance is: $" << accountNode->amount << endl;
            temp = Transactions("DEPOSIT", deposit);
            accountNode->transactionHistory.push(temp);
            break;

        case 2:
            // Cash Withdrawal
            cout << "Enter the amount you want to withdraw: ";
            int withdraw;
            cin >> withdraw;
            if (accountNode->amount >= withdraw) {
                accountNode->amount -= withdraw;
                cout << "Amount $" << withdraw << " withdrawn successfully!\nYour current balance is: $" << accountNode->amount << endl;
                temp = Transactions("WITHDRAW", withdraw);
                accountNode->transactionHistory.push(temp);
            }
            else {
                cout << "Insufficient Balance for the withdrawal!" << endl;
                return;
            }
            break;

        case 3: {
            // Fund Transfer
            cout << "Enter the account number of the receiver: ";
            long receiver_acc_num;
            cin >> receiver_acc_num;

            // Check if the receiver account exists
            if (!bst.exists(receiver_acc_num)) {
                cout << "Receiver account not found!" << endl;
                return;
            }

            
            if (receiver_acc_num == acc_num) {
                cout << "Cannot transfer funds to the same account!" << endl;
                return;
            }

            // Check if the sender has sufficient balance for the transfer
            cout << "Enter the amount you want to transfer: ";
            int transfer_amount;
            cin >> transfer_amount;

            if (transfer_amount > 0 && accountNode->amount >= transfer_amount) {
                accountNode->amount -= transfer_amount;
                cout << "Amount $" << transfer_amount << " transferred successfully!\nYour current balance is: $" << accountNode->amount << endl;
                temp = Transactions("TRANSFER", transfer_amount);
                accountNode->transactionHistory.push(temp);

                // Update the receiver's account balance
                ListNode* receiverNode = accountHash.find(receiver_acc_num);
                receiverNode->amount += transfer_amount;

                cout << "Funds received from account number " << acc_num << ". \nReceiver's new balance is: $" << receiverNode->amount << endl;
            }
            else {
                cout << "Invalid amount or insufficient balance for the transfer!" << endl;
                return;
            }
            break;
        }

        case 4:
            // Apply for Loan
            long loanAmount;
            float interestRate;
            int tenure;
            cout << "Enter loan amount: ";
            cin >> loanAmount;
            cout << "Enter interest rate (in %): ";
            cin >> interestRate;
            cout << "Enter tenure (in months): ";
            cin >> tenure;

            if (loanAmount > 0) {
                accountNode->loan = Loan(loanAmount, interestRate, tenure);
                cout << "Loan of $" << loanAmount << " applied successfully!" << endl;
            }
            else {
                cout << "Invalid loan amount!" << endl;
            }
            break;

        case 5:
            // Freeze Account
            accountNode->isFrozen = true;
            cout << "Account has been frozen successfully!" << endl;
            break;

        default:
            cout << "Invalid option" << endl;
            break;
        }
    }

    void displayLoanDetails(long acc_num) {
        ListNode* accountNode = accountHash.find(acc_num);
        accountNode->displayLoanDetails();
    }

    void displayTransactionHistory(long acc_num) {
        ListNode* accountNode = accountHash.find(acc_num);
        cout << "Transaction history for Account Number " << acc_num << " is:" << endl;
        while (!accountNode->transactionHistory.empty()) {
            Transactions record = accountNode->transactionHistory.pop();
            cout << "Transaction Type: " << record.type << ", Amount: $" << record.amount << endl;
        }
    }

    bool verifyAccount(long acc_num) {
        return bst.exists(acc_num);
    }

    ListNode* getAccountNode(long acc_num) {
        return accountHash.find(acc_num);
    }

    void fraudDetection(long acc_num) {
        ListNode* accountNode = accountHash.find(acc_num);

        if (accountNode->fraudDetected) {
            cout << "Fraud detected in account number " << acc_num << ". Account is frozen and transactions are halted!" << endl;
        }
        else {
            cout << "No fraud detected for account number " << acc_num << "." << endl;
        }
    }
};

int main() {
    BankingSystem bankingSystem;

    long acc_num;
    string name, address;
    long long ph_num;
    long amount;
    int pin;
    int opt;

    do {
        cout << "WELCOME TO SIMULATED BANKING SYSTEM" << endl;
        cout << "==== MENU ====" << endl;
        cout << "1. Open New Account" << endl;
        cout << "2. Show Account Holder Details" << endl;
        cout << "3. Perform Transaction/Apply for Loan/Account Freezing";
        cout << "4. Display Transaction History" << endl;
        cout << "5. Show Current Balance" << endl;
        cout << "6. Verify Account" << endl;
        cout << "7. Show Loan Details" << endl;
        cout << "8. Fraud Detection" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter choice : "; cin >> opt;

        switch (opt) {
        case 1:
            cout << "Enter account number: "; cin >> acc_num;
            cout << "Enter your name: "; cin.ignore(); getline(cin, name);
            cout << "Enter your address: "; getline(cin, address);
            cout << "Enter your phone number: "; cin >> ph_num;
            cout << "Enter initial amount: "; cin >> amount;
            cout << "Enter Pin : "; cin >> pin;

            bankingSystem.addCustomer(acc_num, name, address, ph_num, amount, pin);
            break;

        case 2:
            bankingSystem.displayCustomers();
            break;

        case 3:
            cout << "Enter your account number: "; cin >> acc_num;
            bankingSystem.performTransaction(acc_num);
            break;

        case 4:
            cout << "Enter your account number: "; cin >> acc_num;
            bankingSystem.displayTransactionHistory(acc_num);
            break;

        case 5:
            cout << "Enter your account number: "; cin >> acc_num;
            if (bankingSystem.verifyAccount(acc_num)) {
                ListNode* accountNode = bankingSystem.getAccountNode(acc_num);
                cout << "Your current balance is: $" << accountNode->getBalance() << endl;
            }
            else {
                cout << "Account not found!" << endl;
            }
            break;

        case 6:
            cout << "Enter account number to verify: "; cin >> acc_num;
            if (bankingSystem.verifyAccount(acc_num)) {
                cout << "Account verified successfully!" << endl;
            }
            else {
                cout << "Account not found!" << endl;
            }
            break;

        case 7:
            cout << "Enter your account number: "; cin >> acc_num;
            if (bankingSystem.verifyAccount(acc_num)) {
                ListNode* accountNode = bankingSystem.getAccountNode(acc_num);
                accountNode->displayLoanDetails(); // Display loan details
            }
            else {
                cout << "Account not found!" << endl;
            }
            break;

        case 8:
            cout << "Enter your account number for fraud detection: "; cin >> acc_num;
            bankingSystem.fraudDetection(acc_num); // Fraud detection
            break;

        case 9:
            cout << "Exiting..." << endl;
            break;

        default:
            cout << "Invalid option! Please try again." << endl;
            break;
        }

    } while (opt != 9);

    return 0;
}