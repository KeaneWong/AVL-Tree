#ifndef AVL_H
#define AVL_H
// Modified for CSCI 220 Fall 15
// Updated for CSCI 220 Fall 19 -- overload more operators and no duplicates

#include "BinaryTree.h"
#include "RuntimeException.h"
#include <fstream>
#define COUNT 10
template <typename E>
class AVLTree {                // Modified BST class
public:                             // public types
    typedef typename E::Key K;        // a key
    typedef typename E::Value V;    // a value
    class Iterator;                    // an iterator/position
public:                            // public functions
    AVLTree(): T(), n(0)
    { T.addRoot(); T.expandExternal(T.root()); }// create the super root
    
    AVLTree(string s) : T(),n(0)//modified constructor that uses a filename
    {
        
        T.addRoot();
        T.expandExternal(T.root());
        ifstream inFile;//string stream
        inFile.open(s);
        if (!inFile) {
            cerr << "Unable to open file datafile.txt"<<endl;
            exit(1);   // call system to stop
        }
        string line;
        while(getline(inFile,line))//read in line by line
        {
            
            int i = 0;
            bool hasTheDigit = true;
            int startingDigit = 0;
            while(!isdigit(line[startingDigit]))//we parse through the string to find the first integer. Necessary because I had errors reading in lines
            {
                startingDigit++;
                //startingDigit++;
            }
            i = startingDigit;
            while(i<line.size()&&hasTheDigit)
            {
                
                i++;
                if(!isdigit(line[i]))//checking where the area code/key ends
                {
                    hasTheDigit = false;
                }
            }
            i -= startingDigit;//removing starting digit again
            int j = stoi(line.substr(0,i));
            //i is now the index of the last digit if the area code
            string address = line.substr(i+1,line.size()-i);//Doing i+1 as our starting index since it is implied that there is a comma between the area code and the address
            
            insert(j,address);
        }
        inFile.close();
        // T.PrintList(); //not working rn
        
    }
    int size() const { return n; }                 // number of entries
    void display()
    {
        T.printTree();
    }
    
    
    bool empty() const { return n == 0; };        // is the tree empty?
    Iterator find(const K& k) {
        TPos v = finder(k, root());                // search from virtual root
        if (!v.isExternal()) return Iterator(v);// found it
        else return end();                        // didn't find it
    }
    
    Iterator insert(const K& k, const V& x)        // insert (k,x)
    {
        TPos v = inserter(k, x);
        return Iterator(v); }
    
    void erase(const K& k) { //throw(NonexistentElement) {
        TPos v = finder(k, root());                // search from virtual root
        if (v.isExternal())                    // not found?
            throw NonexistentElement("Erase of nonexistent");
        eraser(v);                        // remove it
    }
    
    void erase(const Iterator& p)            // remove entry at p
    { eraser(p.v); }
    
    Iterator begin(){
        TPos v = root();                    // start at virtual root
        while (v.isInternal()) v = v.left();        // find leftmost node
        return Iterator(v.parent());
    }
    
    Iterator end()                    // iterator to end entry
    { return Iterator(T.root()); }            // return the super root
    
protected:                        // local utilities
    typedef BinaryTree<E> BinaryTree;            // linked binary tree
    typedef typename BinaryTree::Position TPos;        // position in the tree
    TPos root() const    { return T.root().left(); }        // left child of super root
    
    TPos finder(const K& k, TPos v){
        if (v.isExternal()) return v;            // key not found
        if (k < (*v).key()) return finder(k, v.left());    // search left subtree
        else if ((*v).key() < k) return finder(k, v.right());    // search right subtree
        else return v;                    // found it here
    }
public:
    void printVert()//wrapper function for our recursive printvert1
    {
        
        printVert1(T.root(), 0);
    }
    void printVert1(TPos myPos, int space)//this code parses through the list and adds spaces based on the position by recursion
    {
        // Base case
        if (myPos.isExternal())//if we hit a dummy external we're done
            return;
        // Increase distance between levels
        space += COUNT;
        
        // right child first
        printVert1(myPos.right(), space);
        
        // Print current node after space
        // count
        cout<<endl;
        for (int i = COUNT; i < space; i++)
            cout<<" ";
        cout<<myPos.operator*().key()<<"\n";
        
        // leftchild
        printVert1(myPos.left(), space);
    }
protected:
    TPos inserter(const K& k, const V& x){
        TPos v = finder(k, root());                // search from virtual root
        if (!v.isExternal())    {                // key already exists?
            (*v).setValue(x);                   // replace value
            cout<<"Identical key already exists. Replacing value"<<endl;
        }
        else
        {
            T.expandExternal(v);                // add new internal node
            (*v).setKey(k); (*v).setValue(x);    // set entry
            //        v->setKey(k); v->setValue(x);        // set entry
            n++;                                // one more entry
        }
        T.balance(v);
        return v;                                // return insert position
    }
    
    TPos eraser(TPos& v) {
        TPos w;
        if (v.left().isExternal()) w = v.left();        // remove from left
        else if (v.right().isExternal()) w = v.right();    // remove from right
        else {                        // both internal?
            w = v.right();                    // go to right subtree
            do { w = w.left(); } while (!w.isExternal());    // get leftmost node
            TPos u = w.parent();
            (*v).setKey((*u).key()); (*v).setValue((*u).value());    // copy w's parent to v
        }
        n--;                        // one less entry
        return T.removeAboveExternal(w);            // remove w and parent
    }
    
    TPos restructure(const TPos& v) {};             // restructure, not needed
    // throw(BoundaryViolation);
private:                         // member data
    BinaryTree T;                    // the binary tree
    int n;                        // number of entries
public:
    // ...insert Iterator class declaration here
    class Iterator {                                  // an iterator/position
    private:
        TPos v;                        // which entry
    public:
        Iterator(const TPos& vv) : v(vv) { }        // constructor
        const E& operator*() const { return *v; }    // get entry (read only)
        TPos operator*() { return v; }            // get entry (read/write)
        bool operator==(const Iterator& p) const        // are iterators equal?
        { return v == p.v; }
        
        bool operator!=(const Iterator& p) const
        {
            return !(*this == p);
        }
        
        Iterator& operator++(){
            TPos w = v.right();
            if (w.isInternal()) {                // have right subtree?
                do { v = w; w = w.left(); }        // move down left chain
                while (w.isInternal());
            }
            else {
                w = v.parent();                    // get parent
                while (v == w.right())            // move up right chain
                { v = w; w = w.parent(); }
                v = w;                            // and first link to left
            }
            return *this;
        }
        
        friend class AVLTree;                // give search tree access
    };
};
#endif

