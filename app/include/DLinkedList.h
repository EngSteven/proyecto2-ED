#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <stdexcept>
#include <iostream>
#include "List.h"
#include "DNode.h"

using std::runtime_error;
using std::cout;
using std::endl;

template <typename E>
class DLinkedList : public List<E> {
private:
    DNode<E> *head;
    DNode<E> *tail;
    DNode<E> *current;
    int size;

public:
    DLinkedList() {
        current = head = new DNode<E>(nullptr, nullptr);
        head->next = tail = new DNode<E>(nullptr, head);
        size = 0;
    }
    ~DLinkedList() {
        clear();
        delete head;
        delete tail;
    }
    void insert(E element) {
        current->next->previous = new DNode<E>(element, current->next, current);
        current->next = current->next->previous;
        size++;
    }
    void append(E element) {
        tail->previous->next = new DNode<E>(element, tail, tail->previous);
        tail->previous = tail->previous->next;
        size++;
    }
    E remove() {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (current->next == tail)
            throw runtime_error("No current element.");
        E result = current->next->element;
        current->next = current->next->next;
        delete current->next->previous;
        current->next->previous = current;
        size--;
        return result;
    }
    void clear() {
        while (head->next != tail) {
            head->next = head->next->next;
            delete head->next->previous;
        }
        current = tail->previous = head;
        size = 0;
    }
    E getElement() {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (current->next == tail)
            throw runtime_error("No current element.");
        return current->next->element;
    }
    void goToStart() {
        current = head;
    }
    void goToEnd() {
        current = tail->previous;
    }
    void goToPos(int pos) {
        if (pos < 0 || pos > size)
            throw runtime_error("Index out of bounds.");
        current = head;
        for (int i = 0; i < pos; i++)
            current = current->next;
    }
    int getPos() {
        int pos = 0;
        DNode<E> *temp = head;
        while (temp != current) {
            pos++;
            temp = temp->next;
        }
        return pos;
    }
    void next() {
        if (current->next != tail)
            current = current->next;
    }
    void previous() {
        if (current != head)
            current = current->previous;
    }
    bool atStart() {
        return current == head;
    }
    bool atEnd() {
        return current->next == tail;
    }
    int getSize() {
        return size;
    }
    void print() {
        cout << "[ ";
        DNode<E> *temp = head->next;
        for (int i = 0; i < size; i++) {
            cout << temp->element << " ";
            temp = temp->next;
        }
        cout << "]" << endl;
    }
    bool contains(E element) {
        goToStart();
        while (!atEnd()) {
            if (element == getElement())
                return true;
            next();
        }
        return false;
    }
};

#endif // DLINKEDLIST_H
