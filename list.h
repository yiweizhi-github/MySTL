#ifndef MYSTL_LIST_H_
#define MYSTL_LIST_H_

#include "alloc.h"

template <class T>
struct list_node{
    list_node* prev;
    list_node* next;
    T data;
};

template <class T>
struct list_iterator{
    list_node<T>* node;

    list_iterator(list_node<T>* p):node(p){}
    list_iterator(const list_iterator& lis):node(lis.node){}
    list_iterator& operator=(const list_iterator& lis){
        if(this!=&lis)
            node = lis.node;
        return *this;
    }
    list_iterator operator+(size_t n){
        list_iterator temp(*this);
        for(size_t i=0;i<n;i++)
            temp.node = temp.node->next;
        return temp;
    }
    list_iterator operator-(size_t n){
        list_iterator temp(*this);
        for(size_t i=0;i<n;i++)
            temp.node = temp.node->prev;
        return temp;
    }
    size_t operator-(const list_iterator& ite){
        size_t result = 0;
        list_iterator temp(*this);
        while(temp.node!=ite.node){
            result++;
            temp.node = temp.node->prev;
        }
        return result;
    }
    list_iterator& operator++(){
        node = node->next;
        return *this;
    }
    list_iterator operator++(int i){
        list_iterator temp = *this;
        node = node->next;
        return temp;
    }
    list_iterator& operator--(){
        node = node->prev;
        return *this;
    }
    list_iterator operator--(int i){
        list_iterator temp = *this;
        node = node->prev;
        return temp;
    }
    T& operator*(){
        return node->data;
    }
    bool operator==(const list_iterator& ite){
        return node==ite.node;
    }
    bool operator!=(const list_iterator& ite){
        return node!=ite.node;
    }
};

template <class T>
class list{
public:
    typedef list_iterator<T> iterator;
private:
    list_node<T>* node;
    size_t length;
public:
    list();
    list(size_t n, T val);
    list(const list& lis);
    list& operator=(const list& lis);
    list(list&& lis);
    list& operator=(list&& lis);
    ~list();
public:
    iterator begin();
    iterator end();
    size_t size();
    T& front();
    T& back();
    iterator insert(iterator pos, const T& x);
    iterator insert(iterator pos, size_t n, const T& x);
    iterator erase(iterator pos);
    iterator erase(iterator pos_start, iterator pos_end);
    void push_back(const T& x);
};

template <class T>
list<T>::list(){
    node = nullptr;
    length = 0;
}

template <class T>
list<T>::list(size_t n, T val){
    list_node<T> *head, *prev, *now, *temp;
    head = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
    new (&head->data) T(val);
    prev = nullptr;
    now = head;
    for(size_t i=0;i<n-1;i++){
        temp = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        new (&temp->data) T(val);
        now->prev = prev;
        prev = now;
        now->next = temp;
        now = now->next;
    }
    temp = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
    now->prev = prev;
    prev = now;
    now->next = temp;
    now = now->next;
    now->prev = prev;
    now->next = head;
    head->prev = now;
    node = now;
    length = n;
}

template <class T>
list<T>::list(const list& lis){
    list_node<T> *head, *prev, *now, *temp, *lis_node;
    lis_node = lis.node->next;
    T val = lis_node->data;
    head = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
    new (&head->data) T(val);
    prev = nullptr;
    now = head;
    for(size_t i=0;i<lis.length-1;i++){
        lis_node = lis_node->next;
        val = lis_node->data;
        temp = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        new (&temp->data) T(val);
        now->prev = prev;
        prev = now;
        now->next = temp;
        now = now->next;
    }
    temp = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
    now->prev = prev;
    now->next = temp;
    now = now->next;
    now->next = head;
    head->prev = now;
    node = now;
    length = lis.length;
}

template <class T>
list<T>& list<T>::operator=(const list& lis){
    if(this!=&lis){
        list_node<T> *temp;
        for(size_t i=0;i<=length;i++){
            temp = node;
            alloc::deallocate(node, sizeof(list_node<T>));
            node = temp->next;
        }
        list_node<T> *head, *prev, *now, *lis_node;
        lis_node = lis.node->next;
        T val = lis_node->data;
        head = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        new (&head->data) T(val);
        prev = nullptr;
        now = head;
        for(size_t i=0;i<lis.length-1;i++){
            lis_node = lis_node->next;
            val = lis_node->data;
            temp = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
            new (&temp->data) T(val);
            now->prev = prev;
            prev = now;
            now->next = temp;
            now = now->next;
        }
        temp = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        now->prev = prev;
        now->next = temp;
        now = now->next;
        now->next = head;
        head->prev = now;
        node = now;
        length = lis.length;
    }
    return *this;
}

template <class T>
list<T>::list(list&& lis){
    node = lis.node;
    length = lis.length;
    lis.node = nullptr;
    lis.length = 0;
}

template <class T>
list<T>& list<T>::operator=(list&& lis){
    if(this!=&lis){
        list_node<T>* temp;
        for(int i=0;i<=length;i++){
            temp = node;
            alloc::deallocate(node, sizeof(list_node<T>));
            node = temp->next;
        }
        node = lis.node;
        length = lis.length;
        lis.node = nullptr;
        lis.length = 0;
    }
    return *this;
}

template <class T>
list<T>::~list(){
    if(node!=nullptr){
        list_node<T>* temp;
        for(size_t i=0;i<=length;i++){
            temp = node;
            alloc::deallocate(node, sizeof(list_node<T>));
            node = temp->next;
        }
        node = nullptr;
        length = 0;
    }
}

template <class T>
list_iterator<T> list<T>::begin(){
    if(node){
        list_iterator<T> result(node->next);
        return result;
    }
    else{
        list_iterator<T> result(nullptr);
        return result;
    }
}

template <class T>
list_iterator<T> list<T>::end(){
    list_iterator<T> result(node);
    return result;
}

template <class T>
size_t list<T>::size(){
    return length;
}

template <class T>
T& list<T>::front(){
    return node->next->data;
}

template <class T>
T& list<T>::back(){
    return node->prev->data;
}

template <class T>
list_iterator<T> list<T>::insert(list_iterator<T> pos, const T& x){
    list_node<T> *p, *temp, *prev;
    if(node==nullptr){
        temp = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        new (&temp->data) T(x);
        prev = temp;
        temp = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        prev->prev = temp;
        prev->next = temp;
        temp->prev = prev;
        temp->next = prev;
        node = temp;
        length++;
        list_iterator<T> result(node->next);
        return result;
    }
    else{
        prev = node->prev;
        temp = node;
        while(temp!=pos.node){
            prev = temp;
            temp = temp->next;
        }
        p = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        pos.node = p;
        new (&p->data) T(x);
        p->prev = prev;
        prev->next = p;
        p->next = temp;
        temp->prev = p;
        length++;
        return pos;
    }
}

template <class T>
list_iterator<T> list<T>::insert(list_iterator<T> pos, size_t n, const T& x){
    list_node<T> *p, *temp, *prev, *p_prev, *p_next;
    if(node==nullptr){
        temp = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        new (&temp->data) T(x);
        p = temp;
        p_prev = nullptr;
        for(size_t i=0;i<n-1;i++){
            p_prev = p;
            p_next = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
            new (&p_next->data) T(x);
            p->next = p_next;
            p = p_next;
            p->prev = p_prev;
        }
        p_prev = p;
        p_next = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        p->next = p_next;
        p = p_next;
        p->prev = p_prev;
        p->next = temp;
        temp->prev = p;
        node = p;
        length += n;
        list_iterator<T> result(node->next);
        return result;
    }
    else{
        prev = node->prev;
        temp = node;
        while(temp!=pos.node){
            prev = temp;
            temp = temp->next;
        }
        p = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        pos.node = p;
        new (&p->data) T(x);
        p->prev = prev;
        prev->next = p;
        for(size_t i=0;i<n-1;i++){
            p_prev = p;
            p_next = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
            new (&p_next->data) T(x);
            p->next = p_next;
            p = p_next;
            p->prev = p_prev;
        }
        p->next = temp;
        temp->prev = p;
        length += n;
        return pos;
    }
}

template <class T>
list_iterator<T> list<T>::erase(list_iterator<T> pos){
    if(pos.node==node)  //尾节点不可删除
        return pos;
    list_node<T> *temp = node, *prev = nullptr;
    while(temp!=pos.node){
        prev = temp;
        temp = temp->next;
    }
    prev->next = temp->next;
    temp->next->prev = prev;
    alloc::deallocate(temp, sizeof(list_node<T>));
    pos.node = prev->next;
    length--;
    return pos;
}


template <class T>
list_iterator<T> list<T>::erase(list_iterator<T> pos_start, list_iterator<T> pos_end){
    //std::cout<<pos_start.node<<" 1234 "<<pos_end.node<<std::endl;
    list_node<T> *p, *temp = node, *prev = nullptr;
    while(temp!=pos_start.node){
        prev = temp;
        temp = temp->next;
    }
    p = temp;
    while(temp!=pos_end.node){
        temp = temp->next;
        alloc::deallocate(p, sizeof(list_node<T>));
        length--;
        p = temp;
        if(p==node) //当输入的删除区间包括尾节点时，删除到尾节点停止
            break;
    }
    std::cout<<"prev: "<<prev<<" temp: "<<temp<<std::endl;
    temp->prev = prev;
    prev->next = temp;
    return pos_end;
}

template <class T>
void list<T>::push_back(const T& x){
    list_node<T> *p;
    p = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
    new (&p->data) T(x);
    if(node==nullptr){
        list_node<T> *temp;
        temp = (list_node<T>*)alloc::allocate(sizeof(list_node<T>));
        p->prev = temp;
        p->next = temp;
        temp->prev = p;
        temp->next = p;
        node = temp;
        length++;
    }
    else{
        p->prev = node->prev;
        node->prev->next = p;
        p->next = node;
        node->prev = p;
        length++;
    }
}
#endif
