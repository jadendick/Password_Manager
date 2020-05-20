#ifndef NODE_H
#define NODE_H
#include <string>
using namespace std;

struct Record
{
    string service, username, password, notes;
};

class Node
{
    private:
        Record data;
        Node *left, *right;
    public:
        Node() { left = nullptr; right = nullptr; }
        Node(Record d) { data = d; left = nullptr; right = nullptr; }

        Record getData() { return data; }
        Node* getLeft() { return left; }
        Node* getRight() { return right; }
        void setData(string s, string u, string p, string n) { data.service=s; data.username=u; data.password=p; data.notes=n; }
        void setLeft(Node *n) { left = n; }
        void setRight(Node *n) { right = n; }
};

#endif
