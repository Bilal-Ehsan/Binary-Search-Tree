#include "bst.h"

#include <iostream>

struct BST::Node {
  keyType _key;
  itemType _item;

  Node* _leftChild;
  Node* _rightChild;

  Node(keyType k, itemType i) 
    : _key(k), _item(i), _leftChild(nullptr), _rightChild(nullptr) { } 
};

BST::Node* BST::leaf() { return nullptr; } 

bool BST::isLeaf(Node* n) { return n == nullptr; }

BST::itemType* BST::lookup(keyType soughtKey) {
  return lookupRec(soughtKey, _root);
}

BST::itemType* BST::lookupRec(keyType soughtKey, Node* currentNode) {
  if (isLeaf(currentNode)) return nullptr;

  if (soughtKey == currentNode->_key)
    return &(currentNode->_item);

  return soughtKey < currentNode->_key 
    ? lookupRec(soughtKey, currentNode->_leftChild)
    : lookupRec(soughtKey, currentNode->_rightChild);
}

void BST::insert(keyType k, itemType i) {
  insertRec(k, i, _root);
}

void BST::insertRec(keyType k, itemType i, Node*& currentNode) {
  if (isLeaf(currentNode)) {
    currentNode = new Node(k, i);
  } else if (k == currentNode->_key) {
    currentNode->_item = i;
  } else if (k < currentNode->_key) {
    insertRec(k, i, currentNode->_leftChild);
  } else {
    insertRec(k, i, currentNode->_rightChild);
  }
}

void BST::displayEntries() {
  displayEntriesRec(_root);
}

void BST::displayEntriesRec(Node* currentNode) {
  if (isLeaf(currentNode)) return;

  // In-order traversal
  displayEntriesRec(currentNode->_leftChild);
  std::cout << currentNode->_key << " " << currentNode->_item << std::endl;
  displayEntriesRec(currentNode->_rightChild);
}

void BST::displayTree() {
  displayTreeRec("", _root, false);
}

// Displays tree horizontally
void BST::displayTreeRec(const std::string& prefix, Node* currentNode, bool isLeft) {
  if (isLeaf(currentNode)) return;

  if (!isLeaf(currentNode)) {
    std::cout << prefix;
    std::cout << (isLeft ? "├──" : "└──");
    std::cout << currentNode->_key << std::endl;

    // Enter next tree level - left and right branch
    displayTreeRec(prefix + (isLeft ? "│   " : "    "), currentNode->_leftChild, true);
    displayTreeRec(prefix + (isLeft ? "│   " : "    "), currentNode->_rightChild, true);
  }
}

BST::~BST() { deepDelete(_root); }

void BST::deepDelete(Node* currentNode) {
  if (isLeaf(currentNode)) return;

  deepDelete(currentNode->_leftChild);
  deepDelete(currentNode->_rightChild);
  delete currentNode;
}

BST::Node* BST::minimumNode(Node* currentNode) {
  while (!isLeaf(currentNode->_leftChild))
    currentNode = currentNode->_leftChild;
  return currentNode;
}

void BST::remove(keyType k) { removeRec(k, _root); }

void BST::removeRec(keyType k, Node*& currentNode) {
  if (isLeaf(currentNode)) return;

  else if (k < currentNode->_key) {
    removeRec(k, currentNode->_leftChild);
  }

  else if (k > currentNode->_key) {
    removeRec(k, currentNode->_rightChild);
  }

  // Found node to delete
  else {
    // Case 1: Node has no children
    if (isLeaf(currentNode->_leftChild) && isLeaf(currentNode->_rightChild)) {
      delete currentNode;
      currentNode = leaf();
    }
    
    // Case 2: Node has one child
    else if (isLeaf(currentNode->_leftChild)) {
      Node* temp = currentNode;
      currentNode = currentNode->_rightChild; // Moves root
      delete temp;
    } else if (isLeaf(currentNode->_rightChild)) {
      Node* temp = currentNode;
      currentNode = currentNode->_leftChild;
      delete temp;
    } 
    
    // Case 3: Node has two children
    else {
      Node* temp = minimumNode(currentNode->_rightChild);
      currentNode->_key = temp->_key;
      currentNode->_item = temp->_item;
      removeRec(temp->_key, currentNode->_rightChild);
    }
  }
}

// Shallow copy
// BST::BST(const BST& bstToCopy) {
//   this->root = bstToCopy.root;
// }

// Deep copy construction
BST::BST(const BST& bstToCopy) {
  this->_root = deepCopy(bstToCopy._root);
}

BST::Node* BST::deepCopy(Node* source) {
  if (isLeaf(source)) return nullptr;

  Node* result = new Node(source->_key, source->_item);
  result->_leftChild = deepCopy(source->_leftChild);
  result->_rightChild = deepCopy(source->_rightChild);
  return result;
}

// Shallow copy assignment (given by the compiler by default)
// The result is a reference to the assigned object
// BST& BST::operator = (const BST& bstToCopy) {
//   if (this != &bstToCopy)
//     this->root = bstToCopy.root;
//   return *this;
// }

// Deep copy assignment
BST& BST::operator = (const BST& bstToCopy) {
  if (this != &bstToCopy)
    this->_root = deepCopy(bstToCopy._root);
  return *this;
}

BST::BST(BST&& bstToMove) {
  this->_root = bstToMove._root;
  bstToMove._root = nullptr;
}

BST& BST::operator = (BST&& rhs) {
  if (this != &rhs) {    
    this->_root = rhs._root;
    rhs._root = nullptr;
  }

  return *this;
}
