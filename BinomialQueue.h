#ifndef BINOMIAL_QUEUE_H
#define BINOMIAL_QUEUE_H

#include <iostream>
#include <vector>
#include "dsexceptions.h"
#include "QuadraticProbing.h"
#include <cstdlib>

using namespace std;

// Binomial queue class
//
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// deleteMin( )           --> Return and remove smallest item
// Comparable findMin( )  --> Return smallest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void merge( rhs )      --> Absorb rhs into this heap
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinomialQueue
{
  public:
    BinomialQueue( ) : theTrees( DEFAULT_TREES )
    {
        for( auto & root : theTrees )
            root = nullptr;
        currentSize = 0;
    }

    BinomialQueue( const Comparable & item ) : theTrees( 1 ), currentSize{ 1 }
      { theTrees[ 0 ] = new BinomialNode{ item, nullptr, nullptr, nullptr }; }

    BinomialQueue( const BinomialQueue & rhs )
      : theTrees( rhs.theTrees.size( ) ),currentSize{ rhs.currentSize }
    { 
        for( int i = 0; i < rhs.theTrees.size( ); ++i )
            theTrees[ i ] = clone( rhs.theTrees[ i ] );
    }

    BinomialQueue( BinomialQueue && rhs )
      : theTrees{ std::move( rhs.theTrees ) }, currentSize{ rhs.currentSize }
    { 
    }

    ~BinomialQueue( )
      { makeEmpty( ); }

    
    /**
     * Deep copy.
     */
    BinomialQueue & operator=( const BinomialQueue & rhs )
    {
        BinomialQueue copy = rhs;
        std::swap( *this, copy );
        return *this;
    }
        
    /**
     * Move.
     */
    BinomialQueue & operator=( BinomialQueue && rhs )
    {
        std::swap( currentSize, rhs.currentSize );
        std::swap( theTrees, rhs.theTrees );
        
        return *this;
    }
    
    /**
     * Return true if empty; false otherwise.
     */
    bool isEmpty( ) const
      { return currentSize == 0; }

    /**
     * Returns minimum item.
     * Throws UnderflowException if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };

        return theTrees[ findMinIndex( ) ]->element;
    }
    
    /**
     * Insert item x into the priority queue; allows duplicates.
     */
    void insert( const Comparable & x )
      { 
          BinomialQueue oneItem{ x }; 
          hash.Insert(x, oneItem.theTrees[0]);
          merge( oneItem );

      }

    /**
     * Insert item x into the priority queue; allows duplicates.
     */
    void insert( Comparable && x )
      { 
          BinomialQueue oneItem{ std::move( x ) }; 
          hash.Insert(std::move(x), oneItem.theTrees[0]);
          merge( oneItem );
      }

    void fast_insert( const Comparable & x )
      { 
        privateinsert(x);
      }
    /**
     * Insert item x into the priority queue using fast merge one element at a time
     */
    void fast_insert( Comparable && x )
      { 
          privateinsert(x);
      }
      
    /**
     * Remove the smallest item from the priority queue.
     * Throws UnderflowException if empty.
     */
    void deleteMin( )
    {
        Comparable x;
        deleteMin( x );
    }

    /**
     * Remove the minimum item and place it in minItem.
     * Throws UnderflowException if empty.
     */
    void deleteMin( Comparable & minItem )
    {
        if( isEmpty( ) )
            throw UnderflowException{ };

        int minIndex = findMinIndex( );
        minItem = theTrees[ minIndex ]->element;

        BinomialNode *oldRoot = theTrees[ minIndex ];
        BinomialNode *deletedTree = oldRoot->leftChild;
        delete oldRoot;

        // Construct H''
        BinomialQueue deletedQueue;
        deletedQueue.theTrees.resize( minIndex );
        deletedQueue.currentSize = ( 1 << minIndex ) - 1;
        for( int j = minIndex - 1; j >= 0; --j )
        {
            deletedQueue.theTrees[ j ] = deletedTree;
            deletedTree = deletedTree->nextSibling;
            deletedQueue.theTrees[ j ]->nextSibling = nullptr;
        }

        // Construct H'
        theTrees[ minIndex ] = nullptr;
        currentSize -= deletedQueue.currentSize + 1;

        merge( deletedQueue );
    }

    /**
     * Make the priority queue logically empty.
     */
    void makeEmpty( )
    {
        currentSize = 0;
        for( auto & root : theTrees )
            makeEmpty( root );
            hash.MakeEmpty();
    }
    /**
     * Removes a specific Item in the queue making sure it pushes it to the top of queue
       and the removes similar to the deletemin function (deleting index and everything)
     */

    bool Remove(const Comparable & x)
    {
        if( !hash.Contains(x))
            return false;
        
        BinomialNode *ptr = hash.Find(x);
        int index_toDelete = FindIndex_toDelete(ptr);
        BinomialNode *Tree_Dlt = PtrToTop(ptr);
        //now this specific item is at the top
        BinomialNode *old_root = Tree_Dlt;
        Tree_Dlt = Tree_Dlt->leftChild;
        old_root = nullptr;
        delete old_root;
        hash.Remove(x);
        // now do the same functionability as Delete Min to get the same results and keep
        // all rules applicable 
        BinomialQueue deletedQueue;
        deletedQueue.theTrees.resize(index_toDelete);
        deletedQueue.currentSize = ( 1 << index_toDelete) - 1;
        
        for( int j = index_toDelete - 1; j >= 0; --j )
        {
            deletedQueue.theTrees[ j ] = Tree_Dlt;
            Tree_Dlt = Tree_Dlt->nextSibling;
            deletedQueue.theTrees[ j ]->nextSibling = nullptr;
        }
        
        // Construct H'
        theTrees[index_toDelete] = nullptr;
        currentSize -= deletedQueue.currentSize + 1;
        
        merge( deletedQueue );
        
        return true;
    }
    

    /**
     * Merge rhs into the priority queue.
     * rhs becomes empty. rhs must be different from this.
     * Exercise 6.35 needed to make this operation more efficient.
     */
    void merge( BinomialQueue & rhs )
    {
        if( this == &rhs )    // Avoid aliasing problems
            return;

        currentSize += rhs.currentSize;

        if( currentSize > capacity( ) )
        {
            int oldNumTrees = theTrees.size( );
            int newNumTrees = max( theTrees.size( ), rhs.theTrees.size( ) ) + 1;
            theTrees.resize( newNumTrees );
            for( int i = oldNumTrees; i < newNumTrees; ++i )
                theTrees[ i ] = nullptr;
        }

        BinomialNode *carry = nullptr;
        for( int i = 0, j = 1; j <= currentSize; ++i, j *= 2 )
        {
            BinomialNode *t1 = theTrees[ i ];
            BinomialNode *t2 = i < rhs.theTrees.size( ) ? rhs.theTrees[ i ] : nullptr;

            int whichCase = t1 == nullptr ? 0 : 1;
            whichCase += t2 == nullptr ? 0 : 2;
            whichCase += carry == nullptr ? 0 : 4;

            switch( whichCase )
            {
              case 0: /* No trees */
              case 1: /* Only this */
                break;
              case 2: /* Only rhs */
                theTrees[ i ] = t2;
                rhs.theTrees[ i ] = nullptr;
                break;
              case 4: /* Only carry */
                theTrees[ i ] = carry;
                carry = nullptr;
                break;
              case 3: /* this and rhs */
                carry = combineTrees( t1, t2 );
                theTrees[ i ] = rhs.theTrees[ i ] = nullptr;
                break;
              case 5: /* this and carry */
                carry = combineTrees( t1, carry );
                theTrees[ i ] = nullptr;
                break;
              case 6: /* rhs and carry */
                carry = combineTrees( t2, carry );
                rhs.theTrees[ i ] = nullptr;
                break;
              case 7: /* All three */
                theTrees[ i ] = carry;
                carry = combineTrees( t1, t2 );
                rhs.theTrees[ i ] = nullptr;
                break;
            }
        }
        for( auto & root : theTrees )
        {
            if(root != nullptr)
                root->parent = nullptr; // added parent ptr need this condition satisfied so set it to null following same for root
        }

        for( auto & root : rhs.theTrees )
            root = nullptr;
        rhs.currentSize = 0;
    }    


       
    bool Find(const Comparable & element)
    {
        BinomialNode* test = find(element);
        if(test != nullptr)
         {return true;}
         else 
          {return false;}
    }
    int Number_Elements()
    {return theTrees.size();}

  private:
    struct BinomialNode
    {
        Comparable    element;
        BinomialNode *leftChild;
        BinomialNode *nextSibling;
        BinomialNode *parent; // needed for the hole (gap) to remove the element by putting on the top of the queue

        BinomialNode( const Comparable & e, BinomialNode *prnt, BinomialNode *lt, BinomialNode *rt )
          : element{ e }, parent{ prnt }, leftChild{ lt }, nextSibling{ rt } { }
        
        BinomialNode( Comparable && e, BinomialNode *prnt,  BinomialNode *lt, BinomialNode *rt )
          : element{ std::move( e ) }, parent{ prnt }, leftChild{ lt }, nextSibling{ rt } { }
    };

    const static int DEFAULT_TREES = 1;
    HashTable<string, BinomialNode*> hash;
    vector<BinomialNode *> theTrees;  // An array of tree roots
    int currentSize;                  // Number of items in the priority queue
    
    /**
     * Find index of tree containing the smallest item in the priority queue.
     * The priority queue must not be empty.
     * Return the index of tree containing the smallest item.
     */
    int findMinIndex( ) const
    {
        int i;
        int minIndex;

        for( i = 0; theTrees[ i ] == nullptr; ++i )
            ;

        for( minIndex = i; i < theTrees.size( ); ++i )
            if( theTrees[ i ] != nullptr &&
                theTrees[ i ]->element < theTrees[ minIndex ]->element )
                minIndex = i;

        return minIndex;
    }
    
    /**
     * Find index of tree that is at the top.
     * The priority queue must not be empty.
     * Return the index of tree containing this item.
     * returns the index necessary to delete same idea as find min 
     */

    int FindIndex_toDelete(BinomialNode * &tree)
    {
        int index_todelete;
        BinomialNode *top_ptr = tree;
        while( top_ptr->parent != nullptr ){
            top_ptr = top_ptr->parent;
        }
        for( int i = 0; i < theTrees.size(); ++i )
        {
            if( theTrees[i] != nullptr)
            {
               if( theTrees[i ] == top_ptr)
                  {index_todelete = i;}
            }
        }
        return index_todelete;
    }

    /**
     * Return the capacity.
     */
    int capacity( ) const
      { return ( 1 << theTrees.size( ) ) - 1; }

    /**
     * Return the result of merging equal-sized t1 and t2.
     */
    BinomialNode * combineTrees( BinomialNode *t1, BinomialNode *t2 )
    {
        if( t2->element < t1->element )
            return combineTrees( t2, t1 );
        t2->nextSibling = t1->leftChild;
        t1->leftChild = t2;
        t2->parent = t1;
        return t1;
    }

    /**
     * Make a binomial tree logically empty, and free memory.
     */
    void makeEmpty( BinomialNode * & t )
    {
        if( t != nullptr )
        {
            makeEmpty( t->leftChild );
            makeEmpty( t->nextSibling );
            delete t;
            t = nullptr;
        }
    }
    // fast merge an element and its node ptr to the queue 
    void privateinsert( const Comparable & x )
      { 
          BinomialNode *node_ptr = new BinomialNode {x, nullptr, nullptr, nullptr};
          hash.Insert(x, node_ptr);
          Fast_merge( node_ptr );
      }
    /**
     * Insert item x into the priority queue using fast merge one element at a time
     */
    void privateinsert( Comparable && x )
      { 
          BinomialNode *node_ptr = new BinomialNode {x, nullptr, nullptr, nullptr};
          hash.Insert(std::move(x), node_ptr);
          Fast_merge( node_ptr );
      }
    
    void Fast_merge( BinomialNode *  node )
    {
        BinomialNode * temp = node;
        theTrees.push_back(nullptr);
        for( int i = 0, j = 1; j <= currentSize; ++i, j *= 2 )
        { 
            if (theTrees[i] == nullptr)
            { 
                theTrees[i] = temp;
            }
            else 
                temp = combineTrees(temp,theTrees[i]);
                theTrees[i] = nullptr;
        }
    }

    /**
     * Internal method to clone subtree.
     */
    BinomialNode * clone( BinomialNode * t ) const
    {
        if( t == nullptr )
            return nullptr;
        else
            return new BinomialNode{ t->element, clone( t->parent ), clone( t->leftChild ), clone( t->nextSibling ) };
    }
/* finds the hash value in the hash table (pointer) of the element returns the ptr
for usage in the public Find function */
    BinomialNode * find(const Comparable & element)
    {
        return hash.Find(element);
    }

/* this function allows us to move the element in the tree for the remove function
 in order to remove and follow the conditions set in the binomial queue.   */

    BinomialNode * PtrToTop(BinomialNode * & tree)
    {
        if(tree->parent == nullptr)
        {
            return tree;
        }
        
        Comparable old_Elem = tree->element;
        hash.Modify_Ptr(tree->element, tree->parent);
        hash.Modify_Ptr(tree->parent->element, tree);
        tree->element = tree->parent->element;
        tree->parent->element = old_Elem;

        return PtrToTop(tree->parent); //traverse and modify using recursion
    }
    
};

#endif
