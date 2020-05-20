#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tree.h"
using namespace std;

int changeKey();
void add(Tree *tree);
void search(Tree *tree, vector<Node*>*);
void edit(Tree *tree, vector<Node*>*);
void del(Tree *tree, vector<Node*>*);
void printSelection(vector<Node*>* results);
Record encrypt(Record, int);
Record decrypt(Record, int);
int save(string, Tree*);
void readFile(string, Tree*, int);
void printFile(ofstream&, Node*, int);

int main()
{
    const string PASSWORD_FILE_NAME = "passwords.txt";	// Name of file to read and store passwords
    int key = changeKey();	// Integer value used to encrypt and decrypt the passwords, based on string from user

    Tree *tree = new Tree();
    readFile(PASSWORD_FILE_NAME, tree, key); // Read existing records into tree object

    vector<Node*> *selection = new vector<Node*>(); // Hold nodes currently searched / selected
    tree->search("", selection);	// Make initial selection of every record
	printSelection(selection);

    bool exit = false;
    while(!exit)
    {
        cout << "Enter add, search, edit, delete, save.\t";
        string input;
        getline(cin, input);

        if(input == "add") add(tree);
        else if(input == "search") search(tree, selection);
        else if(input == "edit") edit(tree, selection);
        else if(input == "delete") del(tree, selection);
        else if(input == "save") key = save(PASSWORD_FILE_NAME, tree);	// Manual save can change the key
        else exit = true;
    }

    ofstream file(PASSWORD_FILE_NAME);    // Print all records into file
	printFile(file, tree->getRoot(), key);
	file.close();
	cout << "Saved" << endl;

    return 0;
}

void add(Tree *tree)
{
    Record r;
    cout << "Enter the service name:\t";
    getline(cin, r.service);
    cout << "Username:\t";
    getline(cin, r.username);
    cout << "Password:\t";
    getline(cin, r.password);
    cout << "Notes:\t";
    getline(cin, r.notes);

    tree->add(new Node(r));
}

void search(Tree *tree, vector<Node*>* results)
{
    delete results;	// Clear old results
    results = new vector<Node*>;

    cout << "Enter the search term:\t"; // Get search term from user
    string term;
    getline(cin, term);

    tree->search(term, results);
	printSelection(results);
}

void edit(Tree *tree, vector<Node*>* results)
{
    int index = 0;
    cout << "Enter the index of record to edit, 0 to cancel:\t";
    cin >> index;
    cin.ignore(INT_MAX, '\n');

    if(index == 0)	// Enter 0 to cancel
    	return;

    cout << "Leave field blank to not edit.\n";

    Record r = (*results)[index-1]->getData();	// Original values of node being edited
    string username, password, notes;

    cout << "Username:\t";
    getline(cin, username);
    cout << "Password:\t";
    getline(cin, password);
    cout << "Notes:\t";
    getline(cin, notes);

    if(username.empty())	// If nothing was entered, keep original value
    	username = r.username;
    if(password.empty())
    	password = r.password;
    if(notes.empty())
    	notes = r.notes;

    (*results)[index-1]->setData(r.service, username, password, notes);	// Change values in node, service is same
    printSelection(results);
}

void del(Tree *tree, vector<Node*>* results)
{
	int index = 0;
	cout << "Enter the index of record to delete, 0 to cancel:\t";
	cin >> index;
	cin.ignore(INT_MAX, '\n');

	if(index == 0)	// Enter 0 to cancel
		return;

	tree->del((*results)[index-1]);
	results->erase(results->begin() + index-1);	// Erase deleted record from results
	printSelection(results);
}

void printSelection(vector<Node*>* results)	// Print all records in the current selection
{
	if(results->empty())	// If no records in selection, return
		return;

	cout << "Index - Service - Username - Password - Notes\n";
	int i = 1;
	for(Node* n : *results)	// Print results with an index
	{
		Record r = n->getData();
		cout << i++ <<" - "<<r.service<<" - "<<r.username<<" - "<<r.password<<" - "<<r.notes<<'\n';
	}
}

Record encrypt(Record r, int key)	// Add a key value to each character in fields
{
	string service, username, password, notes;

	for(char c : r.service) { service += c - key; }
	for(char c : r.username) { username += c - key; }
	for(char c : r.password) { password += c - key; }
	for(char c : r.service) { notes += c - key; }

	Record er = {service, username, password, notes};
	return er;
}

Record decrypt(Record r, int key)	// Subtract a key value from each character in fields
{
	string service, username, password, notes;

	for(char c : r.service) { service += c + key; }
	for(char c : r.username) { username += c + key; }
	for(char c : r.password) { password += c + key; }
	for(char c : r.notes) { notes += c + key; }

	Record er = {service, username, password, notes};
	return er;
}

int save(string PASSWORD_FILE_NAME, Tree *tree)	// If manually saving, user can change key
{
	int key = changeKey();

	ofstream file(PASSWORD_FILE_NAME);    // Print all records into file
	printFile(file, tree->getRoot(), key);
	file.close();
	cout << "Saved" << endl;

	return key;
}

void readFile(string filename, Tree *tree, int key)
{
    ifstream file(filename);

    while(!file.eof())  // Read each existing entry in file, add into tree object
    {
        string line;
        getline(file, line);
        if(line.empty())    // If line is blank, end of file reached
            break;
        Record record;

        for(int i = 0; i < 4; i++)  // Divide a record from file into pieces
        {
            int delim = line.find('|',0);
            string data = line.substr(0, delim);
            line.erase(0, delim+1);

            switch(i)
            {
            case 0: record.service = data;  break;
            case 1: record.username = data;  break;
            case 2: record.password = data;  break;
            case 3: record.notes = data;  break;
            }
        }
        tree->add(new Node(decrypt(record, key)));    // Add record into tree after decrypted
    }
    file.close();
}

void printFile(ofstream &file, Node *node, int key) // In order traversal of tree, printing each record to file
{
    if(node->getLeft() != nullptr)
        printFile(file, node->getLeft(), key);

    Record r = encrypt(node->getData(), key); // Add encrypted data of record into file
    file << r.service<<'|'<<r.username<<'|'<<r.password<<'|'<<r.notes<<"|\n";

    if(node->getRight() != nullptr)
        printFile(file, node->getRight(), key);
}

int changeKey()
{
    string keyString;
	cout << "Enter encryption key:\t";
	getline(cin, keyString);

	int key = 0;	// Get integer value from key string
	for(char c : keyString)
		key += c;

	return key % 10 + 1;
}
