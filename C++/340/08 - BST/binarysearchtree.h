//***************************************************************************
//
//  Hung Nguyen
//  Z1924897
//  CSCI340 - 0002
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************
#ifndef BINARYSEARCHTREE_H_
#define BINARYSEARCHTREE_H_

#include "bst.h"

template <typename T>
Node<T> * nodeRm;

/**
 * Insert node with data value val to a tree
 *
 * @param val value of the node
 *
 * *********************************************************************************/
template <typename T>
void BinarySearchTree<T>::Insert(const T& val)   
{
    _Insert(this->root, val);
}

/**
 * Search for a leaf node with data value val
 *
 * @param val value of the node
 *
 * @return true if a leaf node with value val is found in a tree
 *
 * *********************************************************************************/
template <typename T>
bool BinarySearchTree<T>::Search(const T & val) const
{
    return (_Search(this->root, val));
}

/**
 * Remove a leaf node of a tree
 *
 * @param val value of the node
 *
 * @return true if successfully remove a leaf node
 *
 * *********************************************************************************/
template <typename T>
bool BinarySearchTree<T>::Remove(const T & val)
{
    if (this->getSize() == 0) return false;

    if (Search(val)) {
        _Remove(this->root, val);
        return true;
    } else return false;
}

/**
 * Insert node with data value val to a tree
 *
 * @param ptr point to the root node of the tree
 * @param val value of the node
 *
 * *********************************************************************************/
template <typename T>
void BinarySearchTree<T>::_Insert(Node<T> *& ptr, const T & val)
{
    if (ptr == nullptr) {
        ptr = new Node<T>;
        ptr -> data = val;
    } else if (val < ptr -> data) {
        _Insert(ptr -> left, val);
    } else if (val > ptr -> data){
        _Insert(ptr -> right, val);
    }
}

/**
 * Search for a leaf node with data value val
 *
 * @param node the root node of the tree
 * @param val value of the node
 *
 * @return true if a leaf node with value val is found
 *
 * *********************************************************************************/
template <typename T>
bool BinarySearchTree<T>::_Search(Node<T> * node, const T & val) const
{
    if (node == nullptr) return false;
    
    if ( (node->data == val) && (_Leaf(node)) ) return true;

    if (node->data > val) {
            return _Search(node->left, val);
    } else if (node->data < val) {
            return _Search(node->right, val);
    } else return false;
    
    return false;
}

template <typename T>
void BinarySearchTree<T>::_deleteRoot(Node<T>* node) ///< Delete a root node of a  BST
{
    if (node == NULL) return;
 
    /* first delete both subtrees */
    _deleteRoot(node->left);
    _deleteRoot(node->right);
     
    /* then delete the node */
    delete node;
}

template <typename T>
Node<T>* BinarySearchTree<T>::_deleteLeaves(Node<T>* &root, T x) ///< Delete a leaf node of a  BST
{
    if (root == NULL)
        return nullptr;
    root->left = _deleteLeaves(root->left, x);
    root->right = _deleteLeaves(root->right, x);

    if (root->data == x && root->left == NULL &&
                        root->right == NULL) {
        return nullptr;
    }
    return root;
}

/**
 * Remove a leaf node of a tree
 *
 * @param ptr the root node of the tree
 * @param val value of the node
 *
 * *********************************************************************************/
template <typename T>
void BinarySearchTree<T>::_Remove(Node<T> *& ptr, const T & val)
{
    if (this->getSize() == 1) {
        _deleteRoot(ptr);
        this->root = NULL;
    }
    else _deleteLeaves(ptr, val);
}

template <typename T>
bool BinarySearchTree<T>::_Leaf(Node<T> *node) const ///< Check if a node is  a leaf
{
    return ((node -> left == nullptr)&&(node -> right == nullptr));
}

#endif // End of BINARYSEARCHTREE_H_
