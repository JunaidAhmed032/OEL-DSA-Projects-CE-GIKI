#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "island.hpp"

using namespace std;

const int maxIslands = 15;

class Graph {

public:
    // Our struct for an edge in the map (i.e., a traversable path)
    struct Edge {
        int to;
        int cost;
    };

    Island* islands[maxIslands];                // Array of pointers to islands
    vector<Edge> adjacencyList[maxIslands];     // Array of vectors to hold edges for each island
    int islandCount;


    Graph() : islandCount(5) {
        string names[5] = { "Romance Dawn", "LRLR", "Ennies Lobby", "Water 7", "Treasure Island" };
        int resources[5] = { 50, 30, 70, 40, 100 };

        for (int i = 0; i < islandCount; i++) {
            islands[i] = new Island(i, names[i], resources[i]);
        }

        addEdge(0, 1, 10); // Romance Dawn <-> LRLR (cost: 10)
        addEdge(1, 2, 20); // LRLR <-> Ennies Lobby (cost: 20)
        addEdge(2, 3, 15); // Ennies Lobby <-> Water 7 (cost: 15)
        addEdge(3, 4, 100); // Water 7 <-> Treasure Island (cost: 100)
        addEdge(0, 4, 200); // Romance Dawn <-> Treasure Island (cost: 200)
    }

    void addIsland(const string& name, int resources) {
        if (islandCount >= maxIslands) {
            cout << "Cannot add more islands. Maximum limit reached!\n";
            return;
        }

        islands[islandCount] = new Island(islandCount, name, resources);

        cout << "Added new island: " << name << " with resources: " << resources << "\n";

        srand(time(0));
        int connections = 1 + rand() % 2; // Randomly create 1 to 2 connections

        for (int i = 0; i < connections; i++) {
            int randomIsland = rand() % islandCount;
            
            while(randomIsland == islandCount)
                int randomIsland = rand() % islandCount;

            int cost = 10 + rand() % 91; // Random travel cost between 10 and 100

            // Add edges in both directions
            addEdge(islandCount, randomIsland, cost);
            cout << "Connected " << name << " to " << islands[randomIsland]->name << " with cost: " << cost << "\n";
        }

        islandCount++; 
    }

    void addEdge(int island1, int island2, int cost) {
        // Add edge from island1 to island2
        adjacencyList[island1].push_back(Edge{ island2, cost });

        // Add edge from island2 to island1 (reverse direction)
        adjacencyList[island2].push_back(Edge{ island1, cost });
    }

    void printGraph() {
        cout << "Graph Representation:\n";
        for (int i = 0; i < islandCount; ++i) {
            cout << islands[i]->name << " -> ";
            for (const auto& edge : adjacencyList[i]) {
                cout << islands[edge.to]->name << " (Cost: " << edge.cost << ") ";
            }
            cout << endl;
        }
    }
};
