#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

// Task structure
struct Task {
    string location;
    int emergencyType; 
    int priority;      
};

// BST Node for Task Priority Queue
struct TaskNode {
    Task task;
    TaskNode* left;
    TaskNode* right;
};

// Function to create a new TaskNode
TaskNode* createTaskNode(const Task& task) {
    TaskNode* newNode = new TaskNode{task, nullptr, nullptr};
    return newNode;
}

// Function to insert a task into the BST
TaskNode* insertTask(TaskNode* root, const Task& task) {
    if (!root) return createTaskNode(task);

    if (task.priority < root->task.priority)
        root->left = insertTask(root->left, task);
    else
        root->right = insertTask(root->right, task);

    return root;
}

// In-order traversal to process tasks based on priority
void processTasks(TaskNode* root, vector<Task>& tasks) {
    if (!root) return;
    processTasks(root->left, tasks);
    tasks.push_back(root->task); 
    processTasks(root->right, tasks);
}

// Function to delete the BST 
void deleteTree(TaskNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// Graph Node structure
struct Node {
    string name;
    double distance;
    Node* neighbor;
};

// Function to add neighbors in the graph
void addNeighbor(Node*& head, string n, double dist) {
    Node* newNode = new Node{n, dist, nullptr};
    if (!head) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->neighbor) temp = temp->neighbor;
        temp->neighbor = newNode;
    }
}

// Function to read the graph from a file
vector<Node*> readFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Unable to open file." << endl;
        return {};
    }

    vector<Node*> graph;
    Node* head = nullptr;
    Node* tail = nullptr;
    string line;

    while (getline(file, line)) {
        if (line == "nullptr") {
            if (head) {
                graph.push_back(head);
                head = nullptr;
                tail = nullptr;
            }
        } else {
            istringstream iss(line);
            string name;
            double distance;
            if (getline(iss, name, ' ') && (iss >> distance)) {
                Node* newNode = new Node{name, distance, nullptr};
                if (!head) {
                    head = newNode;
                    tail = head;
                } else {
                    tail->neighbor = newNode;
                    tail = newNode;
                }
            }
        }
    }

    if (head) graph.push_back(head);
    file.close();
    return graph;
}

// Dijkstra's algorithm to find shortest distance and path
double dijkstra(vector<Node*>& graph, string start, string end) {
    int n = graph.size();
    vector<double> dist(n, numeric_limits<double>::infinity());
    vector<int> prev(n, -1);
    vector<bool> visited(n, false);

    int startIndex = -1;
    for (int i = 0; i < n; ++i) {
        if (graph[i]->name == start) startIndex = i;
    }
    if (startIndex == -1) return -1;

    dist[startIndex] = 0;

    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) u = j;
        }
        if (dist[u] == numeric_limits<double>::infinity()) break;
        visited[u] = true;

        Node* neighbor = graph[u]->neighbor;
        while (neighbor) {
            int v = -1;
            for (int j = 0; j < n; ++j) {
                if (graph[j]->name == neighbor->name) v = j;
            }
            if (v != -1 && dist[u] + neighbor->distance < dist[v]) {
                dist[v] = dist[u] + neighbor->distance;
                prev[v] = u;
            }
            neighbor = neighbor->neighbor;
        }
    }

    int endIndex = -1;
    for (int i = 0; i < n; ++i) {
        if (graph[i]->name == end) endIndex = i;
    }
    if (endIndex == -1 || dist[endIndex] == numeric_limits<double>::infinity()) return -1;

    vector<string> path;
    for (int at = endIndex; at != -1; at = prev[at]) path.push_back(graph[at]->name);
    reverse(path.begin(), path.end());

    for (const auto& step : path) cout << step << " -> ";
    cout << "END\n";

    return dist[endIndex];
}

// Function to find nearest facility
string findNearestFacility(vector<Node*>& graph, string location, string facilityType) {
    string nearest = "";
    double minDistance = numeric_limits<double>::infinity();
    for (auto& node : graph) {
        if (node->name.find(facilityType) != string::npos) { 
            double distance = dijkstra(graph, location, node->name);
            if (distance != -1 && distance < minDistance) {
                minDistance = distance;
                nearest = node->name;
            }
        }
    }
    return nearest;
}

// Function to apply traffic factor to specific edge in memory
void applyTrafficFactor(vector<Node*>& graph, const string& start, const string& end, double trafficFactor) {
    for (auto& node : graph) {
        if (node->name == start) {
            Node* neighbor = node->neighbor;
            while (neighbor) {
                if (neighbor->name == end) {
                    neighbor->distance *= trafficFactor;
                    cout << "Traffic factor applied between " << start << " and " << end << ". New distance: " << neighbor->distance << endl;
                    return;
                }
                neighbor = neighbor->neighbor;
            }
        }
    }
    cout << "No connection found between " << start << " and " << end << endl;
}

// Main menu
void displayMenu() {
    cout << "\n--- Main Menu ---" << endl;
    cout << "1. Find Route" << endl;
    cout << "2. Emergency Handler" << endl;
    cout << "3. Traffic Handler" << endl;
    cout << "0. Exit" << endl;
}

int main() {
    vector<Node*> graph = readFromFile("map.txt");
    TaskNode* taskQueue = nullptr;

    while (true) {
        displayMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 0) {
            cout << "Exiting program." << endl;
            break;
        }

        switch (choice) {
            case 1: { // Find Route
                string start, end;
                cout << "Enter start point: ";
                cin >> start;
                cout << "Enter end point: ";
                cin >> end;
                cout << "Shortest path from " << start << " to " << end << ":\n";
                dijkstra(graph, start, end);
                break;
            }
            case 2: { // Emergency Handler
                while (true) {
                    int emergencyType, priority;
                    string location;

                    cout << "\nEnter emergency type (1=Medical, 2=Fire, 3=Crime, 0=Stop): ";
                    cin >> emergencyType;
                    if (emergencyType == 0) break;

                    cout << "Enter condition (1=Extremely Dangerous, 2=Dangerous, 3=Mildly Dangerous): ";
                    cin >> priority;

                    cout << "Enter emergency location: ";
                    cin >> location;

                    taskQueue = insertTask(taskQueue, {location, emergencyType, priority});
                }

                vector<Task> tasks;
                processTasks(taskQueue, tasks); 

                for (const Task& current : tasks) {
                    cout << "\nProcessing Task at " << current.location << " with urgency " << current.priority << endl;

                    string facility;
                    if (current.emergencyType == 1) {
                        facility = findNearestFacility(graph, current.location, "hospital");
                        cout << "Nearest hospital: " << facility << endl;
                    } else if (current.emergencyType == 2) {
                        facility = findNearestFacility(graph, current.location, "fire-station");
                        cout << "Nearest fire station: " << facility << endl;
                    } else if (current.emergencyType == 3) {
                        facility = findNearestFacility(graph, current.location, "police-station");
                        cout << "Nearest police station: " << facility << endl;
                    }

                    if (facility != "") {
                        cout << "Shortest path from " << current.location << " to " << facility << ":\n";
                        dijkstra(graph, current.location, facility);
                    } else {
                        cout << "No valid facility found or no path exists.\n";
                    }
                }

                deleteTree(taskQueue);
                break;
            }
            case 3: { // Traffic Handler
                string start, end;
                cout << "Enter start point: ";
                cin >> start;
                cout << "Enter end point: ";
                cin >> end;

                int trafficChoice;
                cout << "Select traffic condition between " << start << " and " << end << ":" << endl;
                cout << "1. Extreme Traffic (Weight = 1.75)" << endl;
                cout << "2. Traffic (Weight = 1.5)" << endl;
                cout << "3. Light Traffic (Weight = 1.25)" << endl;
                cout << "0. Exit" << endl;
                cout << "Enter your choice: ";
                cin >> trafficChoice;

                double trafficFactor = 1.0;
                switch (trafficChoice) {
                    case 1: trafficFactor = 1.75; break;
                    case 2: trafficFactor = 1.5; break;
                    case 3: trafficFactor = 1.25; break;
                    case 0: break;
                    default:
                        cout << "Invalid input. Please try again." << endl;
                        continue;
                }

                if (trafficChoice != 0) {
                    applyTrafficFactor(graph, start, end, trafficFactor);
                }

                break;
            }
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}
