#ifndef SET_H
#define SET_H

#include <memory>
#include <iostream>

using namespace std;

template <class T>
class Set{
public:
//constructors
    Set(){
        head = make_shared<Node>();
        tail = make_shared<Node>();
        head->next = tail;
        tail->prev = head;
    }

    Set(T&& v) {
        head = make_shared<Node>();
        tail = make_shared<Node>();
        head->next = make_shared<Node>(v, head, tail);
        tail->prev = head->next;
    }

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
    void insert_after(shared_ptr<Node> p, T v) {
        auto tmp = make_shared<Node>(v, p, p->next);
        p->next->prev = tmp;
        p->next = tmp;
    };

    void remove_node(shared_ptr<Node> p) {
        p->prev->next = p->next;
        p->next->prev = p->prev;
    };

    shared_ptr<Node> head, tail;
};

#endif // SET_H

