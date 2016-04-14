#ifndef SET_H
#define SET_H

#include <memory>
#include <iostream>
#include <utility>

using namespace std;

template <class T>
class Set{
public:
//constructors
    //initialize empty set
    Set(){
        head = make_shared<Node>();
        tail = make_shared<Node>();
        head->next = tail;
        tail->prev = head;
    }
    //initialize set S = {v}
    Set(const T& v) : Set() {
        head->next = make_shared<Node>(v, head, tail);
        tail->prev = head->next;
    }
    //initialize set S with set R
    Set(const Set& R): Set() {
        auto tmpR = R.head->next;
        auto tmpS = head;
        while(tmpR->next != nullptr){
            tmpS->next = make_shared<Node>(tmpR->data, tmpS, tmpS->next);
            tmpR = tmpR->next;
            tmpS = tmpS->next;
        }
    }

    //move constructor, flyttar ett helt set eller ett värde i settet?
    Set(Set&& S) noexcept : head(std::move(S.head)), tail(std::move(S.tail)) {S.head = nullptr; S.tail = nullptr; } // A(A&& o) noexcept : s(std::move(o.s)) { }

    //create a set R from a sorted array a of n integers
    Set(int a[], int n): Set() {
        auto tmp = head;
        int i = 0;
        while(i < n){
            insert_after(tmp, a[i++]);
            tmp = tmp->next;
        }
    }

    //member functions
    //check if a set is empty
    bool _empty(){
        if(this.head->next = this.tail){ return true;}
        else{return false;}
    }

    //cardinality function that returns legth of set
    int& cardinality(T & S){
        auto tmp = S.head->next;
        int i = 0;
        while(tmp != S.tail){
            i++;
            tmp = tmp->next;
        }
        return i+1;
    }
    //memberfunction to check if value v is a part of the set
    bool is_member(T & v){
        auto tmp = this.head;
        if(this._empty()){ return true;}
        else{
            while(tmp->next->value != v && tmp->next != nullptr){
                tmp = tmp->next;
            }
            if(tmp->next == nullptr){return false;}
            else{return true;}
        }
    }

    //member function to make set R empty
    void make_empty(){
        while(this.head->next != this.tail){
            remove_node(this.head->next);
        }
    }

    Set& operator=(Set S){
        std::swap(head, S.head);
        std::swap(tail, S.tail);
        return *this;
    }
#if 0
    //overloaded operator+= such that R+=S => R = R U S
    void operator+=(T & S){
        auto tmpT = this.head;
        auto tmpS = S.head;
        if(this._empty()){
            this.head(std::move(S.head));
        }else if(S._empty()){
        }else{
            if(R.cardinality() > S.cardinality()){auto tmp = }
        }
    }
#endif

//print operator operator<<
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
    // member functions of class Node
    // Helper functions to manage insertion/removal
    void insert_after(shared_ptr<Node> p, const T& v) {
        auto tmp = make_shared<Node>(v, p, p->next);
        p->next->prev = tmp;
        p->next = tmp;
    };
    // delete node that was removed or does shared_ptr manage that?
    void remove_node(shared_ptr<Node> p) {
        p->prev->next = p->next;
        p->next->prev = p->prev;
    };

    //private part of set starts here
    shared_ptr<Node> head, tail;
};



#endif // SET_H

