#ifndef SET_H
#define SET_H
#include "Node.h"
#include <memory>
#include <iostream>
template <typename Object>
class Set{
public:
//constructors
    Set(){
        head->prev = nullptr;
        head->next = tail;
        tail->next = nullptr;
        tail->prev = head;
    }

    Set(Object&& v) {
        head->next = Node<Object>(v, head, tail);
        tail->prev = head->next;
    }

//print operator operator<<
    friend std::ostream& operator<<(std::ostream& os, const Set<Object> & S){
    auto tmp = S.head;
    os << "{ ";
    while (tmp->next != nullptr){
        os << tmp->data << " ";
        tmp = tmp->next;
    }
    os << "}" << std::endl;
    return os;
}

private:


    std::shared_ptr<Node<Object>> head;
    std::shared_ptr<Node<Object>> tail;
};


#endif // SET_H

