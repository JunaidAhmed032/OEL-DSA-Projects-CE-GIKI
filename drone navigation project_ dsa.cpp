//group members
// zainab khan 2023778
//uzair ali 2023548
//iman zeb 2023259



#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits.h>
#include <algorithm>

using namespace std;

struct Node {
    vector<pair<int, int>> neighbors; // Pair<neighbor_id, weight>
    bool noFlyZone;                   // True if this node is a no-fly zone
};

// Global variables
vector<Node> graph;  // Adjacency list for the graph

void addEdge(int u, int v, int weight) {
    graph[u].neighbors.push_back({v, weight});
    graph[v].neighbors.push_back({u, weight}); // Undirected graph
}

void removeEdge(int u, int v) {
    graph[u].neighbors.erase(
        remove_if(graph[u].neighbors.begin(), graph[u].neighbors.end(),
                  [v](pair<int, int> p) { return p.first == v; }),
        graph[u].neighbors.end());

    graph[v].neighbors.erase(
        remove_if(graph[v].neighbors.begin(), graph[v].neighbors.end(),
                  [u](pair<int, int> p) { return p.first == u; }),
        graph[v].neighbors.end());
}

void toggleNoFlyZone(int nodeId) {
    graph[nodeId].noFlyZone = !graph[nodeId].noFlyZone;
    if (graph[nodeId].noFlyZone) {
        cout << "Node " << nodeId << " marked as a no-fly zone." << endl;
    } else {
        cout << "Node " << nodeId << " is no longer a no-fly zone." << endl;
    }
}

void shortestPath(int start, int end) {
    int n = graph.size();
    vector<int> dist(n, INT_MAX);
    vector<int> parent(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    pq.push({0, start});
    dist[start] = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (graph[u].noFlyZone) continue;

        for (auto neighbor : graph[u].neighbors) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (!graph[v].noFlyZone && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
                parent[v] = u;
            }
        }
    }

    if (dist[end] == INT_MAX) {
        cout << "No path exists between nodes " << start << " and " << end << endl;
        return;
    }

    cout << "Shortest Path Distance: " << dist[end] << endl;
    stack<int> path;
    for (int at = end; at != -1; at = parent[at]) {
        path.push(at);
    }

    cout << "Shortest Path: ";
    while (!path.empty()) {
        cout << path.top() << " ";
        path.pop();
    }
    cout << endl;
}

void avoidCollisionBFS(int start) {
    int n = graph.size();
    if (start < 0 || start >= n) {
        cout << "Invalid starting node!" << endl;
        return;
    }

    if (graph[start].noFlyZone) {
        cout << "Starting node " << start << " is in a no-fly zone. Cannot proceed." << endl;
        return;
    }

    vector<bool> visited(n, false);
    queue<int> q;

    q.push(start);
    visited[start] = true;

    cout << "Collision avoidance traversal starting from node " << start << ":\n";

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        cout << "Processing node " << u << endl;

        for (auto neighbor : graph[u].neighbors) {
            int v = neighbor.first;

            if (graph[v].noFlyZone) {
                cout << "Obstacle detected at node " << v << " (No-Fly Zone). Skipping this node." << endl;
            } else if (!visited[v]) {
                q.push(v);
                visited[v] = true;
                cout << "Safe to visit node " << v << endl;
            }
        }
    }

    cout << "Traversal complete.\n";
}

int main() {
    int nodes = 6; // Total nodes in the graph
    graph.resize(nodes);

    for (int i = 0; i < nodes; i++) {
        graph[i].noFlyZone = false;
    }

    // Add edges to the graph
    addEdge(0, 1, 4);
    addEdge(0, 2, 2);
    addEdge(1, 2, 1);
    addEdge(1, 3, 5);
    addEdge(2, 3, 8);
    addEdge(3, 4, 6);
    addEdge(4, 5, 3);
    addEdge(2, 5, 7);

    int choice;
    do {
        cout << "\n--- Drone Navigation System ---" << endl;
        cout << "1. Find Shortest Path" << endl;
        cout << "2. Avoid Collisions using BFS" << endl;
        cout << "3. Toggle No-Fly Zone" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int start, end;
                cout << "Enter start node (0 to " << nodes - 1 << "): ";
                cin >> start;
                cout << "Enter end node (0 to " << nodes - 1 << "): ";
                cin >> end;
                shortestPath(start, end);
                break;
            }
            case 2: {
                int start;
                cout << "Enter starting node for BFS (0 to " << nodes - 1 << "): ";
                cin >> start;
                avoidCollisionBFS(start);
                break;
            }
            case 3: {
                int nodeId;
                cout << "Enter node ID to toggle no-fly zone (0 to " << nodes - 1 << "): ";
                cin >> nodeId;
                toggleNoFlyZone(nodeId);
                break;
            }
            case 4:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}
