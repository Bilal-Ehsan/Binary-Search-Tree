#ifndef BST_H
#define BST_H

#include <string>

class BST {
  public:
    using keyType = int;
    using itemType = std::string;

    BST() = default;
    ~BST();

    BST(const BST&);
    BST& operator = (const BST&);

    BST(BST&&);
    BST& operator = (BST&&);

    itemType* lookup(keyType);
    void insert(keyType, itemType);
    void displayEntries();
    void displayTree();
    void remove(keyType);

  private:
    struct Node;
    Node* _root = leaf();

    itemType* lookupRec(keyType, Node*);
    void insertRec(keyType, itemType, Node*&);
    void displayEntriesRec(Node*);
    void displayTreeRec(const std::string&, Node*, bool);
    void removeRec(keyType, Node*&);
    Node* minimumNode(Node*);
    void deepDelete(Node*);
    Node* deepCopy(Node*);

    static Node* leaf();
    static bool isLeaf(Node*);
};

#endif
