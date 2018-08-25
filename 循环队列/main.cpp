#include <iostream>
using namespace std;

class CircleQueue {
private:
    int *da;
    int size;
    int read;
    int write;
public:
    CircleQueue():da(nullptr),size(0),read(0),write(0) {}
    CircleQueue(int sz):da(new int[sz]),size(sz),read(0),write(0) {}
    ~CircleQueue() { delete[] da; }
    bool isEmpty() { return read==write; }
    bool isFull() { return (write+1)%size==read; }
    int length() { return (write-read+size)%size; }
    void push(int v) {
        if(!isFull()) {
            da[write] = v;
            write = (write++)%size;
        }
    }
    void pop() {
        if(!isEmpty()) {
            read = (read++)%size;
        }
    }
    int top() {
        if(!isEmpty()) {
            return da[read];
        }
    }
};
