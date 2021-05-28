#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include "Node.h"

using namespace std;

/*
B Tree of order m has m-1 max number of keys and m children.
This class contains insert and search functions.
*/

template <class type, int order>
class BTree
{
    Node<type, order> *root;

    bool insert(Node<type, order> *node, type key);
    void insertNode(Node<type, order> *node, type key);
    void shift(Node<type, order> *node, int start);
    bool splitNode(Node<type, order> *node, type key, int index);
    void destroy(Node<type, order> *node);
    void printNode(Node<type, order> *node, int level);

public:
    BTree();
    ~BTree();
    void Insert(type key);
    void Print();
};

template <class type, int order>
BTree<type, order>::BTree(){
    root = NULL;
};

template <class type, int order>
BTree<type, order>::~BTree(){
    destroy(root);
};

//traverse the tree to delete nodes from leaves to the root
template <class type, int order>
void BTree<type, order>::destroy(Node<type, order> *node){
    if (node != NULL){
        int i;
        for (i = 0; i < node->numOfKeys ; i++)
        {
            if (node->leaf == false){
                destroy(node->children[i]);
            }
        }
        if (node->leaf == false){
            destroy(node->children[i]);
        }
    }
    delete node;
}

template <class type, int order>
void BTree<type, order>::Insert(type key){
    //first inserted key
    if (root == NULL){
        Node<type, order> *newNode = new Node<type, order>(true);
        root = newNode;
        root->keys[0] = key;
        root->numOfKeys++;
        return;
    }

    //tree is not empty
    insert(root, key);
};

//recursive function to find the right leaf node to insert the key
template <class type, int order>
bool BTree<type, order>::insert(Node<type, order> *node, type key){
    //if the right leaf node is not full
    if (node->leaf == true && node->numOfKeys != order - 1){
        insertNode(node, key);
        return true;
    }

    //if the right leaf node is full
    else if (node->leaf == true && node->numOfKeys == order - 1){
        if (node == root)  //if it is the root so just split it
            return splitNode(node, key, 0);
        return false;      //if it isn't the root, return to his parent and split it there
    }

    //didn't reach the leaf
    if (node->leaf == false){
        //looping to find the right child for the key
        int i;
        for (i = 0 ; i < node->numOfKeys ; i++){
            if (key < node->keys[i]){
                //go to child i and if it return false, the child should split
                if (insert(node->children[i], key))
                    return true;
                else
                    return splitNode(node, key, i);
            }
        }
        //if the right child was the last child (index = #keys)
        if (i == node->numOfKeys){
            if (insert(node->children[i], key))
                return true;
            else
                return splitNode(node, key, i);
        }
    }
    return true;
};

//insert key immediately at its right index
template <class type, int order>
void BTree<type, order>::insertNode(Node<type, order> *node, type key){
    if (node->numOfKeys != order){
        //looping in this node until finding the right index
        int i;
        for (i = 0 ; i < node->numOfKeys ; i++){
            if (node->keys[i] != NULL){
                if (key < node->keys[i]){
                    shift(node, i);
                    node->keys[i] = key;
                    node->numOfKeys++;
                    return;
                }
            }
        }
        //if the key was the greatest value in this node
        node->keys[i] = key;
        node->numOfKeys++;
        return;
    }
}

//if the key should insert at the an occupied index, the other keys
//after this index should be shifted be one to the right
template <class type, int order>
void BTree<type, order>::shift(Node<type, order> *node, int start){
    for (int i = node->numOfKeys ; i > start ; i--){
        node->keys[i] = node->keys[i-1];
    }
}

//node need to be split when it reaches the max number of keys (order - 1)
//parent is the parent of the node which need to be split
//index is the index of the child (split) node
template <class type, int order>
bool BTree<type, order>::splitNode(Node<type, order> *parent, type key, int index){
    Node<type, order> *newNode;    //new/actual parent of the split node, it will have the middle of the split node
    Node<type, order> *leftNode;   //node which need to be split, it have the left half of the split node
    Node<type, order> *rightNode = new Node<type, order>(true); //new node to have the right half of the split node

    //root is the node which need to be split when it is a leaf or it exceeded the max keys
    if (parent == root && (parent->leaf == true || parent->numOfKeys == order)){
        leftNode = parent;  //left node will be the parent because root doesn't have a parent
        newNode = new Node<type, order>(false);  //parent will be a new node
        root = newNode;                          //root will be the parent
    }
    else{
        leftNode = parent->children[index];
        newNode = parent;
    }

    //if the left node doesn't exceed the max keys, insert the key
    if (leftNode->numOfKeys != order){
        insertNode(leftNode, key);
    }
    //a non leaf node need to be split so the right node should be a non leaf as well
    else {
        rightNode->leaf = false;
    }

    //the middle key that will be inserted to parent node
    type mid = leftNode->keys[(order-1)/2];

    //filling right node with the right half keys and children of the left node
    for (int i = leftNode->numOfKeys - 1, j = ((order-1)/2)-(order%2) ; i > (order-1)/2 ; i--, j--){
        rightNode->keys[j] = leftNode->keys[i];
        rightNode->children[j+1] = leftNode->children[i+1];
        rightNode->numOfKeys++;
    }
    //filling the last child
    rightNode->children[0] = leftNode->children[(order-1)/2+1];

    //assign number of current keys of left node with half of max keys
    leftNode->numOfKeys = (order-1)/2;

    //shifting children of parent node
    for (int i = newNode->numOfKeys + 1 ; i > index + 1 ; i--){
        newNode->children[i] = newNode->children[i-1];
    }

    insertNode(newNode, mid);
    newNode->leaf = false;
    newNode->children[index] = leftNode;
    newNode->children[index + 1] = rightNode;

    //if parent exceeded the max keys, split parent
    if (newNode->numOfKeys == order){
        if (newNode == root)
            splitNode(newNode, 0, 0);
        return false;
    }
    //parent doesn't need to be split
    return true;
}

template <class type, int order>
void BTree<type, order>::Print(){
    if (root != NULL)
        printNode(root, 0);
};

template <class type, int order>
void BTree<type, order>::printNode(Node<type, order> *node, int level) {

	//space depending on the level
	for (int i = 0; i < level; i++) {
		cout << "  ";
	}

	//printing the keys of the current node
	for (int i = 0; i < node->numOfKeys; i++) {
		cout << node->keys[i];
		if (i < node->numOfKeys - 1)
            cout << ",";
	}
	cout << "\n";

	//traverse children
	if (!node->leaf) {
		level++;
		for (int i = 0; i <= node->numOfKeys; i++) {
			printNode(node->children[i], level);
		}
	}
}

#endif // BTREE_H
