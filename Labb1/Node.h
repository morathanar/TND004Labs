#ifndef NODE_H
#define NODE_H

#include <memory>
#include "Set.h"
#include <iostream>

template <typename Object>
struct Node{
    friend class Set;
public:
    Node(const Object & d = Object{}, Node *p = nullptr, Node *n = nullptr) : data {d}, prev{p}, next{n} {}

    Node(const Object && d, Node *p = nullptr, Node *n = nullptr) : data{std::move(d)}, prev{p}, next{n} {}


private:
    const Object data;
    std::shared_ptr<Node<Object>> prev;
    std::shared_ptr<Node<Object>> next;
    //friend ostream& operator<<(ostream& os, const Set& S);
};


#endif // NODE_H
