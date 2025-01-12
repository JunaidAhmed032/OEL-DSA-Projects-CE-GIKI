#pragma once

#include <iostream>
using namespace std;

class Stack {
private:
    int* stack;
    int capacity;
    int topIndex;

public:
   
    Stack(int initialCapacity = 10) : capacity(initialCapacity), topIndex(-1) {
        stack = new int[capacity];  // Dynamically allocate memory for the stack
    }

    ~Stack() {
        delete[] stack;
    }

    void push(int island) {
        if (topIndex == capacity - 1) {
            resize(capacity * 2);
        }
        stack[++topIndex] = island;  
    }

   
    void pop() {
        if (!isEmpty()) {
            topIndex--;
        }
    }

   
    bool isEmpty() const {
        return topIndex == -1;
    }

    int top() const {
        if (!isEmpty()) {
            return stack[topIndex];
        }
        return -1;  // Return -1 if the stack is empty
    }

private:
    void resize(int newCapacity) {
        int* newStack = new int[newCapacity];  // Create a new larger array
        for (int i = 0; i <= topIndex; ++i) {
            newStack[i] = stack[i];  // Copy the old stack to the new array
        }
        delete[] stack;
        stack = newStack;
        capacity = newCapacity;
    }
};

