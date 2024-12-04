#ifndef TRIENODE_H
#define TRIENODE_H

#include "AVLDictionary.h"
#include "List.h"

class TrieNode {
public:
    bool isFinal;
    int prefixCount;
    AVLDictionary<char, TrieNode*> children;
    List<int> *indexList;


    TrieNode() : children() {
        isFinal = 0;
        prefixCount = 0;
        indexList = new DLinkedList<int>();
    }
    ~TrieNode() {
        delete indexList;
    }
    bool contains(char c) {
        return children.contains(c);
    }
    void add(char c) {
        children.insert(c, new TrieNode());
    }
    void remove(char c) {
        children.remove(c);
    }
    TrieNode* getChild(char c) {
        return children.getValue(c);
    }
    List<char>* getChildren() {
        return children.getKeys();
    }
    List<TrieNode*>* getChildrenPointers() {
        return children.getValues();
    }



};

#endif // TRIENODE_H
