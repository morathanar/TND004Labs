
/*
  Author: Aida Nordman
  Course: TND004, Lab 2
  Description: template class HashTable represents an open addressing hash table
              (also known as closed_hashing) with linear probing
*/

#include "Item.h"

#include <iostream>
#include <iomanip>

using namespace std;

const int NOT_FOUND = -1;
const double MAX_LOAD_FACTOR = 0.5;


//Template class to represent an open addressing hash table using linear probing to resolve collisions
//Internally the table is represented as an array of pointers to Items
template <typename Key_Type, typename Value_Type>
class HashTable
{
public:

    //New type HASH: pointer to a hash function
    typedef unsigned (*HASH)(Key_Type, int);


    //Constructor to create a hash table
    //table_size is number of slots in the table (next prime number is used)
    //f is the hash function
    HashTable(int table_size, HASH f);


    //Destructor
    ~HashTable();


    //Return the load factor of the table, i.e. percentage of slots in use or deleted
    double loadFactor() const
    {
        return (double) (nItems+nDeleted) / _size;
    }


    //Return number of items stored in the table
    unsigned get_number_OF_items() const
    {
        return nItems;
    }

    //Return the total number of visited slots (during search, insert, remove, or re-hash)
    unsigned get_total_visited_slots() const
    {
        return total_visited_slots;
    }

    //Return the total number of call to new Item()
    unsigned get_count_new_items() const
    {
        return count_new_items;
    }


    //Return a pointer to the value associated with key
    //If key does not exist in the table then nullptr is returned
    const Value_Type* _find(const Key_Type& key);


    //Insert the Item (key, v) in the table
    //If key already exists in the table then change the value associated with key to v
    //Re-hash if the table reaches the MAX_LOAD_FACTOR
    void _insert(const Key_Type& key, const Value_Type& v);


    //Remove Item with key, if the item exists
    //If an Item was removed then return true
    //otherwise, return false
    bool _remove(const Key_Type& key);

    //Overloaded subscript operator
    //If key is not in the table then insert a new Item = (key, Value_Type())
    Value_Type& operator[](const Key_Type& key);


    //Display all items in table T to stream os
    friend ostream& operator<<(ostream& os, const HashTable& T)
    {
        for (unsigned i = 0; i < T._size; ++i)
        {
            if (T.hTable[i] && T.hTable[i] != Deleted_Item<Key_Type,Value_Type>::get_Item())
            {
                os << *T.hTable[i] << endl;
            }
        }

        return os;
    }


    //Display the table for debug and testing purposes
    //Thus, empty and deleted entries are also displayed
    void display(ostream& os);


private:

    /* ********************************** *
    * Data members                        *
    * *********************************** */

    //Number of slots in the table, a prime number
    unsigned _size;

    //Hash function
    const HASH h;

    //Number of items stored in the table
    //Instances of Deleted_Items are not counted
    unsigned nItems;

    //Number of slots that are marked as deleted
    unsigned nDeleted;

    //Table is an array of pointers to Items
    //Each slot of the table stores a pointer to an Item =(key, value)
    Item<Key_Type, Value_Type>** hTable;

    //Some statistics
    unsigned total_visited_slots;  //total number of visited slots
    unsigned count_new_items;      //number of calls to new Item()


    /* ********************************** *
    * Auxiliar member functions           *
    * *********************************** */

    //Disable copy constructor!!
    HashTable(const HashTable &) = delete;

    //Disable assignment operator!!
    const HashTable& operator=(const HashTable &) = delete;

    unsigned help_find(const Key_Type& key);

    void rehash();
};


//Test if a number is prime
bool isPrime( int n );

//Return a prime number at least as large as n
int nextPrime( int n );


/* ********************************** *
* Member functions implementation     *
* *********************************** */

//Constructor to create a hash table
//table_size number of slots in the table (next prime number is used)
//f is the hash function
template <typename Key_Type, typename Value_Type>
HashTable<Key_Type, Value_Type>::HashTable(int table_size, HASH f)
    : _size(table_size), h(f), nItems(0), nDeleted(0), total_visited_slots(0), count_new_items(0)
{
    //cout << "ctor, " << "size:" << table_size << endl;
    hTable = new Item<Key_Type, Value_Type>*[table_size]();
}


//Destructor
template <typename Key_Type, typename Value_Type>
HashTable<Key_Type, Value_Type>::~HashTable()
{
    //cout << "dtor" << endl;
    for (size_t i = 0; i < _size; i++) {
        if(hTable[i] != nullptr) {
            delete hTable[i];
        }
    }
    delete[] hTable;
}


//Return a pointer to the value associated with key
//If key does not exist in the table then nullptr is returned
template <typename Key_Type, typename Value_Type>
const Value_Type* HashTable<Key_Type, Value_Type>::_find(const Key_Type& key)
{
    auto tmp_hash = help_find(key);

    //cout << "_find, " << "key:" << key << " hash:" << tmp_hash << endl;

    if (hTable[tmp_hash] != nullptr) {
        return &hTable[tmp_hash]->get_value();
    }
    // key not found
    return nullptr;
}

//Insert the Item (key, v) in the table
//If key already exists in the table then change the value associated with key to v
//Re-hash if the table reaches the MAX_LOAD_FACTOR
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::_insert(const Key_Type& key, const Value_Type& v)
{
    //cout << "_insert, " << "key:" << key << " hash:" << tmp_hash << " value:" << v << endl;

    auto tmp_hash = help_find(key);

    if (hTable[tmp_hash] == nullptr) {
        // key was not already in hash table
        hTable[tmp_hash] = new Item<Key_Type, Value_Type>(key,v);
        count_new_items++;
        nItems++;
    } else {
        // key was already in there, update the value.
        hTable[tmp_hash]->get_value() = v;
    }

    if(loadFactor() >= 0.5) {
        rehash();
    }

    return;
}


//Remove Item with key, if the item exists
//If an Item was removed then return true
//otherwise, return false
template <typename Key_Type, typename Value_Type>
bool HashTable<Key_Type, Value_Type>::_remove(const Key_Type& key)
{
    auto tmp_hash = help_find(key);

    if (hTable[tmp_hash] != nullptr) {
        // Key found, delete item.
        delete hTable[tmp_hash];
        hTable[tmp_hash] = Deleted_Item<Key_Type, Value_Type>::get_Item();
        return true;
    }

    // No value was deleted
    return false;
}

// Return reference to the value of the object that has the supplied key..
template <typename Key_Type, typename Value_Type>
Value_Type& HashTable<Key_Type, Value_Type>::operator[](const Key_Type& key)
{
    auto tmp_hash = help_find(key);
    if (hTable[tmp_hash] == nullptr) {
        // Key not found, insert new default value

        hTable[tmp_hash] = new Item<Key_Type, Value_Type>(key,Value_Type());
        count_new_items++;
        nItems++;

        if (loadFactor() > 0.5) {
            rehash();

            // Will always find key because we just inserted it.
            return hTable[help_find(key)]->get_value();
        }
        return hTable[tmp_hash]->get_value();
    }

    // Key found, return ref to value.
    return hTable[tmp_hash]->get_value();
}

//Display the table for debug and testing purposes
//This function is used for debugging and testing purposes
//Thus, empty and deleted entries are also displayed
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::display(ostream& os)
{
    os << "-------------------------------\n";
    os << "Number of items in the table: " << get_number_OF_items() << endl;
    os << "Load factor: " << fixed << setprecision(2) << loadFactor() << endl;

    for (unsigned i = 0; i < _size; ++i)
    {
        os << setw(6) << i << ": ";

        if (!hTable[i])
        {
            os << "null" << endl;
        }
        else if (hTable[i] == Deleted_Item<Key_Type, Value_Type>::get_Item())
        {
            os << "deleted" << endl;
        }
        else
        {
            os << *hTable[i]
               << "  (" << h(hTable[i]->get_key(),_size) << ")" << endl;
        }
    }

    os << endl;
}


/* ********************************** *
* Auxiliar member functions           *
* *********************************** */
//Add any if needed


// Finds the element represented by key or the slot where it should be placed
// by using linear probing.
template <typename Key_Type, typename Value_Type>
unsigned HashTable<Key_Type, Value_Type>::help_find(const Key_Type& key)
{
    auto tmp_hash = h(key, _size);

    //cout << "help_find, " << "key:" << key << " hash:" << tmp_hash << endl;

    while(hTable[tmp_hash] != nullptr) {
        total_visited_slots++;
        if (hTable[tmp_hash]->get_key() == key) {
            return tmp_hash;
        }
        // Wrap around to 0
        tmp_hash++;
        tmp_hash = tmp_hash % _size;
    }
    total_visited_slots++;

    // key was not found, return the currently selected slot. We are guaranteed to always find
    // an empty slot because table will rehash if load factor gets to 0.5
    return tmp_hash;
}

template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::rehash()
{
    auto old_hTable = hTable;
    auto old_size = _size;

    //nItems = 0;

    // Allocate a new array
    _size = nextPrime(2 * old_size);
    hTable = new Item<Key_Type, Value_Type>*[_size]();

    cout << "Rehash..\n" <<
            "New table size " << _size << endl;

    // Copy elements over to new array
    for (size_t i = 0; i < old_size; i++) {
        if (old_hTable[i] != nullptr && old_hTable[i] != Deleted_Item<Key_Type, Value_Type>::get_Item()) {
            //_insert(old_hTable[i]->get_key(), old_hTable[i]->get_value());
            hTable[help_find(old_hTable[i]->get_key())] = old_hTable[i];
            old_hTable[i] = nullptr;
        }
    }

    // delete old array
    for (size_t i = 0; i < old_size; i++) {
        if(old_hTable[i] != nullptr) {
            delete old_hTable[i];
        }
    }
    delete[] old_hTable;
}

/* ********************************** *
* Functions to find prime numbers     *
* *********************************** */


//Test if a number is prime
bool isPrime( int n )
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}


//Return a prime number at least as large as n
int nextPrime( int n )
{
    if( n % 2 == 0 )
        n++;

    for(; !isPrime( n ); n += 2 );

    return n;
}


