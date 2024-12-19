#include <iostream>
#include <string>
#include "graph.hpp"
#include "functions.hpp"

using namespace std;

void displayMenu() {
    cout << "\n===== SEA EXPLORATION MENU =====\n";
    cout << "1. View Islands and Connections\n";
    cout << "2. Add New Island\n";
    cout << "3. Explore Sea\n";
    cout << "4. Find Shortest Path (Dijkstra)\n";
    cout << "5. Sort Islands by Resources\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    Graph graph;

    int currentIsland = 0; // Start from the first island
    int resources = 40;   // Starting resources
    
    cout << "You start at your humble beginning of Island Romance Dawn and a measley 40 resources to your name...";
        
    int choice;
    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            // View Islands and Connections
            graph.printGraph();
            break;
        }
        case 2: {
            // Add a New Island
            string islandName;
            int islandResources;
            cout << "Enter the name of the new island: ";
            cin.ignore(); // To ignore leftover newline character
            getline(cin, islandName);
            cout << "Enter the resources of the new island: ";
            cin >> islandResources;
            graph.addIsland(islandName, islandResources);
            break;
        }
        case 3: {
            // Explore Sea
            cout << "\nYou are currently at: " << graph.islands[currentIsland]->name << "\n";
            exploreSea(graph, currentIsland, resources);
            cout << "Exploration finished. Current resources: " << resources << endl;
            break;
        }
        case 4: {
            // Find Shortest Path (Dijkstra)
            string startIslandName, goalIslandName;
            cout << "Enter the starting island: ";
            cin.ignore();
            getline(cin, startIslandName);
            cout << "Enter the goal island: ";
            getline(cin, goalIslandName);

            int startIslandId = getIslandIdFromName(graph, startIslandName);
            int goalIslandId = getIslandIdFromName(graph, goalIslandName);

            if (startIslandId == -1 || goalIslandId == -1) {
                cout << "Invalid islands entered.\n";
            }
            else {
                vector<int> path = dijkstra(graph, startIslandId, goalIslandId);
                if (path.empty()) {
                    cout << "No path found between these islands.\n";
                }
                else {
                    cout << "Shortest path: ";
                    for (int i : path) {
                        cout << graph.islands[i]->name << " ";
                    }
                    cout << "\n";
                }
            }
            break;
        }
        case 5: {
            // Sort Islands by Resources
            IslandNode* sortedHead = storeAndSortIslands(graph);
            cout << "Islands sorted by resources (lowest to highest):\n";
            IslandNode* current = sortedHead;
            while (current) {
                cout << current->island->name << " (Resources: " << current->island->resourceGain << ")\n";
                current = current->next;
            }
            break;
        }
        case 6: {
            // Exit Program
            cout << "Exiting the program. Safe travels!\n";
            return 0;
        }
        default:
            cout << "Invalid option. Please try again.\n";
            break;
        }
    }
    
    
}

