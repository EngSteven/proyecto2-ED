#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <iostream>
#include <stdexcept>
#include "TrieNode.h"
#include "MaxHeap.h"
#include "KVPair.h"

using std::string;
using std::runtime_error;
using std::cout;
using std::endl;

class Trie {
private:
    TrieNode* root;

    void clearAux(TrieNode *current) {
        List<TrieNode*> *pointers = current->getChildrenPointers();
        pointers->goToStart();
        while (!pointers->atEnd()) {
            clearAux(pointers->getElement());
            pointers->next();
        }
        delete pointers;
        delete current;
    }
    void getMatchesAux(TrieNode* current, string prefix, List<string> *words) {
        if (current->isFinal)
            words->append(prefix);
        List<char> *children = current->getChildren();
        for (children->goToStart(); !children->atEnd(); children->next()) {
            char c = children->getElement();
            getMatchesAux(current->getChild(c), prefix + c, words);
        }
        delete children;
    }

    void getWordsPerNLettersAux(TrieNode* current, string prefix, List<string> *words, unsigned int nLetters){
        if (current->isFinal && prefix.size() == nLetters){
            words->append(prefix);
        }

        List<char> *children = current->getChildren();
        for (children->goToStart(); !children->atEnd(); children->next()) {
            char c = children->getElement();
            getWordsPerNLettersAux(current->getChild(c), prefix + c, words, nLetters);
        }
        delete children;
    }

public:
    Trie() {
        root = new TrieNode();
    }
    ~Trie() {
        clear();
        delete root;
    }
    void insert(string word) {
        /*if (containsWord(word))
            throw runtime_error("Duplicated word.");*/
        TrieNode *current = root;
        for (unsigned int i = 0; i < word.size(); i++) {
            current->prefixCount++;
            if (!current->contains(word[i]))
                current->add(word[i]);
            current = current->getChild(word[i]);
        }
        current->prefixCount++;
        current->isFinal = true;
    }
    bool containsWord(string word) {
        TrieNode *current = findNode(word);
        return current == nullptr? false : current->isFinal;
    }
    bool containsPrefix(string prefix) {
        TrieNode *current = findNode(prefix);
        return current != nullptr;
    }
    int prefixCount(string prefix) {
        TrieNode *current = findNode(prefix);
        return current == nullptr? 0 : current->prefixCount;
    }
    void remove(string word) {
        if (!containsWord(word))
            throw runtime_error("Word not found.");
        TrieNode *current = root;
        for (unsigned int i = 0; i < word.size(); i++) {
            current->prefixCount--;
            TrieNode *child = current->getChild(word[i]);
            if (child->prefixCount == 1)
                current->remove(word[i]);
            if (current->prefixCount == 0)
                delete current;
            current = child;
        }
        current->prefixCount--;
        if (current->prefixCount == 0)
            delete current;
        else
            current->isFinal = false;
    }
    void clear() {
        clearAux(root);
        root = new TrieNode();
    }
    List<string>* getMatches(string prefix) {
        List<string> *words = new DLinkedList<string>();
        TrieNode* current = findNode(prefix);
        if (current != nullptr)
            getMatchesAux(current, prefix, words);
        return words;
    }
    int getSize() {
        return root->prefixCount;
    }
    void print() {
        List<string> *words = getMatches("");
        words->print();
        delete words;
    }

    TrieNode* findNode(string word) {
        TrieNode *current = root;
        for (unsigned int i = 0; i < word.size(); i++) {
            if (!current->contains(word[i]))
                return nullptr;
            current = current->getChild(word[i]);
        }
        return current;
    }

    //metodos extras
    List<string>* getWordsPerNLetters(int nLetters){
        List<string> *words = new DLinkedList<string>();
        string prefix = "";
        getWordsPerNLettersAux(root, prefix, words, nLetters);
        return words;
    }

};

#endif // TRIE_H
