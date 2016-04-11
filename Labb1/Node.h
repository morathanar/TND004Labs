#ifndef NODE_H
#define NODE_H

#include <memory>
#include "Set.h"
#include <iostream>

using namespace std;

template <class T>
class Node{
public:
    Node(const T & d = T{}, shared_ptr<Node<T>> p = nullptr, shared_ptr<Node<T>> n = nullptr) : data {d}, prev{p}, next{n} {}

    Node(const T && d, shared_ptr<Node<T>> p = nullptr, shared_ptr<Node<T>> n = nullptr) : data{move(d)}, prev{p}, next{n} {}

    T data;
    shared_ptr<Node<T>> prev;
    shared_ptr<Node<T>> next;
};


#endif // NODE_H
