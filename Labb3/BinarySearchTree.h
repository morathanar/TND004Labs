#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "dsexceptions.h"
#include <algorithm>
#include <memory>

using namespace std;

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinarySearchTree
{
private:
    struct BinaryNode
    {
        Comparable element;
        shared_ptr<BinaryNode> left, right;
        weak_ptr<BinaryNode> parent;

        BinaryNode( const Comparable & theElement, shared_ptr<BinaryNode> lt, shared_ptr<BinaryNode> rt, shared_ptr<BinaryNode> p )
          : element{ theElement }, left{ lt }, right{ rt }, parent{p}{ }

        BinaryNode( Comparable && theElement, shared_ptr<BinaryNode> lt, shared_ptr<BinaryNode> rt, shared_ptr<BinaryNode> p )
          : element{ std::move( theElement ) }, left{ lt }, right{ rt }, parent{p} { }
    };

public:
    class BiIterator
    {
    public:
        BiIterator() : current{nullptr} {}

        BiIterator( shared_ptr<BinaryNode> p) : current{p} {}

        Comparable & operator*() const
        {
            return current->element;
        }

        Comparable * operator->() const
        {
            if (current) {
                return *(current->element);
            }
            return nullptr;
        }

        bool operator==(const BiIterator &it) const
        {
            return (current == it.current);
        }

        bool operator!=(const BiIterator &it) const {
            return (current != it.current);
        }

        BiIterator & operator++() //pre-increment
        {
            current = find_successor(current);
            return *this;
        }

        BiIterator & operator--() //pre-decrement
        {
            current = find_predecessor(current);
            return *this;
        }
    private:
        shared_ptr<BinaryNode> current;
    };

  public:
    BinarySearchTree( ) : root{ nullptr }
    {
    }

    /**
     * Copy constructor
     */
    BinarySearchTree( const BinarySearchTree & rhs ) : root{ nullptr }
    {
        root = clone( rhs.root, nullptr );
    }

    /**
     * Move constructor
     */
    BinarySearchTree( BinarySearchTree && rhs ) : root{ rhs.root }
    {
        rhs.root = nullptr;
    }

    /**
     * Destructor for the tree
     */
    ~BinarySearchTree( )
    {
        makeEmpty( );
    }

    /**
     * Copy assignment
     */
    BinarySearchTree & operator=( const BinarySearchTree & rhs )
    {
        BinarySearchTree copy = rhs;
        std::swap( *this, copy );
        return *this;
    }

    /**
     * Move assignment
     */
    BinarySearchTree & operator=( BinarySearchTree && rhs )
    {
        std::swap( root, rhs.root );
        return *this;
    }

    /**
     * Get Parent
     */
    Comparable get_parent( const Comparable & x )
    {
        //cout << "get_element: " << x << endl;
        auto ret = find_element(x, root);
        //cout << "element found" << endl;

        if (ret == nullptr || ret->parent.lock() == nullptr) {
            return Comparable();
        }
        return ret->parent.lock()->element;
    }


    /**
     * Find the smallest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMin( root )->element;
    }

    /**
     * Find the largest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMax( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMax( root )->element;
    }

    /**
     * Returns true if x is found in the tree.
     */
    BiIterator contains( const Comparable & x ) const
    {
        return contains( x, root );
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty( ) const
    {
        return root == nullptr;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void printTree( ostream & out = cout ) const
    {
        if( isEmpty( ) )
            out << "Empty tree" << endl;
        else
            printTree( root, out , 0);
    }

    /**
     * Make the tree logically empty.
     */
    void makeEmpty( )
    {
        makeEmpty( root );
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( const Comparable & x )
    {
        //cout << "public insert: " << x << endl;
        insert( x, root , nullptr);
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( Comparable && x )
    {
        insert( std::move( x ), root, nullptr );
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove( const Comparable & x )
    {
        remove( x, root);
    }

    /**
     * Find predecessor and suceeeeessor
     */
    void find_pred_succ(const Comparable & x, Comparable & a, Comparable & b)
    {
        //auto tmp = find_element(x, root);
        auto tmp = root;
        while (tmp != nullptr && x != tmp->element) {
            if ( x < tmp->element ){
                b = tmp->element;
                tmp = tmp->left;
            } else if ( x > tmp->element ) {
                a = tmp->element;
                tmp = tmp->right;
            }
        }

        if (!tmp) {
            // x was not found in tree.
            return;
        }
        //cout << "found element:" << tmp->element << endl;
        //cout << "parent of:" << tmp->element  << " is:" << tmp->parent.lock()->element << endl;

        auto succ = find_successor(tmp);
        if (succ != nullptr) {
            //cout << "successor:" << succ->element << endl;
            b = succ->element;
        }

        auto pred = find_predecessor(tmp);
        if (pred != nullptr) {
            //cout << "predecessor:" << pred->element << endl;
            a = pred->element;
        }
    }

    BiIterator begin() const
    {
        if (isEmpty()) {
            return end();
        }
        return BiIterator(findMin(root));
    }

    BiIterator end() const
    {
        return BiIterator();
    }


private:
    shared_ptr<BinaryNode> root;


    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert( const Comparable & x, shared_ptr<BinaryNode> &t, shared_ptr<BinaryNode> p)
    {
        //cout << "insert: " << x << endl;
        if( t == nullptr ) {

            t = make_shared<BinaryNode>( x, nullptr, nullptr , p);
#if 0
            cout << "insert, creating new node: " << x;
            if (t->parent.lock()) {
                cout << " with parent:" << t->parent.lock()->element << endl;
            } else {
                cout << " No parent" << endl;
            }
#endif
        } else if( x < t->element ) {
            insert( x, t->left, t );
        } else if( t->element < x ) {
            insert( x, t->right, t );
        } else {
            ;  // Duplicate; do nothing
        }
    }

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert( Comparable && x, shared_ptr<BinaryNode> &t, shared_ptr<BinaryNode> p )
    {
        if( t == nullptr )
            t = make_shared<BinaryNode>( std::move( x ), nullptr, nullptr, p );
        else if( x < t->element )
            insert( std::move( x ), t->left, t );
        else if( t->element < x )
            insert( std::move( x ), t->right, t);
        else
        {
             ;  // Duplicate; do nothing
        }
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void remove( const Comparable & x, shared_ptr<BinaryNode> &t)
    {

        if( t == nullptr )
            return;   // Item not found; do nothing
        if( x < t->element )
            remove( x, t->left);
        else if( t->element < x )
            remove( x, t->right);
        else if( t->left != nullptr && t->right != nullptr ) // Two children
        {
            t->element = findMin( t->right )->element;
            remove( t->element, t->right);
        }
        else // One or zero children
        {
            if (t->left != nullptr) {
                // Only left node
                t->left->parent = t->parent;
                t = t->left;
            } else if (t->right != nullptr) {
                // Only right node
                t->right->parent = t->parent;
                t = t->right;
            } else {
                t = nullptr;
            }
        }
    }

    /*
    * Search for an element x, return pointer to element if found. Return nullptr if not found.
    */
    shared_ptr<BinaryNode> find_element(const Comparable & x, shared_ptr<BinaryNode> t)
    {
        if (t == nullptr) {
            // Not found
            return nullptr;
        } else if (x < t->element) {
            t = find_element(x, t->left);
        } else if (x > t->element) {
            t = find_element(x, t->right);
        }
        // Found it
        return t;
    }

    /**
     * Internal method to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    static shared_ptr<BinaryNode> findMin(shared_ptr<BinaryNode> t )
    {
        if( t == nullptr ) {
            return nullptr;
        }
        if( t->left == nullptr ) {
            return t;
        }
        return findMin( t->left );
    }

    /**
     * Internal method to find the largest item in a subtree t.
     * Return node containing the largest item.
     */
    static shared_ptr<BinaryNode> findMax( shared_ptr<BinaryNode> t )
    {
        if( t == nullptr ) {
            return nullptr;
        }
        if( t->right == nullptr ) {
            return t;
        }

        return findMax( t->right );
    }


    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the subtree.
     */
    BiIterator contains( const Comparable & x, shared_ptr<BinaryNode> t ) const
    {
        if( t == nullptr )
            // Not found
            return BiIterator();
        else if( x < t->element )
            return contains( x, t->left );
        else if( t->element < x )
            return contains( x, t->right );
        else
            return BiIterator(t);    // Match
    }
/****** NONRECURSIVE VERSION*************************
    bool contains( const Comparable & x, BinaryNode *t ) const
    {
        while( t != nullptr )
            if( x < t->element )
                t = t->left;
            else if( t->element < x )
                t = t->right;
            else
                return true;    // Match

        return false;   // No match
    }
*****************************************************/

    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty( shared_ptr<BinaryNode> &t )
    {
        t = nullptr;
    }

    /**
     * Internal method to print a subtree rooted at t in sorted order.
     * In-order traversal is used
     */
    void printTree( shared_ptr<BinaryNode> t, ostream & out , int depth) const
    {
        if( t != nullptr )
        {
            for (int i = 0; i < depth; i++) {
                out << "  ";
            }
            out << t->element;
#if 0
            if (t->parent.lock()) {
                out << " parent: " << t->parent.lock()->element << endl;
            } else {
                out << " no parent" << endl;
            }
#else
            out << endl;
#endif

            printTree( t->left, out, depth + 1);
            printTree( t->right, out, depth + 1 );
        }
    }

    /**
     * Internal method to clone subtree.
     */
    shared_ptr<BinaryNode> clone(shared_ptr<BinaryNode> t, const shared_ptr<BinaryNode> & p ) const
    {
        if( t == nullptr ) {
            return nullptr;
        }
        else {
            //return make_shared<BinaryNode>( t->element, clone( t->left, t), clone( t->right, t), p );
            auto temp(make_shared<BinaryNode>( t->element, nullptr, nullptr, p));
            temp->left = clone( t->left, temp);
            temp->right = clone( t->right, temp);
            return temp;
        }
    }

     /**
     * successor
     */
    static shared_ptr<BinaryNode> find_successor(shared_ptr<BinaryNode> t)
    {
        //cout << "find_successor to: " << t->element << endl;
        if (t && t->right) {
            //cout << "finding min in right subtree" << endl;
            return findMin(t->right);
        } else { //successor is one of the ancestors
            //cout << "No right subtree" << endl;
            /*
            Algorithm: climb up using the parent pointer until
            one finds a node N which is left child of its parent
            The parent of N is the successor of node t
            */
            while(t->parent.lock() != nullptr){
                //cout << "t:" << t->element << " t->parent:" << t->parent.lock()->element << endl;
                if(t->parent.lock()->left == t){
                    //cout << "returning node: " << t->parent.lock()->element << endl;
                    return t->parent.lock();
                }
                t = t->parent.lock();
            }
            //cout << "returning nullptr" <<endl;
            return nullptr;
        }
    }

    /**
     * Predecessor
     */
    static shared_ptr<BinaryNode> find_predecessor(shared_ptr<BinaryNode> t)
    {
        //cout << "find_predecessor to: " << t->element << endl;
        if (t && t->left) {
            //cout << "finding max in left subtree" << endl;
            //cout << "left:" << t->left->element;
            return findMax(t->left);
        } else { //successor is one of the ancestors
            //cout << "No left subtree" << endl;
            /*
            Algorithm: climb up using the parent pointer until
            one finds a node N which is right child of its parent
            The parent of N is the successor of node t
            */
            while(t->parent.lock() != nullptr){
                //cout << "t:" << t->element << " t->parent:" << t->parent.lock()->element << endl;
                if(t->parent.lock()->right == t){
                    //cout << "returning node: " << t->parent.lock()->element << endl;
                    return t->parent.lock();
                }
                t = t->parent.lock();
            }
            //cout << "returning nullptr" <<endl;
            return nullptr;
        }
    }
};



#endif
