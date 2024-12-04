#ifndef BSTDICTIONARY_H
#define BSTDICTIONARY_H

#include <iostream>
#include <stdexcept>
#include "Dictionary.h"
#include "KVPair.h"
#include "DLinkedList.h"
#include "BSTree.h"

using std::runtime_error;
using std::cout;
using std::endl;

template <typename K, typename V>
class BSTDictionary : public Dictionary<K, V> {
private:
    BSTree<KVPair<K, V>> *pairs;

public:
    BSTDictionary() {
        pairs = new BSTree<KVPair<K, V>>();
    }
    ~BSTDictionary() {
        delete pairs;
    }
    void insert(K key, V value) {
        KVPair<K, V> p(key, value);
        pairs->insert(p);
    }
    V remove(K key) {
        KVPair<K, V> p(key);
        p = pairs->remove(p);
        return p.value;
    }
    V getValue(K key) {
        KVPair<K, V> p(key);
        p = pairs->find(p);
        return p.value;
    }
    void setValue(K key, V value) {
        KVPair<K, V> p(key, value);
        pairs->remove(p);
        pairs->insert(p);
    }
    bool contains(K key) {
        KVPair<K, V> p(key);
        return pairs->contains(p);
    }
    List<K>* getKeys() {
        List<K> *keys = new DLinkedList<K>();
        List<KVPair<K, V>> *elements = pairs->getElements();
        elements->goToStart();
        while (!elements->atEnd()) {
            KVPair<K, V> p = elements->getElement();
            keys->append(p.key);
            elements->next();
        }
        delete elements;
        return keys;
    }
    List<V>* getValues() {
        List<V> *values = new DLinkedList<V>();
        List<KVPair<K, V>> *elements = pairs->getElements();
        elements->goToStart();
        while (!elements->atEnd()) {
            KVPair<K, V> p = elements->getElement();
            values->append(p.value);
            elements->next();
        }
        delete elements;
        return values;
    }
    int getSize() {
        return pairs->getSize();
    }
    void print() {
        List<KVPair<K, V>> *elements = pairs->getElements();
        elements->print();
        delete elements;
    }
};


#endif // BSTDICTIONARY_H
