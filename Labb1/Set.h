#ifndef SET_H
#define SET_H

#include <memory>
#include <iostream>
#include <utility>

using namespace std;

template <class T>
class Set{
public:
    // Constructors
    Set();                  // Default
    Set(const T& v);        // Type conversion
    Set(const Set& R);      // Copy
    Set(Set&& S) noexcept;  // Move
    Set(int a[], int n);    // Conversion from sorded array

    // Member functions
    int cardinality();
    bool is_member(const T & v);
    void make_empty();
    bool _empty();

    // Operators
    const Set& operator=(Set S);          // Assignment
    const Set& operator+=(const Set & S); // Union
    const Set& operator*=(const Set & S); // Intersection
    const Set& operator-=(const Set & S); // Difference

    Set operator+(const Set& S) const;
    Set operator*(const Set& S) const;
    Set operator-(const Set& S) const;

    bool operator<=(Set& S);
    bool operator<(Set& S);
    bool operator==(Set& S);
    bool operator!=(Set& S);

    // Formatted output operator<<
    friend ostream& operator<<(ostream& os, const Set<T> & S){
        auto tmp = S.head->next;
        os << "{ ";
        while (tmp->next != nullptr){
            os << tmp->data << " ";
            tmp = tmp->next;
        }
        os << "}";
        return os;
    }

private:
    //class Node
    class Node {
    public:
        Node(const T & d = T{}, shared_ptr<Node> p = nullptr, shared_ptr<Node> n = nullptr)
            : data {d}, prev{p}, next{n} {}

        Node(const T && d, shared_ptr<Node> p = nullptr, shared_ptr<Node> n = nullptr)
            : data{move(d)}, prev{p}, next{n} {}

        T data;
        weak_ptr<Node> prev;
        shared_ptr<Node> next;
    };

    // Helper functions to manage insertion/removal
    void insert_after(shared_ptr<Node> p, const T& v) {
        auto tmp = make_shared<Node>(v, p, p->next);
        p->next->prev = tmp;
        p->next = tmp;
    };
    /*
    * Removes node n and returns a pointer to the node that was after the removed node
    */
    void remove_node(shared_ptr<Node> n) {
        auto p = n->prev.lock();
        p->next = n->next;
        n->next->prev = n->prev;
    };

    shared_ptr<Node> head, tail;
};

/*
 * Constructors
 */
template <class T>
Set<T>::Set() {
    head = make_shared<Node>();
    tail = make_shared<Node>();
    head->next = tail;
    tail->prev = head;
}

template <class T>
Set<T>::Set(const T& v) : Set() {
    head->next = make_shared<Node>(v, head, tail);
    tail->prev = head->next;
}

template<class T>
Set<T>::Set(const Set& R): Set() {
    auto tmpR = R.head->next;
    auto tmpS = head;
    while(tmpR->next != nullptr){
        tmpS->next = make_shared<Node>(tmpR->data, tmpS, tmpS->next);
        tmpR = tmpR->next;
        tmpS = tmpS->next;
    }
}

template<class T>
Set<T>::Set(Set&& S) noexcept : head(std::move(S.head)), tail(std::move(S.tail)) {
    S.head = nullptr;
    S.tail = nullptr;
}

template<class T>
Set<T>::Set(int a[], int n): Set() {
    auto tmp = head;
    int i = 0;
    while(i < n){
        insert_after(tmp, a[i++]);
        tmp = tmp->next;
    }
}


/*
* Puclic member functions
*/
template<class T>
int Set<T>::cardinality() {
    auto tmp = head->next;
    int i = 0;
    while(tmp != tail){
        i++;
        tmp = tmp->next;
    }
    return i;
}

template<class T>
bool Set<T>::is_member(const T & v) {
    auto tmp = head->next;
    while(tmp != tail){
        if (tmp->data == v) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

template<class T>
void Set<T>::make_empty() {
    while(head->next != tail){
        remove_node(head->next);
    }
}
template<class T>
bool Set<T>::_empty(){
    if(head->next == tail)return true;
    else return false;
}

/*
* Operators
*/
template<class T>
const Set<T>& Set<T>::operator=(Set S) {
    std::swap(head, S.head);
    std::swap(tail, S.tail);
    return *this;
}

template<class T>
const Set<T>& Set<T>::operator+=(const Set & S) {
    auto tmpR = head;
    auto tmpS = S.head;

    while (tmpS->next != S.tail) {
        if (tmpR->next == tail) {
            insert_after(tmpR, tmpS->next->data);
            tmpR = tmpR->next;
            tmpS = tmpS->next;
        } else if (tmpS->next->data > tmpR->next->data) {
            // Not the right place, advance R
            tmpR = tmpR->next;
        } else if (tmpS->next->data == tmpR->next->data) {
            // Element already in set
            tmpS = tmpS->next;
        } else if (tmpS->next->data < tmpR->next->data) {
            // Insert element before tested element
            insert_after(tmpR, tmpS->next->data);
            tmpS = tmpS->next;
        }
    }

    return *this;
}

template<class T>
const Set<T>& Set<T>::operator*=(const Set & S) {
    auto tmpR = head;
    auto tmpS = S.head;

    /*
    * Loop through this set and remove all elements in this set that is not in S
    */
    while(tmpR->next != tail) {
        if (tmpS->next == S.tail) {
            // Reached end of S, remove the rest of the nodes
            remove_node(tmpR->next);
        } else if (tmpS->next->data > tmpR->next->data) {
            // Data not found in S, remove node from R
            remove_node(tmpR->next);
            // tmpR->next is now pointing to the node after the one that was removed
        } else if (tmpS->next->data == tmpR->next->data) {
            // Data in both sets, keep it. Advance R
            tmpR = tmpR->next;
        } else {
            // Data not found yet, keep advancing S
            tmpS = tmpS->next;
        }
    }

    return *this;
}

template<class T>
const Set<T>& Set<T>::operator-=(const Set & S) {
    auto tmpR = head;
    auto tmpS = S.head;

    /*
    * For each element in S, loop through elements in this set until
    * a bigger element is found. If an equal element is found, remove it
    */
    while(tmpS->next != S.tail && tmpR->next != tail) {
        if (tmpS->next->data == tmpR->next->data) {
            remove_node(tmpR->next);
            // An element was removed from R, no need to check the same node in S again
            // tmpR->next is now pointing to the node after the one that was removed
            tmpS = tmpS->next;
        } else if (tmpS->next->data > tmpR->next->data) {
            // Advance R
            tmpR = tmpR->next;
        } else {
            // Advance S
            tmpS = tmpS->next;
        }
    }

    return *this;
}

template<class T>
Set<T> Set<T>::operator+(const Set& S) const {
    Set<T> ret(*this);
    return ret += S;
}

template<class T>
Set<T> Set<T>::operator*(const Set& S) const {
    Set<T> ret(*this);
    return ret *= S;
}

template<class T>
Set<T> Set<T>::operator-(const Set& S) const {
    Set<T> ret(*this);
    return ret -= S;
}

template<class T>
bool Set<T>::operator<=(Set& S){
    auto tmpS = S.head->next;
    auto tmpR = head->next;
    if(S._empty() || _empty()) return false;
    while(tmpS->next != S.tail){
        if(tmpR->next == tail) return true;
        if(tmpR->data == tmpS->data){
            tmpR = tmpR->next;
            tmpS = tmpS->next;
        }
        else tmpS = tmpS->next;
    }
    return false;
}

template<class T>
bool Set<T>::operator<(Set& S){
    if( ( S.cardinality() > cardinality() ) && (*this <= S) ) return true;
    else return false;
}

template<class T>
bool Set<T>::operator==(Set& S){
    if( ( *this <= S ) && ( S <= *this ) )return true;
    else return false;
}

template<class T>
bool Set<T>::operator!=(Set& S){
    if( *this == S )return false;
    else return true;
}
#endif // SET_H

