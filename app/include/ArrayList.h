#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#define DEFAULT_MAX_SIZE 1024

#include <stdexcept>
#include <iostream>
#include "List.h"

using std::runtime_error;
using std::cout;
using std::endl;

template <typename E>
class ArrayList : public List<E> {
private:
    E *elements;
    int max;
    int size;
    int pos;

public:
    ArrayList(int max = DEFAULT_MAX_SIZE) {
        if (max < 1)
            throw runtime_error("Invalid max size.");
        elements = new E[max];
        size = pos = 0;
        this->max = max;
    }
    ~ArrayList() {
        delete [] elements;
    }
    void insert(E element) {
        if (size == max)
            throw runtime_error("List is full.");
        for (int i = size; i > pos; i--)
            elements[i] = elements[i - 1];
        elements[pos] = element;
        size++;
    }
    void append(E element) {
        if (size == max)
            throw runtime_error("List is full.");
        elements[size] = element;
        size++;
    }
    E remove() {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (pos == size)
            throw runtime_error("No current element.");
        E result = elements[pos];
        for (int i = pos; i < size - 1; i++)
            elements[i] = elements[i + 1];
        size--;
        return result;
    }
    void clear() {
        pos = size = 0;
    }
    E getElement() {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (pos == size)
            throw runtime_error("No current element.");
        return elements[pos];
    }
    int getPos() {
        return pos;
    }
    void goToStart() {
        pos = 0;
    }
    void goToEnd() {
        pos = size;
    }
    void goToPos(int pos) {
        if (pos < 0 || pos > size)
            throw runtime_error("Index out bounds.");
        this->pos = pos;
    }
    void next() {
        if (pos < size)
            pos++;
    }
    void previous() {
        if (pos > 0)
            pos--;
    }
    bool atStart() {
        return pos == 0;
    }
    bool atEnd() {
        return pos == size;
    }
    int getSize() {
        return size;
    }
    void print() {
        cout << "[ ";
        for (int i = 0; i < size; i++)
            cout << elements[i] << " ";
        cout << "]" << endl;
    }
};

#endif // ARRAYLIST_H
