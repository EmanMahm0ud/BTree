#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

template <class type, int order>
class Node
{
public:
    type *keys;      //array of keys
    Node<type, order> **children;  //array of nodes
    int numOfKeys;   //number of current keys
    bool leaf;       //if the node is a leaf or not

    Node(bool leaf);
    Node();
    ~Node();
};

template <class type, int order>
Node<type, order>::Node(){

};

template <class type, int order>
Node<type, order>::Node(bool leaf){
    this->leaf = leaf;
    keys = new type[order];
    children = new Node<type, order>*[order];
    numOfKeys = 0;
};

template <class type, int order>
Node<type, order>::~Node(){
    delete[] keys;
    delete[] children;
};

#endif // NODE_H
