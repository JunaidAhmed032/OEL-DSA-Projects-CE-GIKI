#pragma once
#include <iostream>
#include <string>
using namespace std;

// Declaration of a basic "Node" ie island for our graph

class Island {

public:

	string name;
	int id, resourceGain;

	Island(int id, string name, int resourceGain) : id(id), name(name), resourceGain(resourceGain) {}
};

