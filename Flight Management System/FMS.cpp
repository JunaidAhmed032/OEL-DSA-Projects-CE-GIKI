#include <iostream>
using namespace std;
const int maxCities = 10;            
const int positiveInfinity = 999999; 
struct City
{
    int cityId;
    string name;
};
struct BSTNode
{
    int time; 
    BSTNode *left;
    BSTNode *right;
};
int totalCities = 0;                        
City cities[maxCities];                     
int routes[maxCities][maxCities];           
BSTNode *flightTimes[maxCities][maxCities]; 
int findCityIndex(int cityId)
{
    for (int i = 0; i < totalCities; i++)
    {
        if (cities[i].cityId == cityId)
            return i;
    }
    return -1;
}
BSTNode *insertFlightTime(BSTNode *root, int time)
{
    if (!root)
    {
        BSTNode *newNode = new BSTNode();
        newNode->time = time;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
    if (time < root->time)
    {
        root->left = insertFlightTime(root->left, time);
    }
    else if (time > root->time)
    {
        root->right = insertFlightTime(root->right, time);
    }
    return root;
}
void printFlightTimes(BSTNode *root)
{
    if (!root)
    {
        return;
    }
    printFlightTimes(root->left);
    cout << root->time << " | ";
    printFlightTimes(root->right);
}
bool validCityPair(int fromId, int toId)
{
    int fromIndex = findCityIndex(fromId);
    int toIndex = findCityIndex(toId);

    if (fromIndex == -1 || toIndex == -1)
    {
        return false;
    }
    return true;
}
int findClosestTime(BSTNode *root, int targetTime, int closest)
{
    if (!root)
        return closest;
    if (abs(targetTime - root->time) < abs(targetTime - closest))
    {
        closest = root->time;
    }
    if (targetTime < root->time)
    {
        return findClosestTime(root->left, targetTime, closest);
    }
    else
    {
        return findClosestTime(root->right, targetTime, closest);
    }
}
void findPath(int startId, int endId)
{
    if (!validCityPair(startId, endId))
    {
        cout << "Invalid city IDs." << endl;
        return;
    }
    else
    {
        int fromIndex = findCityIndex(startId);
        int toIndex = findCityIndex(endId);
        int dist[maxCities];
        bool visited[maxCities] = {false};
        int parent[maxCities];
        for (int i = 0; i < totalCities; i++)
        {
            dist[i] = positiveInfinity;
            parent[i] = -1;
        }
        dist[fromIndex] = 0;
        for (int i = 0; i < totalCities; i++)
        {
            int u = -1; 
            for (int j = 0; j < totalCities; j++)
            {
                if (!visited[j] && (u == -1 || dist[j] < dist[u]))
                    u = j;
            }
            if (dist[u] == positiveInfinity)
            {
                break;
            }
            visited[u] = true;
            for (int v = 0; v < totalCities; v++)
            {
                if (routes[u][v] > 0 && !visited[v] && dist[u] + routes[u][v] < dist[v])
                {
                    dist[v] = dist[u] + routes[u][v];
                    parent[v] = u;
                }
            }
        }
        if (dist[toIndex] == positiveInfinity)
        {
            cout << "No route exists between the cities." << endl;
            return;
        }
        cout << "Shortest route cost: " << dist[toIndex] << "\nPath: ";
        int path[maxCities];
        int count = 0;
        int current = toIndex;
        while (current != -1)
        {
            path[count++] = current;
            current = parent[current];
        }
        for (int i = count - 1; i >= 0; i--)
        {
            cout << cities[path[i]].name;
            if (i > 0)
                cout << " ==> ";
        }
        cout << endl;
    }
}
int main()
{
    for (int i = 0; i < maxCities; i++)
        for (int j = 0; j < maxCities; j++)
        {
            routes[i][j] = -1;
            flightTimes[i][j] = nullptr;
        }
    int choice;
    do
    {
        cout << endl;
        cout << "Main Menu" << endl;
        cout << "1. Admin" << endl;
        cout << "2. User" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1)
        {
            int adminChoice;
            do
            {
                cout << endl;
                cout << "Admin Menu" << endl;
                cout << "1. Add City" << endl;
                cout << "2. Add/Update Route" << endl;
                cout << "3. Add Flight Time" << endl;
                cout << "4. Display Routes" << endl;
                cout << "5. Display Flight Times" << endl;
                cout << "6. Exit" << endl;
                cin >> adminChoice;
                if (adminChoice == 1)
                {
                    int cityId;
                    string cityName;
                    cout << "Enter City ID: ";
                    cin >> cityId;
                    cout << "Enter City Name: ";
                    cin >> cityName;
                    if (totalCities >= maxCities)
                    {
                        cout << "Cannot add more cities." << endl;
                    }
                    if (findCityIndex(cityId) != -1)
                    {
                        cout << "City with ID " << cityId << " already exists." << endl;
                    }
                    cities[totalCities].cityId = cityId;
                    cities[totalCities].name = cityName;
                    totalCities++;
                    cout << "City added successfully." << endl;
                }
                else if (adminChoice == 2)
                {
                    int fromId;
                    int toId;
                    int cost;
                    cout << "Enter Source ID: ";
                    cin >> fromId;
                    cout << "Enter Destination ID: ";
                    cin >> toId;
                    cout << "Enter Cost: ";
                    cin >> cost;
                    if (validCityPair(fromId, toId))
                    {
                        int fromIndex = findCityIndex(fromId);
                        int toIndex = findCityIndex(toId);
                        routes[fromIndex][toIndex] = cost;
                        cout << "Route updated successfully." << endl;
                    }
                    else
                    {
                        cout << "Invalid city IDs." << endl;
                    }
                }
                else if (adminChoice == 3)
                {
                    int fromId;
                    int toId;
                    int time;
                    cout << "Enter Source ID: ";
                    cin >> fromId;
                    cout << "Enter Destination ID: ";
                    cin >> toId;
                    cout << "Enter Flight Time: ";
                    cin >> time;
                    if (!validCityPair(fromId, toId))
                    {
                        cout << "Invalid city IDs." << endl;
                    }
                    else
                    {
                        int fromIndex = findCityIndex(fromId);
                        int toIndex = findCityIndex(toId);
                        flightTimes[fromIndex][toIndex] = insertFlightTime(flightTimes[fromIndex][toIndex], time);
                        cout << "Flight time added successfully." << endl;
                    }
                }
                else if (adminChoice == 4)
                {
                    cout << endl;
                    cout << "Cities:" << endl;
                    for (int i = 0; i < totalCities; i++)
                    {
                        cout << "City ID: " << cities[i].cityId << ", Name: " << cities[i].name << endl;
                    }
                    cout << endl;
                    cout << "Routes" << endl;
                    for (int i = 0; i < totalCities; i++)
                    {
                        for (int j = 0; j < totalCities; j++)
                        {
                            if (routes[i][j] > 0)
                            {
                                cout << "From " << cities[i].name << " to " << cities[j].name << " : Cost = " << routes[i][j] << endl;
                            }
                        }
                    }
                }
                else if (adminChoice == 5)
                {
                    int fromId;
                    int toId;
                    cout << "Enter Source ID: ";
                    cin >> fromId;
                    cout << "Enter Destination ID: ";
                    cin >> toId;
                    {
                        if (!validCityPair(fromId, toId))
                        {
                            cout << "Invalid city IDs." << endl;
                        }
                        else
                        {
                            int fromIndex = findCityIndex(fromId);
                            int toIndex = findCityIndex(toId);
                            cout << "Flight times from " << cities[fromIndex].name << " to " << cities[toIndex].name << ": ";
                            if (flightTimes[fromIndex][toIndex])
                            {
                                printFlightTimes(flightTimes[fromIndex][toIndex]);
                            }
                            else
                            {
                                cout << "No flight times available." << endl;
                            }
                            cout << endl;
                        }
                    }
                }
            } while (adminChoice != 6);
        }
        else if (choice == 2)
        {
            int userChoice;
            do
            {
                cout << endl;
                cout << "User Menu" << endl;
                cout << "1. Search Flight" << endl;
                cout << "2. View All Cities" << endl;
                cout << "3. Flight Time" << endl;
                cout << "4. Exit" << endl;
                cin >> userChoice;
                if (userChoice == 1)
                {
                    int startId;
                    int endId;
                    cout << "Enter source city ID: ";
                    cin >> startId;
                    cout << "Enter destination city ID: ";
                    cin >> endId;
                    findPath(startId, endId);
                }
                else if (userChoice == 2)
                {
                    for (int i = 0; i < totalCities; i++)
                    {
                        cout << "City ID: " << cities[i].cityId << ", Name: " << cities[i].name << endl;
                    }
                }
                else if (userChoice == 3)
                {
                    int startId;
                    int endId;
                    int flightTime;
                    cout << "Enter source city ID: ";
                    cin >> startId;
                    cout << "Enter destination city ID: ";
                    cin >> endId;
                    cout << "Enter desired time: ";
                    cin >> flightTime;
                    int fromIndex = findCityIndex(startId);
                    int toIndex = findCityIndex(endId);
                    if (fromIndex == -1 || toIndex == -1)
                    {
                        cout << "Invalid city IDs." << endl;
                    }
                    else
                    {
                        BSTNode *bstTree = flightTimes[fromIndex][toIndex];
                        int time = findClosestTime(bstTree, flightTime, positiveInfinity);

                        if (time != positiveInfinity)
                        {
                            cout << "Closest flight found: " << time << endl;
                        }
                        else
                        {
                            cout << "No flights." << endl;
                        }
                    }
                }
            } while (userChoice != 4);
        }
    } while (choice != 3);
    cout << "Exiting program." << endl;
    return 0;
}
