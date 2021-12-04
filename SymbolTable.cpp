#include "SymbolTable.h"


string getFirstSplitString (string line);
string noCommandLine (string line);


void insertElementToTable (string, StackLinkedList&, string, int);
void assignExistedElement (string, StackLinkedList&, string, int);
void lookupElementInScope (string, StackLinkedList&, string, int);
void removeAllNodeInScope (StackLinkedList&,int);

void SymbolTable::run(string filename)
{
    ifstream inputFile (filename);
    string line;
    int scope = 0;


    while ( getline (inputFile,line) )
    {
        // cout <<"------ line = " << line <<endl;
        string command = getFirstSplitString (line);
        string noCommand =  noCommandLine(line);

        if (command == "INSERT") 
            insertElementToTable (noCommand, SLList, line, scope);
        else if (command == "ASSIGN") 
            assignExistedElement (noCommand, SLList, line, scope); // assign in same scope nếu kh thì kiếm scope giảm
        else if (command == "LOOKUP") 
            lookupElementInScope (noCommand, SLList, line, scope);
        else if (command == "BEGIN") 
            {
                ++scope;
            }
        else if (command == "END") 
            {
                removeAllNodeInScope (SLList,scope);
                --scope;
            }
        else if (command == "PRINT") 
            {
                if (SLList.getSize() != 0)
                {
                    SLList.printTilEnd (); 
                    cout <<endl;
                }
            }
            // SLList.print();
        else if (command == "RPRINT") 
            {
                if (SLList.getSize() != 0)
                {
                    SLList.revPrint (SLList.getHead()); 
                    cout <<endl;
                }
            }
        else
        {
            throw InvalidInstruction (line);
        }

    }

    if (scope > 0)
    {
        throw UnclosedBlock (scope);
    }
    
}

string getFirstSplitString (string line)
{
    size_t pos = line.find(' ');
    if (pos == string::npos)
    return line;

    return line.substr(0,pos);
}

string noCommandLine (string line)
{
    size_t pos = line.find(' ');
    return line.erase(0,pos+1);
}


void insertElementToTable (string noCommand, StackLinkedList &SLList, string line,int scope)
{
    string id = getFirstSplitString (noCommand);

    string type_id = noCommand.erase(0,id.size()+1);

    Node * newnode = new Node (id,type_id, scope);
    if (!SLList.insert(newnode))  
    {
        throw Redeclared(line);
    }

    cout <<"success"<<endl;
}

void assignExistedElement (string noCommand, StackLinkedList &SLList, string line, int scope)
{
    string id = getFirstSplitString (noCommand);

    string assign_value = noCommand.erase(0,id.size()+1);

    if (!SLList.findID_largestScope(id)) // if no node with id -> nullptr
    {
        throw Undeclared (line); 
    }

    if (SLList.assign (id,assign_value,scope,line))
    {
        cout <<"success"<<endl;
    }
}

void lookupElementInScope (string noCommand, StackLinkedList &SLList, string line, int scope) //scope cua LOOKUP
{
    string id = getFirstSplitString (noCommand);
    Node *foundEle = SLList.findID_largestScope(id);
    
    if (foundEle)
    {
        cout << foundEle->scope<<endl;
    }

    else
    throw Undeclared (line);
    
}

void removeAllNodeInScope (StackLinkedList &SLList, int scope)
{
    if (scope == 0)
    {
        throw UnknownBlock ();
    }

    SLList.removeScope (scope);
}


// StackLinkedList methods
//Begins here
inline bool similarIDinScope (Node * a, Node * b)
{
    if (a->identifier == b->identifier && a->scope == b->scope)
    {
        return true;
    }
    return false;
}


bool StackLinkedList::insert (Node * anode)
{
    if (head == nullptr)
    {
        head = anode;
        size = 1;
        return true;
    }

    Node *cur = head;
    while (cur->next != nullptr)
    {
        if (similarIDinScope(cur, anode)) return false;
        cur = cur->next;
    }

    if(similarIDinScope(cur,anode)) return false;

    cur->next = anode;
    ++size;
    return true;
}



Node *StackLinkedList::getHead ()
{
    return head;
}

int StackLinkedList::getSize ()
{
    return size;
}


void StackLinkedList::printTilEnd ()
{
    if (head != nullptr)
    {
        Node* cur= head;
        while (cur->next != nullptr)
        {
            Node* temp = findID_largestScope(cur->identifier);
            if (temp == nullptr || temp == cur)
            {
                cout <<cur->identifier<<"//"<<cur->scope<<" ";
                cur = cur->next;
            }
            else
            cur = cur->next;
        }

        cout <<cur->identifier<<"//"<<cur->scope;
    }
}


void StackLinkedList::revPrint (Node * cur)
{
    if (cur == nullptr)
    {
        return;
    }

    if (cur->next == nullptr)
    {
        cout <<cur->identifier<<"//"<<cur->scope;
        return;
    }

    Node* temp = findID_largestScope(cur->identifier);

    if (temp == nullptr || temp == cur)
    {
        revPrint(cur->next);
        cout <<" " <<cur->identifier<<"//"<<cur->scope;
    }
    else
    revPrint(cur->next);
}


void StackLinkedList::removeScope ( int _scope)
{
    if (head == nullptr) return;
    Node *pre = head;
    if (pre->scope == _scope)
    {
        while (head != nullptr)
        {
            head = head->next;
            delete pre;
            pre = head;
        }
        size = 0;
    }

    else {
        pre = head;
        while (pre->next != nullptr)
        {
            if (pre->next->scope == _scope) break;
            pre = pre->next;
        }
        
        while (pre->next != nullptr)
        {
            Node *cur = pre->next;
            pre->next = cur->next;
            delete cur;
            --size;
        }
    }
}


Node * StackLinkedList::findID_largestScope (string id)
{
    Node *temp = nullptr;

    if (head == nullptr) return temp;
    Node *cur = head;
    while (cur != nullptr)
    {
        if (cur->identifier == id)
        {
            temp = cur;
        }
        cur = cur->next;
    }

    return temp;
} 


bool isNumber (string value)
{
    int size = value.size();
    for (int i = 0; i<size; i++)
    {
        char c = value[i];
        if (c > '9' || c < '0')
        {
            return false;
        }        
    }
    return true;
}

bool isString (string value)
{
    int size = value.size();
    if (value[0] !='\''|| value[size-1] != '\'' ) return false;

    for (int i = 1; i<size-1; i++)
    {
        char c = value[i];

        if (c <= 'Z' && c >='A')    continue;
        else if (c <= 'z' && c >= 'a') continue;
        else if ( c == ' ') continue;
        else if ( c <= '9' && c >= '0') continue;
        else
        {
            return false;
        }
    }
    return true;
}

bool StackLinkedList::assign (string id, string value, int _scope, string line)
{
    Node *eleID = findID_largestScope (id);

    if (eleID->type == "number")
    {
        if (isNumber(value)) return true;
        if (isString(value))
        {
            throw TypeMismatch(line);
        }
        
        Node* found = findID_largestScope(value);
        if (found == nullptr ) 
        {
            throw Undeclared(line);
        }
        if (found->type == "number") return true;   
        if (found->type == "string")
        {
            throw TypeMismatch (line);
        }    
    }

    else if (eleID->type == "string") 
    {
        if (isString (value)) return true;
        if (isNumber(value)) 
        {
            throw TypeMismatch(line);
        }
        Node* found = findID_largestScope(value);
        if (found == nullptr ) 
        {
            throw Undeclared (line);
        }
        if (found->type == "string") return true;  
        if (found->type == "number") 
        {
            throw TypeMismatch (line);
        }   
    }
    return false;
}
