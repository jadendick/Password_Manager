#ifndef TREE_H
#define TREE_H

#include <vector>
#include "node.h"
using namespace std;

class Tree
{
    private:
        Node *root;
    public:
        Tree() { root = nullptr; }
        Tree(Record r) { Node *n = new Node(r); root = n; }
        ~Tree() { destruct(root); }

        void add(Node*);
        void search(string, vector<Node*>*);
        void recursiveSearch(string, Node*, vector<Node*>*);
        void del(Node*);
        void recursiveDel(Node*, Node*, Node*);
        void destruct(Node*);

        Node* getRoot() { return root; }
        void setRoot(Node *n) { root = n; }
};

#endif
