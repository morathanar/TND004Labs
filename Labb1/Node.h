#ifndef NODE_H
#define NODE_H

#include <memory>
#include "Set.h"
#include <iostream>

using namespace std;

template <class Object>
class Node{
public:
    Node(const Object & d = Object{}, shared_ptr<Node<Object>> p = nullptr, shared_ptr<Node<Object>> n = nullptr) : data {d}, prev{p}, next{n} {}

    Node(const Object && d, shared_ptr<Node<Object>> p = nullptr, shared_ptr<Node<Object>> n = nullptr) : data{move(d)}, prev{p}, next{n} {}

    Object data;
    shared_ptr<Node<Object>> prev;
    shared_ptr<Node<Object>> next;
};


#endif // NODE_H
