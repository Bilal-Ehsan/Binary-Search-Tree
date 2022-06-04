#include "bst.h"

#include <iostream>

struct BST::Node {
  keyType key;
  itemType item;

  Node* leftChild;
  Node* rightChild;

  Node(keyType k, itemType i) 
    : key(k), item(i), leftChild(nullptr), rightChild(nullptr) { } 
};

BST::Node* BST::leaf() { return nullptr; } 

bool BST::isLeaf(Node* n) { return n == nullptr; }

BST::itemType* BST::lookup(keyType soughtKey) {
  return lookupRec(soughtKey, root);
}

BST::itemType* BST::lookupRec(keyType soughtKey, Node* currentNode) {
  if (isLeaf(currentNode)) return nullptr;

  if (soughtKey == currentNode->key)
    return &(currentNode->item);

  return soughtKey < currentNode->key ? 
    lookupRec(soughtKey, currentNode->leftChild) :
    lookupRec(soughtKey, currentNode->rightChild);
}

void BST::insert(keyType k, itemType i) {
  insertRec(k, i, root);
}

void BST::insertRec(keyType k, itemType i, Node*& currentNode) {
  if (isLeaf(currentNode)) {
    currentNode = new Node(k, i);
  } else if (k == currentNode->key) {
    currentNode->item = i;
  } else if (k < currentNode->key) {
    insertRec(k, i, currentNode->leftChild);
  } else {
    insertRec(k, i, currentNode->rightChild);
  }
}

void BST::displayEntries() {
  displayEntriesRec(root);
}

void BST::displayEntriesRec(Node* currentNode) {
  if (isLeaf(currentNode)) return;

  // In-order traversal
  displayEntriesRec(currentNode->leftChild);
  std::cout << currentNode->key << " " << currentNode->item << std::endl;
  displayEntriesRec(currentNode->rightChild);
}

void BST::displayTree() {
  displayTreeRec("", root, false);
}

// Displays tree horizontally
void BST::displayTreeRec(const std::string& prefix, Node* currentNode, bool isLeft) {
  if (isLeaf(currentNode)) return;

  if (!isLeaf(currentNode)) {
    std::cout << prefix;
    std::cout << (isLeft ? "├──" : "└──");
    std::cout << currentNode->key << std::endl;

    // Enter next tree level - left and right branch
    displayTreeRec(prefix + (isLeft ? "│   " : "    "), currentNode->leftChild, true);
    displayTreeRec(prefix + (isLeft ? "│   " : "    "), currentNode->rightChild, true);
  }
}

BST::~BST() { deepDelete(root); }

void BST::deepDelete(Node* currentNode) {
  if (isLeaf(currentNode)) return;

  deepDelete(currentNode->leftChild);
  deepDelete(currentNode->rightChild);
  delete currentNode;
}

BST::Node* BST::minimumNode(Node* currentNode) {
  while (!isLeaf(currentNode->leftChild))
    currentNode = currentNode->leftChild;
  return currentNode;
}

void BST::remove(keyType k) { removeRec(k, root); }

void BST::removeRec(keyType k, Node*& currentNode) {
  if (isLeaf(currentNode)) return;

  else if (k < currentNode->key) {
    removeRec(k, currentNode->leftChild);
  }

  else if (k > currentNode->key) {
    removeRec(k, currentNode->rightChild);
  }

  // Found node to delete
  else {
    // Case 1: Node has no children
    if (isLeaf(currentNode->leftChild) && isLeaf(currentNode->rightChild)) {
      delete currentNode;
      currentNode = leaf();
    }
    
    // Case 2: Node has one child
    else if (isLeaf(currentNode->leftChild)) {
      Node* temp = currentNode;
      currentNode = currentNode->rightChild; // Moves root
      delete temp;
    } else if (isLeaf(currentNode->rightChild)) {
      Node* temp = currentNode;
      currentNode = currentNode->leftChild;
      delete temp;
    } 
    
    // Case 3: Node has two children
    else {
      Node* temp = minimumNode(currentNode->rightChild);
      currentNode->key = temp->key;
      currentNode->item = temp->item;
      removeRec(temp->key, currentNode->rightChild);
    }
  }
}

// Shallow copy
// BST::BST(const BST& bstToCopy) {
//   this->root = bstToCopy.root;
// }

// Deep copy construction
BST::BST(const BST& bstToCopy) {
  this->root = deepCopy(bstToCopy.root);
}

BST::Node* BST::deepCopy(Node* source) {
  if (isLeaf(source)) return nullptr;

  Node* result = new Node(source->key, source->item);
  result->leftChild = deepCopy(source->leftChild);
  result->rightChild = deepCopy(source->rightChild);
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
    this->root = deepCopy(bstToCopy.root);
  return *this;
}

BST::BST(BST&& bstToMove) {
  this->root = bstToMove.root;
  bstToMove.root = nullptr;
}

BST& BST::operator = (BST&& rhs) {
  if (this != &rhs) {    
    this->root = rhs.root;
    rhs.root = nullptr;
  }

  return *this;
}
