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
        cout << "get_element: " << x << endl;
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
    bool contains( const Comparable & x ) const
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
        remove( x, root );
    }


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
            cout << "insert, creating new node: " << x << endl;
            t = make_shared<BinaryNode>( x, nullptr, nullptr , p);
            cout << "root:" << root << endl;
            cout << "t: " << t << endl;

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
            t = new BinaryNode{ std::move( x ), nullptr, nullptr, p };
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
    void remove( const Comparable & x, BinaryNode * & t )
    {
        if( t == nullptr )
            return;   // Item not found; do nothing
        if( x < t->element )
            remove( x, t->left );
        else if( t->element < x )
            remove( x, t->right );
        else if( t->left != nullptr && t->right != nullptr ) // Two children
        {
            t->element = findMin( t->right )->element;
            remove( t->element, t->right );
        }
        else
        {
            BinaryNode *oldNode = t;
            t = ( t->left != nullptr ) ? t->left : t->right;
            delete oldNode;
        }
    }

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
    shared_ptr<BinaryNode> findMin( shared_ptr<BinaryNode> t ) const
    {
        if( t == nullptr )
            return nullptr;
        if( t->left == nullptr )
            return t;
        return findMin( t->left );
    }

    /**
     * Internal method to find the largest item in a subtree t.
     * Return node containing the largest item.
     */
    shared_ptr<BinaryNode> findMax( shared_ptr<BinaryNode> t ) const
    {
        if( t != nullptr )
            while( t->right != nullptr )
                t = t->right;
        return t;
    }


    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the subtree.
     */
    bool contains( const Comparable & x, shared_ptr<BinaryNode> t ) const
    {
        if( t == nullptr )
            return false;
        else if( x < t->element )
            return contains( x, t->left );
        else if( t->element < x )
            return contains( x, t->right );
        else
            return true;    // Match
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
    void makeEmpty( shared_ptr<BinaryNode> &t )  // QUESTION: Why "BinaryNode * & t" ??
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
            out << t->element << endl;
            printTree( t->left, out, depth + 1);
            printTree( t->right, out, depth + 1 );
        }
    }

    /**
     * Internal method to clone subtree.
     */
    shared_ptr<BinaryNode> clone( shared_ptr<BinaryNode> t, shared_ptr<BinaryNode> p ) const
    {
        if( t == nullptr )
            return nullptr;
        else
            return make_shared<BinaryNode>( t->element, clone( t->left, t), clone( t->right, t), p );
    }
};

#endif
