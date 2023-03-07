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
#ifndef H_BTREE
#define H_BTREE

#include "node.h"
template <typename T> class BinaryTree{

public:
    
    BinaryTree();                       ///< Default constructor for the BinaryTree class
    unsigned getSize() const;           ///< Returns size of tree
    unsigned getHeight() const;         ///< Returns height of tree
    
    virtual void Insert(const T& val)   ///< Inserts node with data 'val' to a  tree
    {
        _Insert(root, val);
    }
    
    /**
     * Inorder traversal of tree
     *
     * @param funcPtr a pointer to a function that output data of a node
     *
     * *********************************************************************************/
    void Inorder(void (* funcPtr)(const T&));

protected:
    
    Node<T> *root;  ///< Root of tree

private:
    
    /**
     * Private version of getSize()
     *
     * @param node the root node of the tree
     * @return the size of the tree with the root node
     *
     * *********************************************************************************/
    unsigned _getSize(Node<T> * node) const;
    
    /**
     * Private version of getHeight()
     *
     * @param node the root node of the tree
     * @return the height of the tree with the root node
     *
     * *********************************************************************************/
    unsigned _getHeight(Node<T> * node) const;

    /**
     * Private version of Insert()
     *
     * The function will insert a node with the data value 'val' to a binary tree.
     * If the tree is empty, then the new node will be the root, otherwise, it will be
     * inserted in the left or right sub tree according to the result of the function
     * 'rnd'.
     *
     * @param ptr point to a node of the tree
     * @param val data value of the node
     *
     * *********************************************************************************/
    void _Insert(Node<T> *& ptr, const T& val);             

    /**
     * Private version of Inorder()
     *
     * Inorder traversal the binary tree
     *
     * @param node the root node of a tree or subtree
     * @param funcPtr pointer to a function that output data of a node
     *
     * *********************************************************************************/
    void _Inorder(Node<T> * node, void (* funcPtr)(const T&));
};

/**
 * Default constructor for the BinaryTree class
 *
 * *********************************************************************************/
template <typename T>
BinaryTree<T>::BinaryTree(): root(nullptr) {}

/**
 * Private version of getSize()
 *
 * @param node the root node of the tree
 * @return the size of the tree from the root node
 *
 * *********************************************************************************/
template <typename T>
unsigned BinaryTree<T>::_getSize(Node<T> * node) const
{
    if (node == NULL)
        return 0;
    else
        return(_getSize(node->left) + 1 + _getSize(node->right));
}

/**
 * Private version of getHeight()
 *
 * @param node the root node of the tree
 * @return the height of the tree with the root node
 *
 * *********************************************************************************/
template <typename T>
unsigned BinaryTree<T>::_getHeight(Node<T> * node) const
{
    if (!node) return 0;
    unsigned heightLeft = _getHeight(node -> left);
    unsigned heightRight = _getHeight(node -> right);
    return (heightLeft > heightRight ? heightLeft : heightRight) + 1;
}

typedef enum {left_side, right_side } SIDE;
SIDE rnd() {return rand()%2 ? right_side : left_side;}

/**
 * Private version of Insert()
 *
 * The function will insert a node with the data value 'val' to a binary tree.
 * If the tree is empty, then the new node will be the root, otherwise, it will be
 * inserted in the left or right sub tree according to the result of the function
 * 'rnd'.
 *
 * @param ptr point to a node of the tree
 * @param val data value of the node
 *
 * *********************************************************************************/
template <typename T>
void BinaryTree<T>::_Insert(Node<T> *& ptr, const T& val)
{
    if (ptr == nullptr) {
        ptr = new Node<T>;
        ptr -> data = val;
    } else if (rnd() == left_side) {
        _Insert(ptr -> left, val);
    } else {
        _Insert(ptr -> right, val);
    }
}

/**
 * Private version of Inorder()
 *
 * Inorder traversal the binary tree
 *
 * @param node the root node of a tree or subtree
 * @param funcPtr pointer to a function that output data of a node
 *
 * *********************************************************************************/
template <typename T>
void BinaryTree<T>::_Inorder(Node<T> * node, void (* funcPtr)(const T&))
{
    if (node == nullptr) return;
    _Inorder(node -> left,funcPtr);
    funcPtr(node -> data);
    _Inorder(node -> right,funcPtr);
}

template <typename T>
unsigned BinaryTree<T>::getSize() const     ///< Returns size of tree
{
    return _getSize(root);
}

template <typename T>
unsigned BinaryTree<T>::getHeight() const   ///< Returns height of tree
{
    return _getHeight(root);
}

/**
 * Inorder traversal of tree
 *
 * @param funcPtr a pointer to a function that output data of a node
 *
 * *********************************************************************************/
template <typename T>
void BinaryTree<T>::Inorder(void (* funcPtr)(const T&))
{
    _Inorder(root, funcPtr);
}

#endif // End of H_BTREE
