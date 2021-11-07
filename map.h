#ifndef MYSTL_MAP_H_
#define MYSTL_MAP_H_

#include "RBtree.h"

template <class T_Key, class T_Value>
class map_iterator{
public:
    RBtree_node<T_Key, T_Value>* node;
public:
    map_iterator():node(nullptr){};
    map_iterator(RBtree_node<T_Key, T_Value>* p):node(p){};
    map_iterator(const map_iterator& lis):node(lis.node){}
    map_iterator& operator=(const map_iterator& lis){
        if(this!=&lis)
            node = lis.node;
        return *this;
    }
    ~map_iterator(){};
    map_iterator& operator++(){
        if(node->right_child){
            node = node->right_child;
            while(node->left_child)
                node = node->left_child;
        }
        else{
            RBtree_node<T_Key, T_Value> *parent = node->parent;
            while(node==parent->right_child){
                node = parent;
                parent = node->parent;
            }
            if(node->right_child!=parent)
                node = parent;
        }
        return *this;
    }
    map_iterator& operator--(){
        if(node->left_child){
            std::cout<<"first"<<std::endl;
            node = node->left_child;
            while(node->right_child)
                node = node->right_child;
        }
        else{
            std::cout<<"second"<<std::endl;
            RBtree_node<T_Key, T_Value> *parent = node->parent;
            while(node==parent->left_child){
                node = parent;
                parent = node->parent;
            }
            if(node->left_child!=parent)
                node = parent;
        }
        return *this;
    }
    RBtree_node<T_Key, T_Value>& operator*(){
        return *node;
    }
    bool operator==(const map_iterator& ite){
        return node==ite.node;
    }
    bool operator!=(const map_iterator& ite){
        return node!=ite.node;
    }
};

template <class T_Key, class T_Value>
class map: public RBtree<T_Key, T_Value>{
public:
    typedef map_iterator<T_Key, T_Value> iterator;
public:
    map();
    map(RBtree_node<T_Key, T_Value>* node);
    ~map();
public:
    iterator begin();
    iterator end();
    iterator find(T_Key key);
    T_Value& operator[](T_Key key);
};

template <class T_Key, class T_Value>
map<T_Key, T_Value>::map():RBtree<T_Key, T_Value>(){
}

template <class T_Key, class T_Value>
map<T_Key, T_Value>::map(RBtree_node<T_Key, T_Value>* node):RBtree<T_Key, T_Value>(node){
}

template <class T_Key, class T_Value>
map<T_Key, T_Value>::~map(){
}

template <class T_Key, class T_Value>
map_iterator<T_Key, T_Value> map<T_Key, T_Value>::begin(){
    if(RBtree<T_Key, T_Value>::root_node==nullptr){
        std::cout<<"Now the tree is empty!"<<std::endl;
        return end();
    }
    map_iterator<T_Key, T_Value> it;
    RBtree_node<T_Key, T_Value>* temp_node = RBtree<T_Key, T_Value>::root_node;
    while(temp_node->left_child)
        temp_node = temp_node->left_child;
    it.node = temp_node;
    return it;
}

template <class T_Key, class T_Value>
map_iterator<T_Key, T_Value> map<T_Key, T_Value>::end(){
    map_iterator<T_Key, T_Value> it(RBtree<T_Key, T_Value>::end_node);
    return it;
}

template <class T_Key, class T_Value>
map_iterator<T_Key, T_Value> map<T_Key, T_Value>::find(T_Key key){
    RBtree_node<T_Key, T_Value>* temp_node = RBtree<T_Key, T_Value>::root_node;
    while(temp_node!=nullptr&&temp_node->key!=key){
        if(temp_node->key>key)
            temp_node = temp_node->left_child;
        else
            temp_node = temp_node->right_child;
    }
    map_iterator<T_Key, T_Value> it(temp_node);
    return it;
}

template <class T_Key, class T_Value>
T_Value& map<T_Key, T_Value>::operator[](T_Key key){
    map_iterator<T_Key, T_Value> it = find(key);
    if(it.node!=nullptr)
        return it.node->value;
    RBtree_node<T_Key, T_Value> node(key, 0);
    RBtree<T_Key, T_Value>::insert(node);
    it = find(key);
    return it.node->value;
}
#endif
