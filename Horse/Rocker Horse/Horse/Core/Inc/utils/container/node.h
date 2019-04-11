/*
 * node.h
 * 
 * Created : 02/18/2019
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _NODE_H_
#define _NODE_H_

template <typename T>
class Node
{
public:
        Node(T elem, Node *next = nullptr) {
                set_Next(next);
                set_Element(elem);
        }

        Node(Node &&) = default;
        Node(const Node &) = default;
        Node &operator=(Node &&) = default;
        Node &operator=(const Node &) = default;
        ~Node() { }

        void set_Next(Node *next) { next_ = next; }
        Node* get_Next() { return next_; }
        T get_Element() { return elem_; }
        void set_Element(T elem) { elem_ = elem; }

private:
        Node* next_;
        T elem_;
};

#endif // !_NODE_H_
