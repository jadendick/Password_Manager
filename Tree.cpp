#include "tree.h"

void Tree::add(Node *node)
{
    if(root == nullptr) // If tree is empty, set root
        root = node;

    else
    {
        Node *traversal = root; // For traversing tree
        while(traversal != nullptr)
        {
            if(node->getData().service <= traversal->getData().service) // If node belongs to the left, same name goes to left
            {
                if(traversal->getLeft() == nullptr) // If left pointer is empty, add node there, else move traversal
                {
                    traversal->setLeft(node);
                    break;
                }
                else
                    traversal = traversal->getLeft();
            }
            else    // If node belongs to right
            {
                if(traversal->getRight() == nullptr)    // If right pointer is empty, add node there, else move traversal
                {
                    traversal->setRight(node);
                    break;
                }
                else
                    traversal = traversal->getRight();
            }
        }
    }
}

void Tree::del(Node *node) { recursiveDel(node, root, root); }
void Tree::recursiveDel(Node *node, Node *traversal, Node *parent)
{
	if(node == traversal)	// If node being deleted has been found
	{
		if(node->getLeft() == nullptr)	// If node has only right children
		{
			if(node == root)	// If node is root, replace root
				root = node->getRight();
			else if(node == parent->getLeft())	// If node is left child
				parent->setLeft(node->getRight());
			else	// If node is right child
				parent->setRight(node->getRight());

			node->setRight(nullptr); // Disconnect node from tree
		}
		else if(node->getRight() == nullptr)	// If node has only left children
		{
			if(node == root)	// If node is root, replace root
				root = node->getLeft();
			else if(node == parent->getLeft())	// If node is left child
				parent->setLeft(node->getLeft());
			else	// If node is right child
				parent->setRight(node->getLeft());

			node->setLeft(nullptr);	// Disconnect node from tree
		}
		else	// If node has 2 children
		{
			parent = node;
			Node *maxParent = node;
			node = node->getLeft();

			while(node->getRight() != nullptr)	// Find max value of left subtree of node
			{
				maxParent = node;
				node = node->getRight();
			}

			if(maxParent->getRight() == node)	// Normal case to remove max node from subtree
				maxParent->setRight(node->getLeft());
			else	// Special case when max node is the left child of the deleted node
				maxParent->setLeft(node->getLeft());

			Record r = node->getData();
			parent->setData(r.service, r.username, r.password, r.notes);	// Copy max node data to deleted node position
			node->setLeft(nullptr);	// Disconnect node from tree
		}
	}
	else if(node->getData().service <= traversal->getData().service && traversal->getLeft() != nullptr)	// If node being deleted is to left of traversal
		recursiveDel(node, traversal->getLeft(), traversal);
	else if(traversal->getRight() != nullptr)	// If node being deleted is to right of traversal
		recursiveDel(node, traversal->getRight(), traversal);
}

void Tree::search(string term, vector<Node*> *resultsArray) { recursiveSearch(term, root, resultsArray); }
void Tree::recursiveSearch(string term, Node *n, vector<Node*> *resultsArray)
{
	if(n == nullptr)
		return;

    if(n->getLeft() != nullptr)
        recursiveSearch(term, n->getLeft(), resultsArray);

    Record r = n->getData();	// Searching "" returns every record
    if(term.empty() || r.service.find(term)!=string::npos || r.username.find(term)!=string::npos || r.password.find(term)!=string::npos || r.notes.find(term)!=string::npos)
        resultsArray->push_back(n);
        
    if(n->getRight() != nullptr)
        recursiveSearch(term, n->getRight(), resultsArray);
}

void Tree::destruct(Node *n)
{
	if(n->getLeft() != nullptr)		// Delete left branch
		destruct(n->getLeft());
	if(n->getRight() != nullptr)	// Delete right branch
		destruct(n->getRight());

	delete n;	// Delete node
}
