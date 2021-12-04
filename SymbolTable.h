#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
class StackLinkedList;



class Node
{
    public:
    string identifier;
    string type;
    int scope;
    Node *next;

    public:
    Node ();
    Node (string name, string _type, int _scope): 
        identifier(name), type(_type), 
        scope(_scope), next(nullptr) {};

      
};

class StackLinkedList 
{
    Node *head;
    int size;

    public:
    StackLinkedList(): head(nullptr), size(0) {};

    ~StackLinkedList()
    {
        Node *cur = head;
        while (head != nullptr)
        {
            head = head->next;
            delete cur;
            cur = head;
        }
        size = 0;
    }

    Node* getHead ();
    int getSize ();

    bool insert (Node* );
    bool assign (string, string, int, string);
    
    void removeScope (int);

    void printTilEnd ();
    void revPrint (Node *);

    Node* findID_largestScope (string );

};

class SymbolTable
{
    public:
    StackLinkedList SLList;

    public:
    SymbolTable() {}

    void run(string filename);
};

#endif