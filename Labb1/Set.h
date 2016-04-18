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

    // Operators
    const Set& operator=(Set S);          // Assignment
    const Set& operator+=(const Set & S); // Union
    const Set& operator*=(const Set & S); // Intersection

    // Formatted output operator<<
    friend ostream& operator<<(ostream& os, const Set<T> & S){
        auto tmp = S.head->next;
        os << "{ ";
        while (tmp->next != nullptr){
            os << tmp->data << " ";
            tmp = tmp->next;
        }
        os << "}" << endl;
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
    void remove_node(shared_ptr<Node> n) {
        auto p = n->prev.lock();
        p->next = n->next;
        n->next->prev = n->prev;
    };

    /*
    * Finds the position for element v, starting the search at node n. Returns
    * the node at which to continue searching the next time.
    */
    shared_ptr<Node> insert_element_after(const T & v, shared_ptr<Node> n ){
        // Loop until end of list, or until the right position is found
        while (n->next != tail) {
            if (n->data == v) {
                // No insertion needed
                return n;
            } else if (n->next->data > v){
                // Insertion time!
                insert_after(n, v);
                return n->next;
            }
            n = n->next;
        }
        // List was empty
        insert_after(n, v);
        return n->next;
    }

    shared_ptr<Node> remove_element_at(const T & v, shared_ptr<Node> n){
        while(n->next != tail){
            if(n->next->data != v && n->next->data < v){
                remove_node(n->next);
            }
            n = n->next;
        }
        return n;
    }

    //

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
const Set<T>& Set<T>::operator+=(const Set & S){
    auto src = S.head->next;
    auto dest = head->next;

    while (src != S.tail) {
        dest = insert_element_after(src->data, dest);
        src = src->next;
    }

    return *this;
}

template<class T>
const Set<T>& Set<T>::operator*=(const Set & S){
    auto tmpR = head->next;
    auto tmpS = S.head;

    while(tmpS->next != S.tail){
        tmpR = remove_element_at(tmpS->data, tmpR);
        tmpS = tmpS->next;
    }

    return *this;
}



#endif // SET_H

