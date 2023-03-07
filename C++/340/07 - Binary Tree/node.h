#ifndef H_NODE
#define H_NODE

// definition of node in bin tree

template <typename T>
class BinaryTree;  // forward declaration

template <typename T>
class Node {
  friend class BinaryTree<T>;  // BinaryTree is friend
 public:
  Node(const T& = T(), Node<T>* = nullptr, Node<T>* = nullptr);  // default constructor
 private:
  T        data;   // data component
  Node<T>* left;   // left pointer
  Node<T>* right;  // right pointer
};

// default constructor
template <typename T>
Node<T>::Node(const T& x, Node<T>* l, Node<T>* r) : data(x), left(l), right(r) {}

#endif  // End of H_NODE