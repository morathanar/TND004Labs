#ifndef SET_H
#define SET_H

#include "Node.h"
#include <memory>
#include <iostream>

using namespace std;

template <class Object>
class Set{
public:
//constructors
    Set(){
        head = make_shared<Node<Object>>();
        tail = make_shared<Node<Object>>();
        head->prev = nullptr;
        head->next = tail;
        tail->next = nullptr;
        tail->prev = head;
    }

    Set(Object&& v) {
        head = make_shared<Node<Object>>();
        tail = make_shared<Node<Object>>();
        head->next = make_shared<Node<Object>>(v, head, tail);
        tail->prev = head->next;
    }

//print operator operator<<
    friend ostream& operator<<(ostream& os, const Set<Object> & S){
    auto tmp = S.head;
    os << "{ ";
    while (tmp->next != nullptr){
        os << tmp->data << " ";
        tmp = tmp->next;
    }
    os << "}" << endl;
    return os;
}

private:
    shared_ptr<Node<Object>> head, tail;
};

#endif // SET_H

