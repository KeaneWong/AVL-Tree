#ifndef BINARY_TREE_H
#define BINARY_TREE_H
// Modified for CSCI 220 Fall 13
// Updated for CSCI 220 Fall 19

#include <list>
using namespace std;

//typedef int Elem;                    // base element type
template <typename Elem>
class BinaryTree {
protected:
    // insert Node declaration here...
    struct Node {                    // a node of the tree
        Elem    elt;                    // element value
        Node*   par;                    // parent
        Node*   left;                    // left child
        Node*   right;                    // right child
        Node() : elt(NULL), par(NULL), left(NULL), right(NULL) { } // constructor
    };
    
public:
    // insert Position declaration here...
    class Position {                    // position in the tree
    private:
        Node* v;                        // pointer to the node
    public:
        Position(Node* _v = NULL) : v(_v) { }        // constructor
        Elem& operator*()                    // get element
        { return v->elt; }
        Position left() const                // get left child
        { return Position(v->left); }
        Position right() const                // get right child
        { return Position(v->right); }
        Position parent() const                // get parent
        { return Position(v->par); }
        bool isRoot() const                // root of the tree?
        { return v->par == NULL; }
        bool isExternal() const                // an external node?
        { return v->left == NULL && v->right == NULL; }
        
        bool isInternal() const
        {
            return !isExternal();
        }
        bool operator==(const Position& p) const
        {
            return v == p.v;
        }
        
        bool operator!=(const Position& p) const
        {
            return !(*this == p);
        }
        
        friend class BinaryTree;            // give tree access
    };
    typedef std::list<Position> PositionList;        // list of positions
    void PrintList()
    {
        
        list <Position> myList = positions();
        if(!myList.empty())
        {
            typename PositionList::iterator myIterator = myList.begin();
        }
        else{
            // cout<<"List is EMPTY son"<<endl;
        }
    }
public:
    BinaryTree();                    // constructor
    
    int size() const;                    // number of nodes
    bool empty() const;                    // is tree empty?
    Position root() const                // get the root
    { return Position(_root); }
    void balance(Position w);
    int height(Position w);
    int height1(Position w, int x);
    void rotateRight(Position pivot);
    void rotateLeft(Position pivot);
    
    void printTree()
    {
        PositionList myList = positionsInorder();
        while(myList.size()!=0)
        {
            Position myPos = myList.front();
            cout<<myPos.operator*().key() << " " << myPos.operator*().value()<<endl;
            myList.pop_front();
        }
    }
    
    PositionList positions() const              // list of nodes
    {
        PositionList pl;
        preorder(_root, pl);                    // preorder traversal
        
        return PositionList(pl);                // return resulting list
    }
    PositionList positionsInorder()const
    {
        PositionList p1;
        inorder(_root,p1);
        return PositionList(p1);
    }
    
    void addRoot();                    // add root to empty tree
    void expandExternal(const Position& p);        // expand external node
    Position removeAboveExternal(const Position& p)    // remove p and parent
    {
        Node* w = p.v;  Node* v = w->par;            // get p's node and parent
        Node* sib = (w == v->left ?  v->right : v->left);
        if (v == _root) {                    // child of root?
            _root = sib;                    // ...make sibling root
            sib->par = NULL;
        }
        else {
            Node* gpar = v->par;                // w's grandparent
            if (v == gpar->left) gpar->left = sib;         // replace parent by sib
            else gpar->right = sib;
            sib->par = gpar;
        }
        delete w; delete v;                    // delete removed nodes
        n -= 2;                        // two fewer nodes
        return Position(sib);
    }
    
    // housekeeping functions omitted...
protected:                         // local utilities
    void preorder(Node* v, PositionList& pl) const;    // preorder utility
    void inorder(Position v, PositionList& pl) const;
private:
    Node* _root;                    // pointer to the root
    int n;                        // number of nodes
};

template <typename Elem>
BinaryTree<Elem>::BinaryTree()            // constructor
: _root(NULL), n(0) { }
template <typename Elem>


//template <typename Elem> //creating an error for some reason? Had to comment out
int BinaryTree<Elem>::size() const            // number of nodes
{ return n; }

template <typename Elem>
bool BinaryTree<Elem>::empty() const            // is tree empty?
{ return size() == 0; }

template <typename Elem>
void BinaryTree<Elem>::addRoot()            // add root to empty tree
{ _root = new Node; n = 1; }

template <typename Elem>
void BinaryTree<Elem>::expandExternal(const Position& p) {
    Node* v = p.v;                    // p's node
    v->left = new Node;                    // add a new left child
    v->left->par = v;                    // v is its parent
    v->right = new Node;                // and a new right child
    v->right->par = v;                    // v is its parent
    n += 2;                        // two more nodes
}

// preorder traversal
template <typename Elem>
void BinaryTree<Elem>::preorder(Node* v, PositionList& pl) const {
    pl.push_back(Position(v));                // add this node
    if (v->left != NULL)                    // traverse left subtree
        preorder(v->left, pl);
    if (v->right != NULL)                    // traverse right subtree
        preorder(v->right, pl);
}

template <typename Elem>
void BinaryTree<Elem>::inorder(Position v, PositionList& pl) const
{
    if(!v.left().isExternal())
    {
        inorder(v.left(), pl);
    }
    pl.push_back(v);
    if(!v.right().isExternal())
    {
        inorder(v.right(),pl);
    }
    
}

template <typename Elem>

void BinaryTree<Elem>::balance(Position w)//we use our height functions to check for balance
{
    if(!w.isExternal())//dummy external marks end
    {
        int avlDifference = height(w.left())-height(w.right());//checking if our subtrees are imbalanced
        // cout<<"The avl difference is"<<avlDifference<<endl;
        if(avlDifference > 1)//this means the left subtree is significantly greater in height than the right one
        {
            int avl1 = height(w.left().left()) - height(w.left().right());
            if (avl1<0)//if the difference is negative, the subtree is imbalanced toward the right and needs correction
            {
                rotateLeft(w.left());
            }
            rotateRight(w);
        }
        else if(avlDifference < -1)//means our right subtree has a significantly higher height than our left
        {
            int avl2 = height(w.right().left()) - height(w.right().right());
            if (avl2>0)//if difference is positive, our subtree is imbalanced towards the left
            {
                rotateRight(w.right());
            }
            rotateLeft(w);
            
        }
        if(w.parent()!=root())//make our way up to our root
        {
            balance(w.parent());
        }
    }
}
int max(int a, int b) //helpful function for our height function
{
    return (a >= b) ? a : b;
}
template <typename Elem>
int BinaryTree<Elem>::height(Position subTree)//wrapper function for height
{
    return height1(subTree,0);
}
template <typename Elem>
int BinaryTree<Elem>::height1(Position subTree, int k)//recursive function. Always passes k=0 on the first call
{
    Node *sTree = subTree.v;
    // cout<<"The subtree is " << (subTree.isExternal())<<endl;
    if(subTree.isExternal())//if both children exist, check their heights
    {
        return k;
    }
    else if(sTree->right == NULL && sTree->left!=NULL)
    {
        return height1(subTree.left(),k+1);
    }
    else if(sTree->left ==NULL && sTree->right!=NULL)
    {
        return height1(subTree.right(),k+1);
    }
    else if (sTree->left!=NULL&&sTree->right!=NULL){
        return max(height1(subTree.left(),k+1),height1(subTree.right(),k+1));
    }
    else{
        return 0;
    }
}

template <typename Elem>
void BinaryTree<Elem>::rotateRight(Position pivot)//basic rotate ability
{
    //cout<<"Rotating right"<<endl;
    Node *piv = pivot.v;//working with nodes instead of Positions so we can set values
    
    Node *newPiv = piv->left;//the node that'll take the pivot's spot in rotation
    Node *newLeft = newPiv->right;//the child of the new pivot that'll be replaced by the old pivot
    piv->left = newLeft;
    newLeft->par=piv;
    
    if(!pivot.isRoot())
    {
        newPiv->par = piv->par;
        if(piv->par->left==piv)
        {
            piv->par->left = newPiv;//if pivot is the left child of its parent we then set the new pivot as the left child of the parent
        }
        else
        {
            piv->par->right = newPiv;//same as above but if the pivot wasnt the left child of its parent, its the right
        }
    }
    else{
        newPiv->par = NULL;//if our pivot was the root then our new pivot is now the node and needs its parent set to NULL
    }
    piv->par = newPiv;//we set the previous pivot's new parent equal to the new pivot
    newPiv->right = piv;
    
}
template <typename Elem>
void BinaryTree<Elem>::rotateLeft(Position pivot)//rotates left about a designated position object
{
    //cout<<"Rotating Left"<<endl;
    Node *piv = pivot.v;//working with nodes instead of Positions so we can set values
    Node *newPiv = piv->right;//the node that'll take the pivot's spot in rotation
    
    Node *newRight = newPiv->left;//the child of the new pivot that'll be replaced by the old pivot
    piv->right = newRight;
    newRight->par = piv;
    if(!pivot.isRoot())
    {
        newPiv->par = piv->par;
        if(piv->par->left==piv)
        {
            piv->par->left = newPiv;//if pivot is the left child of its parent we then set the new pivot as the left child of the parent
        }
        else
        {
            newPiv->par->right = newPiv;//same as above but if the pivot isnt the left child, its the right
        }
    }
    else{
        newPiv->par = NULL;//if our pivot was the root then our new pivot is now the node and needs its parent set to NULL
    }
    piv->par = newPiv;//we set the previous pivot's new parent equal to the new pivot
    newPiv->left = piv;
    
}

#endif

