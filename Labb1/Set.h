#ifndef SET_H
#define SET_H

#include "Node.h"
#include <memory>
#include <iostream>

using namespace std;

template <class T>
class Set{
public:
//constructors
    Set(){
        head = make_shared<Node<T>>();
        tail = make_shared<Node<T>>();
        head->prev = nullptr;
        head->next = tail;
        tail->next = nullptr;
        tail->prev = head;
    }

    Set(T&& v) {
        head = make_shared<Node<T>>();
        tail = make_shared<Node<T>>();
        head->next = make_shared<Node<T>>(v, head, tail);
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
    shared_ptr<Node<T>> head, tail;
};

#endif // SET_H

