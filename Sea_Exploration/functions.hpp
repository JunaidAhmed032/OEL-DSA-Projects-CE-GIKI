#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include "graph.hpp"
#include "stack.hpp"

using namespace std;

// for Dijkstra's Algorithm
struct Path {
    int island;
    int cost;

    // operator overloading to look cool
    bool operator>(const Path& other) const {
        return cost > other.cost;
    }
};

// Function to implement Dijkstra's algorithm to find the shortest path from startIsland to goalIsland
vector<int> dijkstra(Graph& graph, int startIsland, int goalIsland) {
    vector<int> dist(maxIslands, INT_MAX);
    vector<int> prev(maxIslands, -1);
    priority_queue<Path, vector<Path>, greater<Path>> pq; // min heap

    dist[startIsland] = 0;
    pq.push({ startIsland, 0 });

    while (!pq.empty()) {
        int u = pq.top().island;
        int uDist = pq.top().cost;
        pq.pop();

        if (uDist > dist[u]) continue;

        // Explore neighbors
        for (const auto& edge : graph.adjacencyList[u]) {
            int v = edge.to;
            int weight = edge.cost;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({ v, dist[v] });
            }
        }
    }

    // Reconstruct the path
    vector<int> path;
    for (int at = goalIsland; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

// utility function to get neighboring islands of the current island
void getNeighboringIslands(Graph& graph, int currentIsland) {
    cout << "You can travel to the following islands from " << graph.islands[currentIsland]->name << ":\n";
    for (const auto& edge : graph.adjacencyList[currentIsland]) {
        cout << graph.islands[edge.to]->name << " (Cost: " << edge.cost << ")\n";
    }
}

// Function to process user input and find the corresponding island ID by name
int getIslandIdFromName(Graph& graph, const string& islandName) {
    for (int i = 0; i < graph.islandCount; i++) {
        if (graph.islands[i] != nullptr && graph.islands[i]->name.compare(islandName) == 0) {
            return i; // island ID
        }
    }
    cout << "Island with name " << islandName << " not found.\n";
    return -1;
}
 
// Function to implement exploration in the sea
void exploreSea(Graph& graph, int& currentIsland, int& resources) {
    Stack visitedStack;
    visitedStack.push(currentIsland);  // Mark the starting island as visited
    bool runif = true;

    cout << "Exploring from " << graph.islands[currentIsland]->name << "\n";

    while (resources > 0) {
        cout << "\nCurrent Island: " << graph.islands[currentIsland]->name << " (Resources: " << resources << ")\n";

        // Get the neighboring islands
        getNeighboringIslands(graph, currentIsland);

        // Ask user to choose an island to travel to
        string islandName;
        cout << "Enter the name of the island to visit, or 0 to escape: ";

        while (runif) {
            cin.ignore();
            runif = false;
        }

        getline(cin, islandName);

        if (islandName.compare("0") == 0) {
            cout << "Exiting Exploration...\n";

            cout << "\nIslands visited during your exploration:\n";
            while (!visitedStack.isEmpty()) {
                cout << graph.islands[visitedStack.top()]->name << "\n";
                visitedStack.pop();
            }
            break;
        }
        static int choice;
        choice = getIslandIdFromName(graph, islandName); // Get the island ID from the name

        if (choice == -1) {
            cout << "Invalid choice. Try again.\n";
            continue;
        }
        // Check if the island is reachable
        bool foundEdge = false;
        for (const auto& edge : graph.adjacencyList[currentIsland]) {
            if (edge.to == choice) {
                foundEdge = true;
                break;
            }
        }

        if (!foundEdge) {
            cout << "You can't travel to this island directly from here. Try again.\n";
            continue;
        }

        // Travel to the chosen island
        int travelCost = 0;
        for (const auto& edge : graph.adjacencyList[currentIsland]) {
            if (edge.to == choice) {
                travelCost = edge.cost;
                break;
            }
        }

        if (resources < travelCost) {
            cout << "Not enough resources to travel! Exploration ends.\n";
            break;
        }

        resources -= travelCost;  // Deduct resources for travel
        cout << "Traveling to " << graph.islands[choice]->name << " with cost: " << travelCost << endl;

        currentIsland = choice;  // Move to the new island
        resources += graph.islands[currentIsland]->resourceGain;  // Gain resources
        graph.islands[currentIsland]->resourceGain = 0;  // Island resources are now depleted
        visitedStack.push(currentIsland);  // Mark the island as visited
        cout << "Explored " << graph.islands[currentIsland]->name << " and gained resources.\n";

        // If exploration ends, show visited islands
        if (resources <= 0) {
            cout << "You have run out of resources. Game over.\n";
        }

        cout << "\nIslands visited during your exploration:\n";
        while (!visitedStack.isEmpty()) {
            cout << graph.islands[visitedStack.top()]->name << "\n";
            visitedStack.pop();

        }
    }
}

// Linked list node for sorting resources
struct IslandNode {
    Island* island;
    IslandNode* next;
    IslandNode(Island* island) : island(island), next(nullptr) {}
};

// Function to merge two sorted linked lists
IslandNode* mergeSortedLists(IslandNode* list1, IslandNode* list2) {
    if (!list1) return list2;
    if (!list2) return list1;

    if (list1->island->resourceGain < list2->island->resourceGain) {
        list1->next = mergeSortedLists(list1->next, list2);
        return list1;
    }
    else {
        list2->next = mergeSortedLists(list1, list2->next);
        return list2;
    }
}

// Function to perform merge sort on the linked list of islands
IslandNode* mergeSort(IslandNode* head) {
    if (!head || !head->next) return head;

    IslandNode* mid = head;
    IslandNode* fast = head->next;
    while (fast && fast->next) {
        mid = mid->next;
        fast = fast->next->next;
    }

    IslandNode* right = mid->next;
    mid->next = nullptr;

    return mergeSortedLists(mergeSort(head), mergeSort(right));
}

// Function to store all islands in a linked list and sort by resources
IslandNode* storeAndSortIslands(Graph& graph) {
    IslandNode* head = nullptr;
    IslandNode* tail = nullptr;

    for (int i = 0; i < graph.islandCount; i++) {
        IslandNode* newNode = new IslandNode(graph.islands[i]);
        if (!head) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    return mergeSort(head);
}
