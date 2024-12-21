#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;

struct DataPacket {
    string data;
    int errorFlag;
    int destination;
    int source;
    int packetID;
    DataPacket* next;
};

struct ErrorStack {
    DataPacket* packet;
    ErrorStack* next;
};

struct EndPoint {
    int DeviceID;
    string name;
};

struct Neighbour {
    int deviceID;
    int latency;
    Neighbour* next;
    Neighbour() {
        deviceID = -1;
        latency = 0;
        next = nullptr;
    }

};

int counter = 0;

struct Graph {
    EndPoint devices[10];
    Neighbour* list[10];
    int counter;
    Graph() : counter(0) {
        for (int i = 0; i < 10; i++) {
            list[i] = nullptr;
        }
    }
};

void InitializeNetwork(Graph& network) {
    for (int i = 0; i < 10; i++) {
        network.list[i] = nullptr;
    }
}

void AddDevice(Graph& network) {
    if (network.counter >= 15) {
        cout << "Maximum number of devices reached.\n";
        return;
    }
    cout << "Enter device ID (Between 1 and 15): ";
    int deviceID;
    cin >> deviceID;
    cin.ignore();
    if (deviceID < 0 || deviceID > 250) {
        cout << "Invalid device ID. Please enter a value between 0 and 250.\n";
        return;
    }
    for (int i = 0; i < network.counter; ++i) {
        if (network.devices[i].DeviceID == deviceID) {
            cout << "Device ID already exists. Please choose a unique ID.\n";
            return;
        }
    }
    cout << "Enter device name: ";
    string deviceName;
    getline(cin, deviceName);
    network.devices[network.counter].DeviceID = deviceID;
    network.devices[network.counter].name = deviceName;
    network.list[network.counter] = nullptr;
    cout << "Device added: " << deviceName << " with ID " << deviceID << endl;
    network.counter++;
}

void AddConnection(Graph& network) {
    if (network.counter < 2) {
        cout << "At least two devices are required to add a connection.\n";
        return;
    }
    for (int i = 0; i < network.counter; ++i) {
        for (int j = i + 1; j < network.counter; ++j) {
            int sourceIndex = i;
            int destIndex = j;
            int latency = rand() % 100 + 1;
            Neighbour* newNeighbour = new Neighbour;
            newNeighbour->deviceID = network.devices[destIndex].DeviceID;
            newNeighbour->latency = latency;
            newNeighbour->next = network.list[sourceIndex];
            network.list[sourceIndex] = newNeighbour;
            newNeighbour = new Neighbour;
            newNeighbour->deviceID = network.devices[sourceIndex].DeviceID;
            newNeighbour->latency = latency;
            newNeighbour->next = network.list[destIndex];
            network.list[destIndex] = newNeighbour;
            cout << "Connection established between Device "
                << network.devices[sourceIndex].DeviceID
                << " and Device "
                << network.devices[destIndex].DeviceID
                << " with latency " << latency << " ms.\n";
        }
    }
}

void ShowLoadingBar(int length, int delay_ms) {
    for (int i = 0; i < length; ++i) {
        cout << "|";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(delay_ms));
    }
    cout << endl;
}

void Push(ErrorStack*& top, DataPacket* packet) {
    ErrorStack* newNode = new ErrorStack;
    newNode->packet = packet;
    newNode->next = top;
    top = newNode;
    cout << "Corrupt packet added to the stack\n";
}

void CheckErrorFlag(DataPacket*&, ErrorStack*&);
void GenerateDataPacket(DataPacket*& header, int& counter) {
    DataPacket* newPacket = new DataPacket;
    newPacket->data = "Test message";
    newPacket->errorFlag = rand() % 2;
    newPacket->source = rand() % 250;
    newPacket->destination = rand() % 16;
    newPacket->packetID = ++counter;
    newPacket->next = nullptr;
    if (!header) {
        header = newPacket;
    }
    else {
        DataPacket* temp = header;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newPacket;
    }
    cout << "Packet appended to data frame\n";
}
void ProcessPackets(Graph& graph, DataPacket*& header, ErrorStack*& lost_packets) {
    DataPacket* temp = header;
    DataPacket* prev = nullptr;
    while (temp != nullptr) {
        bool delivered = false;
        bool destinationFound = false;
        for (int i = 0; i < graph.counter; ++i) {
            if (graph.devices[i].DeviceID == temp->destination && temp->errorFlag != 1) {
                destinationFound = true;
                Neighbour* adjTemp = nullptr;
                cout << "Packet " << temp->packetID << " delivered to " << graph.devices[i].name << " with Device ID : " << graph.devices[i].DeviceID << endl;
                cout << "-----------------------\n";
                delivered = true;

                break;
            }
        }
        if (!destinationFound) {
            cout << "Packet ID : " << temp->packetID << endl;
            cout << "Payload : " << temp->data << endl;
            cout << "Destination : " << temp->destination << endl;
            cout << "Status : Not delivered\n";
            cout << "Reason : Destination not on the network\n";
            cout << "---------------------------------\n";
            DataPacket* to_move = new DataPacket(*temp);
            Push(lost_packets, to_move);
        }

        DataPacket* nextPacket = temp->next;
        if (delivered || !destinationFound) {
            if (prev == nullptr) {
                header = nextPacket;
            }
            else {
                prev->next = nextPacket;
            }
            delete temp;
        }
        else {
            prev = temp;
        }
        temp = nextPacket;
    }
}

void CheckErrorFlags(DataPacket*& header, ErrorStack*& top) {
    DataPacket* temp = header;
    DataPacket* prev = nullptr;
    int error_count = 0;
    if (header == nullptr) {
        cout << "No packets to check\n";
        return;
    }
    while (temp != nullptr) {
        if (temp->errorFlag == 1) {
            error_count++;
            cout << "Corrupt Packet Found!\n";
            cout << "PacketID: " << temp->packetID << endl;
            cout << "Source: " << temp->source << endl;
            cout << "Destination: " << temp->destination << endl;
            cout << "Removing packet from the frame\n";
            DataPacket* corrupt = new DataPacket;
            *corrupt = *temp;
            Push(top, corrupt);
            if (prev == nullptr) {
                header = temp->next;
            }
            else {
                prev->next = temp->next;
            }
            DataPacket* to_delete = temp;
            temp = temp->next;
            delete to_delete;
        }
        else {
            prev = temp;
            temp = temp->next;
        }
    }
    if (error_count == 0) {
        cout << "No corrupt packets found\n";
    }
    else {
        cout << error_count << " corrupt packet(s) found and removed. Kindly request them again.\n";
    }
}

void DisplayErrorStack(ErrorStack* top) {
    cout << "----Contents of Error Stack----\n";
    ErrorStack* temp = top;
    if (top == nullptr) {
        cout << "Error stack empty\n";
    }
    else {
        while (temp != nullptr) {
            cout << "PacketID : " << temp->packet->packetID << endl;
            cout << "Payload : " << temp->packet->data << endl;
            cout << "Source : " << temp->packet->source << endl;
            cout << "Destination : " << temp->packet->destination << endl;
            cout << "--------------------------\n";
            temp = temp->next;
        }
    }
}

void ClearStack(ErrorStack*& top) {
    ErrorStack* temp = top;
    if (top == nullptr) {
        cout << "Error stack empty\n";
    }
    else {
        while (temp != nullptr) {
            ErrorStack* temp_ptr = temp;
            ErrorStack* to_del = temp;
            temp = temp->next;
            delete to_del;
        }
        cout << "Stack emptied\n";
    }
}

void ViewDataFrame(DataPacket* header) {
    DataPacket* temp = header;
    if (header == nullptr) {
        cout << "Data Frame empty\n";
    }
    else {
        while (temp != nullptr) {
            cout << "PacketID : " << temp->packetID << endl;
            cout << "Payload : " << temp->data << endl;
            cout << "Source : " << temp->source << endl;
            cout << "Destination : " << temp->destination << endl;
            cout << "--------------------------\n";
            temp = temp->next;
        }
    }
}

void DisplayGraph(Graph& network) {
    for (int i = 0; i < network.counter; ++i) {
        cout << "Device " << network.devices[i].DeviceID << " (" << network.devices[i].name << ") is connected to:\n";
        Neighbour* current = network.list[i];
        if (current == nullptr) {
            cout << "No connections.\n";
        }
        else {
            while (current != nullptr) {
                cout << "Device " << current->deviceID << " with latency " << current->latency << " ms\n";
                current = current->next;
            }
        }
        cout << "-------------------------------\n";
    }
}

int main() {
    cout << "Initializing Network : ";
    ShowLoadingBar(20, 20);
    DataPacket* header = nullptr;
    ErrorStack* corruptPackets = nullptr;
    ErrorStack* lost_packets = nullptr;
    Graph network;
    int choice;
    do {
        cout << "-----Network And Firewall Manager-----\n";
        cout << "1.Add Device To The Network\n2.Connect devices\n3.View Network\n4.Generate Data Packets\n5.Check for Errors in the data frame\n6.View the Data Frame\n7.Display Corrupt Packets\n8.Delete Corrupt Packets\n9.Route Packets to their destinations\n10.Display for Lost packets\n11.Delete Lost Packets\n0.Exit\n";
        cin >> choice;
        switch (choice) {
        case 1: {
            AddDevice(network);
            break;
        }
        case 2: {
            AddConnection(network);
            break;
        }
        case 3: {
            DisplayGraph(network);
            break;
        }
        case 4: {
            GenerateDataPacket(header, counter);
            break;
        }
        case 5: {
            CheckErrorFlags(header, corruptPackets);
            break;
        }
        case 6: {
            ViewDataFrame(header);
            break;
        }
        case 7: {
            DisplayErrorStack(corruptPackets);
            break;
        }
        case 8: {
            ClearStack(corruptPackets);
            break;
        }
        case 9: {
            ProcessPackets(network, header, lost_packets);
            break;
        }
        case 10: {
            cout << "Lost packets : " << endl;
            DisplayErrorStack(lost_packets);
            break;
        }
        case 11: {
            ClearStack(lost_packets);
            break;
        }
        case 0:
            cout << "Exiting program\n";
            break;
        default:
            cout << "Invalid choice! Try again\n";
        }
    } while (choice != 0);
}
